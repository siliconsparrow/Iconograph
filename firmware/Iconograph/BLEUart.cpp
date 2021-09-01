/*
 * BLEUart.cpp
 *
 *  Created on: 6 Jul 2021
 *      Author: adam
 */

// NOTE: MAX GATT PAYLOAD SIZE IS 20 BYTES

#include "BLEUart.h"

#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "nrf_sdh_ble.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "app_timer.h"
#include "ble_nus.h"
#include "app_util_platform.h"

#define APP_BLE_CONN_CFG_TAG            1                                           /**< A tag identifying the SoftDevice BLE configuration. */

#define DEVICE_NAME                     "Iconograph"                               /**< Name of device. Will be included in the advertising data. */
#define NUS_SERVICE_UUID_TYPE           BLE_UUID_TYPE_VENDOR_BEGIN                  /**< UUID type for the Nordic UART Service (vendor specific). */

#define APP_BLE_OBSERVER_PRIO           3                                           /**< Application's BLE observer priority. You shouldn't need to modify this value. */
#define APP_ADV_INTERVAL                64                                          /**< The advertising interval (in units of 0.625 ms. This value corresponds to 40 ms). */
#define APP_ADV_DURATION                18000                                       /**< The advertising duration (180 seconds) in units of 10 milliseconds. */
#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(20, UNIT_1_25_MS)             /**< Minimum acceptable connection interval (20 ms), Connection interval uses 1.25 ms units. */
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(75, UNIT_1_25_MS)             /**< Maximum acceptable connection interval (75 ms), Connection interval uses 1.25 ms units. */
#define SLAVE_LATENCY                   0                                           /**< Slave latency. */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)             /**< Connection supervisory timeout (4 seconds), Supervision Timeout uses 10 ms units. */
#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000)                       /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000)                      /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                           /**< Number of attempts before giving up the connection parameter negotiation. */
#define DEAD_BEEF                       0xDEADBEEF                                  /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

/* Resource reserved by Softdevice S112
 *
 * CLOCK
 * POWER
 * BPROT
 * RADIO
 * TIMER0
 * RTC0
 * TEMP
 * RNG
 * ECB
 * CCM
 * AAR
 * EGU1/SWI1
 * EGU5/SWI5
 * NVMC
 * FICR
 * UICR
 * NVIC
 *
 * SWI Channels 0, 1, 2, 3, 4 and 5
 * SVC numbers 0x10-0xFF
 *
 * Interrupt priorities 0, 1 and 4
 *
 */


static ble_uuid_t m_adv_uuids[]          =                                          /**< Universally unique service identifier. */
{
    {BLE_UUID_NUS_SERVICE, NUS_SERVICE_UUID_TYPE}
};

// Generated variables. These must be in global scope because macros.
BLE_NUS_DEF(m_nus, NRF_SDH_BLE_TOTAL_LINK_COUNT);                                   /**< BLE NUS service instance. */
NRF_BLE_GATT_DEF(m_gatt);                                                           /**< GATT module instance. */
NRF_BLE_QWR_DEF(m_qwr);                                                             /**< Context for the Queued Write module.*/
BLE_ADVERTISING_DEF(m_advertising);                                                 /**< Advertising module instance. */

// Main BLEUart instance.
BLEUart *g_uartInstance = 0;

// Softdevice ASSERT
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}

// Queued Write Module error handler.
static void nrf_qwr_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

// Handle events from the GATT library.
void gatt_evt_handler(nrf_ble_gatt_t * p_gatt, nrf_ble_gatt_evt_t const * p_evt)
{
	if(0 != g_uartInstance) {
		g_uartInstance->evtGatt(p_gatt, p_evt);
	}
}

// Function for handling the data from the Nordic UART Service.
static void nus_data_handler(ble_nus_evt_t * p_evt)
{
    if (p_evt->type == BLE_NUS_EVT_RX_DATA && 0 != g_uartInstance) {
    	g_uartInstance->rxDataHandler(p_evt->params.rx_data.p_data, p_evt->params.rx_data.length);
    }
}

