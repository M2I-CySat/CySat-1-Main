################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/ADCS.c \
../Src/BatteryCapacity.c \
../Src/EPS.c \
../Src/GroundStation.c \
../Src/UHF.c \
../Src/adcs_commissioning_steps.c \
../Src/disco_helper_functions.c \
../Src/gpio.c \
../Src/helper_functions.c \
../Src/i2c.c \
../Src/main.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_it.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32f4xx.c \
../Src/usart.c 

OBJS += \
./Src/ADCS.o \
./Src/BatteryCapacity.o \
./Src/EPS.o \
./Src/GroundStation.o \
./Src/UHF.o \
./Src/adcs_commissioning_steps.o \
./Src/disco_helper_functions.o \
./Src/gpio.o \
./Src/helper_functions.o \
./Src/i2c.o \
./Src/main.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_it.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32f4xx.o \
./Src/usart.o 

C_DEPS += \
./Src/ADCS.d \
./Src/BatteryCapacity.d \
./Src/EPS.d \
./Src/GroundStation.d \
./Src/UHF.d \
./Src/adcs_commissioning_steps.d \
./Src/disco_helper_functions.d \
./Src/gpio.d \
./Src/helper_functions.d \
./Src/i2c.d \
./Src/main.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_it.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32f4xx.d \
./Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F429xx -c -I../Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/ADCS.d ./Src/ADCS.o ./Src/ADCS.su ./Src/BatteryCapacity.d ./Src/BatteryCapacity.o ./Src/BatteryCapacity.su ./Src/EPS.d ./Src/EPS.o ./Src/EPS.su ./Src/GroundStation.d ./Src/GroundStation.o ./Src/GroundStation.su ./Src/UHF.d ./Src/UHF.o ./Src/UHF.su ./Src/adcs_commissioning_steps.d ./Src/adcs_commissioning_steps.o ./Src/adcs_commissioning_steps.su ./Src/disco_helper_functions.d ./Src/disco_helper_functions.o ./Src/disco_helper_functions.su ./Src/gpio.d ./Src/gpio.o ./Src/gpio.su ./Src/helper_functions.d ./Src/helper_functions.o ./Src/helper_functions.su ./Src/i2c.d ./Src/i2c.o ./Src/i2c.su ./Src/main.d ./Src/main.o ./Src/main.su ./Src/stm32f4xx_hal_msp.d ./Src/stm32f4xx_hal_msp.o ./Src/stm32f4xx_hal_msp.su ./Src/stm32f4xx_it.d ./Src/stm32f4xx_it.o ./Src/stm32f4xx_it.su ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/system_stm32f4xx.d ./Src/system_stm32f4xx.o ./Src/system_stm32f4xx.su ./Src/usart.d ./Src/usart.o ./Src/usart.su

.PHONY: clean-Src

