################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../R2U2_C/src/AT/at_checkers.c \
../R2U2_C/src/AT/at_compare.c \
../R2U2_C/src/AT/at_globals.c \
../R2U2_C/src/AT/at_operations.c 

OBJS += \
./R2U2_C/src/AT/at_checkers.o \
./R2U2_C/src/AT/at_compare.o \
./R2U2_C/src/AT/at_globals.o \
./R2U2_C/src/AT/at_operations.o 

C_DEPS += \
./R2U2_C/src/AT/at_checkers.d \
./R2U2_C/src/AT/at_compare.d \
./R2U2_C/src/AT/at_globals.d \
./R2U2_C/src/AT/at_operations.d 


# Each subdirectory must supply rules for building sources it contributes
R2U2_C/src/AT/%.o: ../R2U2_C/src/AT/%.c R2U2_C/src/AT/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' -DDEBUG_ENABLE '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F427xx -c -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src/drivers -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Drivers/CMSIS/Include -I../Drivers/Sensors -I../Middlewares -I../Middlewares/Inc -I../Appl -I/usr/local/include -I../R2U2_C/src -I../R2U2_C/src/AT -I../R2U2_C/src/AT/extra_filters -I../R2U2_C/src/AT/fft_filter -I../R2U2_C/src/AT/prognostic_filter -I../R2U2_C/src/binParser -I../R2U2_c/src/TL -O0 -ffunction-sections -Wall -u _printf_float -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