// Handle event from the Connection Parameters Module.
// NOTE: This could have been done by simply setting the disconnect_on_fail
//       config parameter, but instead we use the event handler
//       mechanism to demonstrate its use.
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
	if(p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED && 0 != g_uartInstance) {
		g_uartInstance->evtConnectionFailed();
	}
}

// Handle errors from the Connection Parameters module.
static void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error); // Just call the Softdevice error handler.
}

// BLE event handler.
static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
{
	if(0 != g_uartInstance) {
		g_uartInstance->evtBle(p_ble_evt);
	}
}


// ***************************************************

// Set up BLE and the UART service.
BLEUart::BLEUart()
	: _delegate(0)
	, _bleNusMaxDataLen(BLE_GATT_ATT_MTU_DEFAULT - 3)
	, _connHandle(BLE_CONN_HANDLE_INVALID)
{
    // Pointer so handlers can have some context.
    g_uartInstance = this;

    // Set up everything.
	bleStackInit();
	gapParamsInit();
    gattInit();
    servicesInit();
    advertisingInit();
    connParamsInit();

    // Start advertising our service.
    advertisingStart();
}

// Return the most number of bytes which can be sent or received in one packet.
unsigned BLEUart::getMaxPayload() const
{
	return _bleNusMaxDataLen;
}

// Initialize BLE
void BLEUart::bleStackInit()
{
    ret_code_t err_code;

    // Configure the BLE stack using the default settings.
    // Fetch the start address of the application RAM.
    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err_code);

    // Enable BLE stack.
    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Register a handler for BLE events.
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
}

// Set up the Connection Parameters module.
void BLEUart::connParamsInit()
{
    uint32_t               err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = on_conn_params_evt;
    cp_init.error_handler                  = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for the GAP initialization.
 *
 * @details This function will set up all the necessary GAP (Generic Access Profile) parameters of
 *          the device. It also sets the permissions and appearance.
 */
void BLEUart::gapParamsInit()
{
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *) DEVICE_NAME,
                                          strlen(DEVICE_NAME));
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}

// Initialize the GATT library.
void BLEUart::gattInit()
{
    ret_code_t err_code;

    err_code = nrf_ble_gatt_init(&m_gatt, gatt_evt_handler);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_ble_gatt_att_mtu_periph_set(&m_gatt, NRF_SDH_BLE_GATT_MAX_MTU_SIZE);
    APP_ERROR_CHECK(err_code);
}

// Initialize services that will be used by the application.
void BLEUart::servicesInit()
{
    uint32_t           err_code;
    ble_nus_init_t     nus_init;
    nrf_ble_qwr_init_t qwr_init = {0};

    // Initialize Queued Write Module.
    qwr_init.error_handler = nrf_qwr_error_handler;

    err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init);
    APP_ERROR_CHECK(err_code);

    // Initialize NUS.
    memset(&nus_init, 0, sizeof(nus_init));

    nus_init.data_handler = nus_data_handler;

    err_code = ble_nus_init(&m_nus, &nus_init);
    APP_ERROR_CHECK(err_code);
}

// Initialize the Advertising functionality.
void BLEUart::advertisingInit()
{
    uint32_t               err_code;
    ble_advertising_init_t init;

    memset(&init, 0, sizeof(init));

    init.advdata.name_type          = BLE_ADVDATA_FULL_NAME;
    init.advdata.include_appearance = false;
    init.advdata.flags              = BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE;

    init.srdata.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    init.srdata.uuids_complete.p_uuids  = m_adv_uuids;

    init.config.ble_adv_fast_enabled  = true;
    init.config.ble_adv_fast_interval = APP_ADV_INTERVAL;
    init.config.ble_adv_fast_timeout  = APP_ADV_DURATION;

    err_code = ble_advertising_init(&m_advertising, &init);
    APP_ERROR_CHECK(err_code);

    ble_advertising_conn_cfg_tag_set(&m_advertising, APP_BLE_CONN_CFG_TAG);
}

