
TARGET = firmware

DEBUG = 1
# optimization
OPT = -Og
# printf float support: set PRINTF_FLOAT=1 when needed
PRINTF_FLOAT = 0


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
src/main.c \
src/APP_Layer/App_Layer.c \
src/Mid_Layer/Mid_Layer.c \
src/IP_Layer/Ip_Line_Sensor.c \
src/IP_Layer/Ip_Delay.c \
src/IP_Layer/Ip_Usart.c \
src/IP_Layer/Ip_Hcsr04.c \
src/IP_Layer/Ip_Motor.c \
src/IP_Layer/Ip_Line_PID.c \
src/IP_Layer/Ip_Logic.c \
src/IP_Layer/Ip_Ring_Buffer.c \
src/IP_Layer/Ip_Ir_Remote.c \
lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_spi.c \
lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_usart.c \
lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_rtc.c \
lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_exti.c \
lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_fsmc.c \
lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_bkp.c \
lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_dma.c \
lib/STM32F10x_StdPeriph_Driver/src/misc.c \
lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_sdio.c \
lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_i2c.c \
lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_wwdg.c \
lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c \
lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_crc.c \
lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c \
lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_flash.c \
lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_tim.c \
lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_dbgmcu.c \
lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_pwr.c \
lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_iwdg.c \
lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_can.c \
lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_cec.c \
lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_adc.c \
lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_dac.c \
lib/CMSIS/CM3/CoreSupport/core_cm3.c \
lib/CMSIS/CM3/DeviceSupport/ST/STM32F10x/system_stm32f10x.c 

# ASM sources
ASM_SOURCES =  \
startup_stm32f103.s


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# Leave empty to use toolchain from PATH.
# Can be set from command line: make GCC_PATH=C:/arm_gcc/bin
# Or exported as environment variable ARM_GCC_PATH.
GCC_PATH ?= $(ARM_GCC_PATH)
GCC_PATH := $(subst \\,/,$(strip $(GCC_PATH)))
GCC_PATH := $(patsubst %/,%,$(GCC_PATH))
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifneq ($(GCC_PATH),)
CC = "$(GCC_PATH)/$(PREFIX)gcc"
AS = "$(GCC_PATH)/$(PREFIX)gcc" -x assembler-with-cpp
CP = "$(GCC_PATH)/$(PREFIX)objcopy"
SZ = "$(GCC_PATH)/$(PREFIX)size"
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m3

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi


# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DSTM32F10X_MD \
-DUSE_STDPERIPH_DRIVER


# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-Iinc \
-Iinc/APP_Layer \
-Iinc/IP_Layer \
-Iinc/Mid_Layer \
-Ilib/CMSIS/CM3/CoreSupport \
-Ilib/CMSIS/CM3/DeviceSupport/ST/STM32F10x \
-Ilib/STM32F10x_StdPeriph_Driver/inc


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = stm32f103_flash.ld

# libraries
LIBS = -lc -lm
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -specs=nosys.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

ifeq ($(PRINTF_FLOAT),1)
LDFLAGS += -u _printf_float
endif

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir -p $@

#######################################
# st-flash added by Bui Van Hai
#######################################		
flash: $(BUILD_DIR)/$(TARGET).bin
	st-flash write $< 0x08000000
#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# erase
#######################################
erase:
	st-flash erase
reset:
	st-flash reset

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***