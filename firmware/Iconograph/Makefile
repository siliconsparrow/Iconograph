# Makefile for the Iconograph firmware on the nRF52832
#
# Created 7-May-2021 by Adam Pierce <adam@siliconsparrow.com>
#

PROJECT_NAME     := Iconograph
TARGETS          := $(PROJECT_NAME)
OUTPUT_DIRECTORY := Release

SDK_ROOT      := ../nRF_SDK_16.0.0
PROJ_DIR      := .
NRF_STACK     := $(SDK_ROOT)/components/softdevice/s112/hex/s112_nrf52_7.0.1_softdevice.hex
HEXMERGE      := $(PROJ_DIR)/mergeHex.py
TEMPLATE_PATH := $(SDK_ROOT)/components/toolchain/gcc

$(OUTPUT_DIRECTORY)/$(PROJECT_NAME).out: \
  LINKER_SCRIPT  := nRF52832_xxaa.ld

# Source files common to all targets
SRC_FILES += \
  $(SDK_ROOT)/modules/nrfx/mdk/gcc_startup_nrf52.S \
  $(SDK_ROOT)/modules/nrfx/mdk/system_nrf52.c \
  $(SDK_ROOT)/components/libraries/pwr_mgmt/nrf_pwr_mgmt.c \
  $(SDK_ROOT)/components/libraries/timer/app_timer2.c \
  $(SDK_ROOT)/components/libraries/timer/drv_rtc.c \
  $(SDK_ROOT)/components/libraries/util/app_error.c \
  $(SDK_ROOT)/components/libraries/util/app_util_platform.c \
  $(SDK_ROOT)/components/libraries/sortlist/nrf_sortlist.c \
  $(SDK_ROOT)/components/libraries/atomic/nrf_atomic.c \
  $(SDK_ROOT)/components/libraries/atomic_fifo/nrf_atfifo.c \
  $(SDK_ROOT)/components/libraries/atomic_flags/nrf_atflags.c \
  $(SDK_ROOT)/components/libraries/balloc/nrf_balloc.c \
  $(SDK_ROOT)/components/libraries/memobj/nrf_memobj.c \
  $(SDK_ROOT)/components/libraries/strerror/nrf_strerror.c \
  $(SDK_ROOT)/components/libraries/experimental_section_vars/nrf_section_iter.c \
  $(SDK_ROOT)/components/libraries/log/src/nrf_log_frontend.c \
  $(SDK_ROOT)/components/softdevice/common/nrf_sdh.c \
  $(SDK_ROOT)/components/softdevice/common/nrf_sdh_ble.c \
  $(SDK_ROOT)/components/ble/ble_services/ble_nus/ble_nus.c \
  $(SDK_ROOT)/components/ble/ble_link_ctx_manager/ble_link_ctx_manager.c \
  $(SDK_ROOT)/components/ble/common/ble_srv_common.c \
  $(SDK_ROOT)/components/ble/nrf_ble_qwr/nrf_ble_qwr.c \
  $(SDK_ROOT)/components/ble/nrf_ble_gatt/nrf_ble_gatt.c \
  $(SDK_ROOT)/components/ble/common/ble_conn_state.c \
  $(SDK_ROOT)/components/ble/ble_advertising/ble_advertising.c \
  $(SDK_ROOT)/components/ble/common/ble_advdata.c \
  $(SDK_ROOT)/components/ble/common/ble_conn_params.c \
  $(PROJ_DIR)/tinycpp.cpp \
  $(PROJ_DIR)/Motor.cpp \
  $(PROJ_DIR)/Timer.cpp \
  $(PROJ_DIR)/BLEUart.cpp \
  $(PROJ_DIR)/Iconograph.cpp \
  $(PROJ_DIR)/main.cpp

