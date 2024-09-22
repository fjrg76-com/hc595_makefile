ARDUINO_DIR = $(HOME)/arduino-1.8.19
ARDMK_DIR = $(ARDUINO_DIR)/Arduino-Makefile
ARDUINO_CORE_LIBRARIES_PATH = $(HOME)/.arduino15/packages/arduino/hardware/sam/1.6.12/libraries

PLATFORM = Arduino

BOARD_TAG = arduino_due_x
MONITOR_PORT = /dev/ttyACM*
MONITOR_BAUDRATE = 115200

C_OPTIMIZATION_LEVEL = -Og
CXX_OPTIMIZATION_LEVEL = -Og

WARNING_FLAGS = -Wall -Wextra
BUILD_EXTRA_FLAGS = -I. 
BUILD_EXTRA_FLAGS += -DARDUINO_SAM_DUE -DARDUINO_ARCH_SAM


CXX_INC = -I. \
			 -I.. \


CXXFLAGS_STD = -c -g $(CXX_OPTIMIZATION_LEVEL) -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD
CXXFLAGS_STD += ${BUILD_EXTRA_FLAGS} ${WARNING_FLAGS}

CXXFLAGS_STD += $(CXX_INC)

LOCAL_CPP_SRCS = main.cpp \
					  Display_HC595.cpp \


CFLAGS_STD = -c -g $(C_OPTIMIZATION_LEVEL) -std=gnu11 -ffunction-sections -fdata-sections -MMD -fno-fat-lto-objects
CFLAGS_STD += ${BUILD_EXTRA_FLAGS} ${WARNING_FLAGS} 

CORE_PATH = $(HOME)/.arduino15/packages/arduino
ALTERNATE_CORE_PATH = $(CORE_PATH)/hardware/sam/1.6.12
CMSIS_DIR = $(CORE_PATH)/hardware/sam/1.6.12/system/CMSIS/CMSIS
CMSIS_ATMEL_DIR = $(CORE_PATH)/hardware/sam/1.6.12/system/CMSIS
include $(ARDMK_DIR)/Sam.mk