// Start advertising our service.
void BLEUart::advertisingStart()
{
    uint32_t err_code = ble_advertising_start(&m_advertising, BLE_ADV_MODE_FAST);
    APP_ERROR_CHECK(err_code);
}

// Send data to the Android app.
void BLEUart::send(const uint8_t *data, uint16_t length)
{
	uint32_t err_code;
	do
	{
		err_code = ble_nus_data_send(&m_nus, (uint8_t *)data, &length, _connHandle);
		if ((err_code != NRF_ERROR_INVALID_STATE) &&
			(err_code != NRF_ERROR_RESOURCES) &&
			(err_code != NRF_ERROR_NOT_FOUND))
		{
			APP_ERROR_CHECK(err_code);
		}
	} while (err_code == NRF_ERROR_RESOURCES);
}

// Receive data from the Android app.
void BLEUart::rxDataHandler(const uint8_t *data, unsigned length)
{
	if(_delegate != 0) {
		_delegate->rxData(data, length);
	}
}

void BLEUart::evtConnectionFailed()
{
	uint32_t err_code = sd_ble_gap_disconnect(_connHandle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
    APP_ERROR_CHECK(err_code);

	if(0 != _delegate) {
		_delegate->connectionStateChanged(false);
	}
}

// GATT event handler.
void BLEUart::evtGatt(nrf_ble_gatt_t * p_gatt, nrf_ble_gatt_evt_t const * p_evt)
{
    if ((_connHandle == p_evt->conn_handle) && (p_evt->evt_id == NRF_BLE_GATT_EVT_ATT_MTU_UPDATED) && 0 != g_uartInstance) {
    	_bleNusMaxDataLen = p_evt->params.att_mtu_effective - OPCODE_LENGTH - HANDLE_LENGTH;
    }
}

void BLEUart::evtBle(ble_evt_t const *p_ble_evt)
{
	uint32_t err_code;

	switch (p_ble_evt->header.evt_id)
	{
		case BLE_GAP_EVT_CONNECTED:
			if(0 != _delegate) {
				_delegate->connectionStateChanged(true);
			}
			_connHandle = p_ble_evt->evt.gap_evt.conn_handle;
			err_code = nrf_ble_qwr_conn_handle_assign(&m_qwr, _connHandle);
			APP_ERROR_CHECK(err_code);
			break;

		case BLE_GAP_EVT_DISCONNECTED:
			if(0 != _delegate) {
				_delegate->connectionStateChanged(false);
			}
			_connHandle = BLE_CONN_HANDLE_INVALID;
			break;

		case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
		{
			ble_gap_phys_t const phys =
			{
				.tx_phys = BLE_GAP_PHY_AUTO,
				.rx_phys = BLE_GAP_PHY_AUTO,
			};
			err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
			APP_ERROR_CHECK(err_code);
		} break;

		case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
			// Pairing not supported
			err_code = sd_ble_gap_sec_params_reply(_connHandle, BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP, NULL, NULL);
			APP_ERROR_CHECK(err_code);
			break;

		case BLE_GATTS_EVT_SYS_ATTR_MISSING:
			// No system attributes have been stored.
			err_code = sd_ble_gatts_sys_attr_set(_connHandle, NULL, 0, 0);
			APP_ERROR_CHECK(err_code);
			break;

		case BLE_GATTC_EVT_TIMEOUT:
			if(0 != _delegate) {
				_delegate->connectionStateChanged(false);
			}
			// Disconnect on GATT Client timeout event.
			err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
											 BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
			APP_ERROR_CHECK(err_code);
			break;

		case BLE_GATTS_EVT_TIMEOUT:
			if(0 != _delegate) {
				_delegate->connectionStateChanged(false);
			}
			// Disconnect on GATT Server timeout event.
			err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
											 BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
			APP_ERROR_CHECK(err_code);
			break;

		default:
			break;
	}
}
