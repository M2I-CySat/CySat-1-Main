################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/BatteryCapacity.c \
../Src/CySatPacketProtocol.c \
../Src/UHF.c \
../Src/adcs_functions.c \
../Src/crc.c \
../Src/dma2d.c \
../Src/fmc.c \
../Src/gpio.c \
../Src/helper_functions.c \
../Src/i2c.c \
../Src/ltdc.c \
../Src/main.c \
../Src/spi.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_hal_timebase_tim.c \
../Src/stm32f4xx_it.c \
../Src/syscalls.c \
../Src/system_stm32f4xx.c \
../Src/tim.c \
../Src/usart.c \
../Src/usb_host.c \
../Src/usbh_conf.c \
../Src/usbh_platform.c 

OBJS += \
./Src/BatteryCapacity.o \
./Src/CySatPacketProtocol.o \
./Src/UHF.o \
./Src/adcs_functions.o \
./Src/crc.o \
./Src/dma2d.o \
./Src/fmc.o \
./Src/gpio.o \
./Src/helper_functions.o \
./Src/i2c.o \
./Src/ltdc.o \
./Src/main.o \
./Src/spi.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_hal_timebase_tim.o \
./Src/stm32f4xx_it.o \
./Src/syscalls.o \
./Src/system_stm32f4xx.o \
./Src/tim.o \
./Src/usart.o \
./Src/usb_host.o \
./Src/usbh_conf.o \
./Src/usbh_platform.o 

C_DEPS += \
./Src/BatteryCapacity.d \
./Src/CySatPacketProtocol.d \
./Src/UHF.d \
./Src/adcs_functions.d \
./Src/crc.d \
./Src/dma2d.d \
./Src/fmc.d \
./Src/gpio.d \
./Src/helper_functions.d \
./Src/i2c.d \
./Src/ltdc.d \
./Src/main.d \
./Src/spi.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_hal_timebase_tim.d \
./Src/stm32f4xx_it.d \
./Src/syscalls.d \
./Src/system_stm32f4xx.d \
./Src/tim.d \
./Src/usart.d \
./Src/usb_host.d \
./Src/usbh_conf.d \
./Src/usbh_platform.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F429xx -I"C:/Users/Bryan Friestad/Desktop/Senior Design/cysat-ece-senior-design/CubeSAT/dev/OBC/discoboard_no_rtos/Inc" -I"C:/Users/Bryan Friestad/Desktop/Senior Design/cysat-ece-senior-design/CubeSAT/dev/OBC/discoboard_no_rtos/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Bryan Friestad/Desktop/Senior Design/cysat-ece-senior-design/CubeSAT/dev/OBC/discoboard_no_rtos/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Bryan Friestad/Desktop/Senior Design/cysat-ece-senior-design/CubeSAT/dev/OBC/discoboard_no_rtos/Middlewares/ST/STM32_USB_Host_Library/Core/Inc" -I"C:/Users/Bryan Friestad/Desktop/Senior Design/cysat-ece-senior-design/CubeSAT/dev/OBC/discoboard_no_rtos/Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc" -I"C:/Users/Bryan Friestad/Desktop/Senior Design/cysat-ece-senior-design/CubeSAT/dev/OBC/discoboard_no_rtos/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Bryan Friestad/Desktop/Senior Design/cysat-ece-senior-design/CubeSAT/dev/OBC/discoboard_no_rtos/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