# Include folders common to all targets
INC_FOLDERS += \
  $(PROJ_DIR) \
  $(SDK_ROOT)/components/toolchain/cmsis/include \
  $(SDK_ROOT)/components/libraries/util \
  $(SDK_ROOT)/components/libraries/timer \
  $(SDK_ROOT)/components/libraries/log \
  $(SDK_ROOT)/components/libraries/log/src \
  $(SDK_ROOT)/components/libraries/sortlist \
  $(SDK_ROOT)/components/libraries/pwr_mgmt \
  $(SDK_ROOT)/components/libraries/mutex \
  $(SDK_ROOT)/components/libraries/atomic \
  $(SDK_ROOT)/components/libraries/atomic_fifo \
  $(SDK_ROOT)/components/libraries/atomic_flags \
  $(SDK_ROOT)/components/libraries/memobj \
  $(SDK_ROOT)/components/libraries/balloc \
  $(SDK_ROOT)/components/libraries/strerror \
  $(SDK_ROOT)/components/libraries/delay \
  $(SDK_ROOT)/components/libraries/ringbuf \
  $(SDK_ROOT)/components/libraries/experimental_section_vars \
  $(SDK_ROOT)/components/ble/common \
  $(SDK_ROOT)/components/ble/ble_advertising \
  $(SDK_ROOT)/components/ble/nrf_ble_qwr \
  $(SDK_ROOT)/components/ble/nrf_ble_gatt \
  $(SDK_ROOT)/components/ble/ble_link_ctx_manager \
  $(SDK_ROOT)/components/ble/ble_services/ble_nus \
  $(SDK_ROOT)/components/softdevice/common \
  $(SDK_ROOT)/components/softdevice/s112/headers \
  $(SDK_ROOT)/components/softdevice/s112/headers/nrf52 \
  $(SDK_ROOT)/integration/nrfx \
  $(SDK_ROOT)/modules/nrfx \
  $(SDK_ROOT)/modules/nrfx/mdk \
  $(SDK_ROOT)/modules/nrfx/hal \
  $(SDK_ROOT)/external/fprintf \

# Libraries common to all targets
LIB_FILES += -lm
#-lc -lnosys

# Optimization flags
OPT = -O0 -g3
# Uncomment the line below to enable link time optimization
#OPT += -flto

# C flags common to all targets
CFLAGS += $(OPT)
CFLAGS += -DAPP_TIMER_V2
CFLAGS += -DAPP_TIMER_V2_RTC1_ENABLED
CFLAGS += -DBOARD_PCA10040
CFLAGS += -DCONFIG_GPIO_AS_PINRESET
#CFLAGS += -DCONFIG_NFCT_PINS_AS_GPIOS
CFLAGS += -DFLOAT_ABI_HARD
CFLAGS += -DNRF52
CFLAGS += -DNRF52832_XXAA
CFLAGS += -DNRF52_PAN_74
CFLAGS += -DNRF_SD_BLE_API_VERSION=7
CFLAGS += -DS112
CFLAGS += -DSOFTDEVICE_PRESENT
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs
CFLAGS += -Wall -Werror
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in a separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin -fshort-enums

# C++ flags common to all targets
CXXFLAGS += $(OPT)
CXXFLAGS += -std=c++11 -std=gnu++11
CXXFLAGS += -fno-rtti
CXXFLAGS += -fno-exceptions
CXXFLAGS += -fno-threadsafe-statics

# Assembler flags common to all targets
ASMFLAGS += -g3
ASMFLAGS += -mcpu=cortex-m4
ASMFLAGS += -mthumb -mabi=aapcs
ASMFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
ASMFLAGS += -DAPP_TIMER_V2
ASMFLAGS += -DAPP_TIMER_V2_RTC1_ENABLED
ASMFLAGS += -DBOARD_PCA10040
ASMFLAGS += -DCONFIG_GPIO_AS_PINRESET
ASMFLAGS += -DFLOAT_ABI_HARD
ASMFLAGS += -DNRF52
ASMFLAGS += -DNRF52832_XXAA
ASMFLAGS += -DNRF52_PAN_74
ASMFLAGS += -DNRF_SD_BLE_API_VERSION=7
ASMFLAGS += -DS112
ASMFLAGS += -DSOFTDEVICE_PRESENT

# Linker flags
LDFLAGS += $(OPT)
LDFLAGS += -mthumb -mabi=aapcs -L$(SDK_ROOT)/modules/nrfx/mdk -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# let linker dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs
# Suppress 32-bit character warnings.
#LDFLAGS +=  -Wl,--no-wchar-size-warning

$(PROJECT_NAME): CFLAGS += -D__HEAP_SIZE=2048
$(PROJECT_NAME): CFLAGS += -D__STACK_SIZE=2048
$(PROJECT_NAME): ASMFLAGS += -D__HEAP_SIZE=2048
$(PROJECT_NAME): ASMFLAGS += -D__STACK_SIZE=2048

include $(TEMPLATE_PATH)/Makefile.common

$(foreach target, $(TARGETS), $(call define_target, $(target)))

# Merge the application and stack to produce a complete image.
release: $(OUTPUT_DIRECTORY)/$(PROJECT_NAME).hex
	$(HEXMERGE) $(OUTPUT_DIRECTORY)/$(PROJECT_NAME).hex $(NRF_STACK) > $(OUTPUT_DIRECTORY)/$(PROJECT_NAME)_final.hex
