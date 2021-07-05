package com.example.iconograph

import android.bluetooth.*
import android.bluetooth.le.ScanCallback
import android.bluetooth.le.ScanFilter
import android.bluetooth.le.ScanResult
import android.bluetooth.le.ScanSettings
import android.content.Context
import android.util.Log
import java.util.*


class BleComm(m: MainActivity) {

    enum class BleState(val str: String) {
        IDLE("IDLE"),
        SCANNING("SCANNING"),
        CONNECTING("CONNECTING"),
        CONNECTED("CONNECTED")
    }

    // Values
    private val cccd            = UUID.fromString("00002902-0000-1000-8000-00805f9b34fb")
    private val uartServiceUuid = UUID.fromString("6e400001-b5a3-f393-e0a9-e50e24dcca9e")
    private val rxCharUuid      = UUID.fromString("6e400002-b5a3-f393-e0a9-e50e24dcca9e")
    private val rxServiceUuid   = UUID.fromString("6e400001-b5a3-f393-e0a9-e50e24dcca9e")
    private val txCharUuid      = UUID.fromString("6e400003-b5a3-f393-e0a9-e50e24dcca9e")

    private val mainAct = m
    private val logTag = "BLE"
    private val bleDeviceName = "Nordic_UART"

    // Create filter so we will only detect our device and nothing else.
    private val filters = listOf(ScanFilter.Builder().setDeviceName(bleDeviceName).build())
    private val bluetoothAdapter: BluetoothAdapter by lazy {
        val bluetoothManager = mainAct.getSystemService(Context.BLUETOOTH_SERVICE) as BluetoothManager
        bluetoothManager.adapter
    }
    // BLE scanner
    private val bleScanner by lazy {
        bluetoothAdapter.bluetoothLeScanner
    }
    private val scanSettings = ScanSettings.Builder()
        .setScanMode(ScanSettings.SCAN_MODE_LOW_LATENCY)
        .build()
    private val scanCallback = object : ScanCallback() {
        override fun onScanResult(callbackType: Int, result: ScanResult) {
            with(result.device) {
                scanStop()
                log("Found BLE device. Name: ${name ?: "Unnamed"}, address: $address")
                bleConnect(result.device)
            }
        }
    }

    // Variables
    private var state = BleState.IDLE
    private var connectedGatt: BluetoothGatt? = null

    private fun log(msg:String) {
        Log.w(logTag, msg)
    }

    private fun setState(s: BleState) {
        state = s
        log("State: ${s.str}")
        mainAct.setStatusText(s.str)
    }

    // Probe whether system bluetooth is turned on.
    fun isBluetoothEnabled(): Boolean {
        return bluetoothAdapter.isEnabled
    }

    fun pause() {
        // TODO: Maybe disconnect BLE connection here?
    }

    fun resume() {
        if(state == BleState.IDLE) {
            scanStart()
        }
    }

    fun destroy() {
    }

    // Start scanning for the bluetooth device.
    private fun scanStart() {
        bleScanner.startScan(filters, scanSettings, scanCallback)
        setState(BleState.SCANNING)
    }

    // Stop scanning for the bluetooth device.
    private fun scanStop() {
        bleScanner.stopScan(scanCallback)
        setState(BleState.IDLE)
    }

    // Called if the BLE connection fails or if the remote device disconnects.
    private fun connectionFail() {
        setState(BleState.IDLE)

        // Wait a second or so then restart scanning
        Thread.sleep(1000)
        scanStart()
    }

    // Log all the discovered services.
    private fun printGattTable(services: List<BluetoothGattService>) {
        if(services.isEmpty()) {
            log("No services available")
            return
        }
        services.forEach { service->
            val characteristicsTable = service.characteristics.joinToString(
                separator = "\n|--",
                prefix = "|--"
            ) { it.uuid.toString() }
            log("\nService ${service.uuid}\nCharacteristics:\n$characteristicsTable")
        }
    }

    private val gattCallback = object : BluetoothGattCallback() {
        override fun onConnectionStateChange(gatt: BluetoothGatt, status: Int, newState: Int) {
            val deviceAddress = gatt.device.address
            if(status == BluetoothGatt.GATT_SUCCESS) {
                if(newState == BluetoothProfile.STATE_CONNECTED) {
                    log("Connected to $deviceAddress")
                    log("Discovering services...")
                    gatt.discoverServices()
                    connectedGatt = gatt
                } else {
                    log("Disconnected from $deviceAddress")
                    connectedGatt = null
                    gatt.close()
                    connectionFail()
                }
            } else {
                log("Error $status with connection to $deviceAddress")
                log("Disconnected from $deviceAddress")
                connectedGatt = null
                gatt.close()
                connectionFail()
            }
        }

        override fun onServicesDiscovered(gatt: BluetoothGatt, status: Int) {
            log("Discovered ${gatt.services.size} services on ${gatt.device.address}")
            printGattTable(gatt.services)
            enableTxNotification()
            setState(BleState.CONNECTED)
        }

        override fun onCharacteristicChanged(gatt: BluetoothGatt, characteristic: BluetoothGattCharacteristic) {
            log("Characteristic Change Notification ${characteristic.toString()}")
            log("CHValue: ${characteristic.getStringValue(0)}")
            mainAct.recvd(characteristic.getStringValue(0))

            //gatt.readCharacteristic(characteristic)
        }

//        override fun onCharacteristicRead(gatt: BluetoothGatt, characteristic: BluetoothGattCharacteristic, status: Int) {
//            log("CHRCALLBACK")
//            if (status == BluetoothGatt.GATT_SUCCESS) {
//                val rx = characteristic.value.toString()
//                log("Received $rx")
//            }
//        }
    }

    private fun bleConnect(device: BluetoothDevice)  {
        log("Connecting to ${device.address}")
        setState(BleState.CONNECTING)
        device.connectGatt(mainAct, false, gattCallback)
    }

    fun enableTxNotification(): Boolean {
        val rxService: BluetoothGattService? = connectedGatt?.getService(rxServiceUuid)
        if(rxService == null) {
            log("Rx service not found!")
            return false
        }
        val txChar = rxService.getCharacteristic(txCharUuid)
        if (txChar == null) {
            log("Tx characteristic not found!")
            return false
        }
        connectedGatt?.setCharacteristicNotification(txChar, true)

        val descriptor = txChar.getDescriptor(cccd)
        descriptor.value = BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE
        connectedGatt?.writeDescriptor(descriptor)
        log("TX Notification enabled")
        return true
    }

    fun send(data: ByteArray): Boolean {
        if(state != BleState.CONNECTED) {
            return false
        }

        val rxService = connectedGatt?.getService(uartServiceUuid) ?: return false
        val rxChar = rxService.getCharacteristic(rxCharUuid) ?: return false

        rxChar.value = data

        val result = connectedGatt?.writeCharacteristic(rxChar)

        if(result == true) {
            log("Sent ${data.size} bytes")
        } else {
            log("Send ${data.size} bytes FAILED")
        }
        return result == true
    }

}