################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../R2U2_C/src/AT/prognostic_filter/BHM_emxAPI.c \
../R2U2_C/src/AT/prognostic_filter/BHM_emxutil.c \
../R2U2_C/src/AT/prognostic_filter/BHM_initialize.c \
../R2U2_C/src/AT/prognostic_filter/BHM_rtwutil.c \
../R2U2_C/src/AT/prognostic_filter/BHM_terminate.c \
../R2U2_C/src/AT/prognostic_filter/CellConstantInputPredict.c \
../R2U2_C/src/AT/prognostic_filter/CellConstantInputPredictLookup.c \
../R2U2_C/src/AT/prognostic_filter/OutputEqnUKF.c \
../R2U2_C/src/AT/prognostic_filter/StateEqnCurrent.c \
../R2U2_C/src/AT/prognostic_filter/StateEqnPower.c \
../R2U2_C/src/AT/prognostic_filter/StateEqnResistance.c \
../R2U2_C/src/AT/prognostic_filter/StateEqnUKF.c \
../R2U2_C/src/AT/prognostic_filter/ThresholdEqnCurrent.c \
../R2U2_C/src/AT/prognostic_filter/ThresholdEqnPower.c \
../R2U2_C/src/AT/prognostic_filter/ThresholdEqnResistance.c \
../R2U2_C/src/AT/prognostic_filter/UKFInitialize.c \
../R2U2_C/src/AT/prognostic_filter/UKFStep.c \
../R2U2_C/src/AT/prognostic_filter/asinh.c \
../R2U2_C/src/AT/prognostic_filter/colon.c \
../R2U2_C/src/AT/prognostic_filter/computeSOCEstimate.c \
../R2U2_C/src/AT/prognostic_filter/computeSigmaPoints.c \
../R2U2_C/src/AT/prognostic_filter/fileManager.c \
../R2U2_C/src/AT/prognostic_filter/filter_prognostics.c \
../R2U2_C/src/AT/prognostic_filter/fprintf.c \
../R2U2_C/src/AT/prognostic_filter/getInitialBatteryState.c \
../R2U2_C/src/AT/prognostic_filter/log.c \
../R2U2_C/src/AT/prognostic_filter/power.c \
../R2U2_C/src/AT/prognostic_filter/rdivide.c \
../R2U2_C/src/AT/prognostic_filter/repmat.c \
../R2U2_C/src/AT/prognostic_filter/rtGetInf.c \
../R2U2_C/src/AT/prognostic_filter/rtGetNaN.c \
../R2U2_C/src/AT/prognostic_filter/rt_nonfinite.c \
../R2U2_C/src/AT/prognostic_filter/wcov.c \
../R2U2_C/src/AT/prognostic_filter/wmean.c 

OBJS += \
./R2U2_C/src/AT/prognostic_filter/BHM_emxAPI.o \
./R2U2_C/src/AT/prognostic_filter/BHM_emxutil.o \
./R2U2_C/src/AT/prognostic_filter/BHM_initialize.o \
./R2U2_C/src/AT/prognostic_filter/BHM_rtwutil.o \
./R2U2_C/src/AT/prognostic_filter/BHM_terminate.o \
./R2U2_C/src/AT/prognostic_filter/CellConstantInputPredict.o \
./R2U2_C/src/AT/prognostic_filter/CellConstantInputPredictLookup.o \
./R2U2_C/src/AT/prognostic_filter/OutputEqnUKF.o \
./R2U2_C/src/AT/prognostic_filter/StateEqnCurrent.o \
./R2U2_C/src/AT/prognostic_filter/StateEqnPower.o \
./R2U2_C/src/AT/prognostic_filter/StateEqnResistance.o \
./R2U2_C/src/AT/prognostic_filter/StateEqnUKF.o \
./R2U2_C/src/AT/prognostic_filter/ThresholdEqnCurrent.o \
./R2U2_C/src/AT/prognostic_filter/ThresholdEqnPower.o \
./R2U2_C/src/AT/prognostic_filter/ThresholdEqnResistance.o \
./R2U2_C/src/AT/prognostic_filter/UKFInitialize.o \
./R2U2_C/src/AT/prognostic_filter/UKFStep.o \
./R2U2_C/src/AT/prognostic_filter/asinh.o \
./R2U2_C/src/AT/prognostic_filter/colon.o \
./R2U2_C/src/AT/prognostic_filter/computeSOCEstimate.o \
./R2U2_C/src/AT/prognostic_filter/computeSigmaPoints.o \
./R2U2_C/src/AT/prognostic_filter/fileManager.o \
./R2U2_C/src/AT/prognostic_filter/filter_prognostics.o \
./R2U2_C/src/AT/prognostic_filter/fprintf.o \
./R2U2_C/src/AT/prognostic_filter/getInitialBatteryState.o \
./R2U2_C/src/AT/prognostic_filter/log.o \
./R2U2_C/src/AT/prognostic_filter/power.o \
./R2U2_C/src/AT/prognostic_filter/rdivide.o \
./R2U2_C/src/AT/prognostic_filter/repmat.o \
./R2U2_C/src/AT/prognostic_filter/rtGetInf.o \
./R2U2_C/src/AT/prognostic_filter/rtGetNaN.o \
./R2U2_C/src/AT/prognostic_filter/rt_nonfinite.o \
./R2U2_C/src/AT/prognostic_filter/wcov.o \
./R2U2_C/src/AT/prognostic_filter/wmean.o 

C_DEPS += \
./R2U2_C/src/AT/prognostic_filter/BHM_emxAPI.d \
./R2U2_C/src/AT/prognostic_filter/BHM_emxutil.d \
./R2U2_C/src/AT/prognostic_filter/BHM_initialize.d \
./R2U2_C/src/AT/prognostic_filter/BHM_rtwutil.d \
./R2U2_C/src/AT/prognostic_filter/BHM_terminate.d \
./R2U2_C/src/AT/prognostic_filter/CellConstantInputPredict.d \
./R2U2_C/src/AT/prognostic_filter/CellConstantInputPredictLookup.d \
./R2U2_C/src/AT/prognostic_filter/OutputEqnUKF.d \
./R2U2_C/src/AT/prognostic_filter/StateEqnCurrent.d \
./R2U2_C/src/AT/prognostic_filter/StateEqnPower.d \
./R2U2_C/src/AT/prognostic_filter/StateEqnResistance.d \
./R2U2_C/src/AT/prognostic_filter/StateEqnUKF.d \
./R2U2_C/src/AT/prognostic_filter/ThresholdEqnCurrent.d \
./R2U2_C/src/AT/prognostic_filter/ThresholdEqnPower.d \
./R2U2_C/src/AT/prognostic_filter/ThresholdEqnResistance.d \
./R2U2_C/src/AT/prognostic_filter/UKFInitialize.d \
./R2U2_C/src/AT/prognostic_filter/UKFStep.d \
./R2U2_C/src/AT/prognostic_filter/asinh.d \
./R2U2_C/src/AT/prognostic_filter/colon.d \
./R2U2_C/src/AT/prognostic_filter/computeSOCEstimate.d \
./R2U2_C/src/AT/prognostic_filter/computeSigmaPoints.d \
./R2U2_C/src/AT/prognostic_filter/fileManager.d \
./R2U2_C/src/AT/prognostic_filter/filter_prognostics.d \
./R2U2_C/src/AT/prognostic_filter/fprintf.d \
./R2U2_C/src/AT/prognostic_filter/getInitialBatteryState.d \
./R2U2_C/src/AT/prognostic_filter/log.d \
./R2U2_C/src/AT/prognostic_filter/power.d \
./R2U2_C/src/AT/prognostic_filter/rdivide.d \
./R2U2_C/src/AT/prognostic_filter/repmat.d \
./R2U2_C/src/AT/prognostic_filter/rtGetInf.d \
./R2U2_C/src/AT/prognostic_filter/rtGetNaN.d \
./R2U2_C/src/AT/prognostic_filter/rt_nonfinite.d \
./R2U2_C/src/AT/prognostic_filter/wcov.d \
./R2U2_C/src/AT/prognostic_filter/wmean.d 


# Each subdirectory must supply rules for building sources it contributes
R2U2_C/src/AT/prognostic_filter/%.o: ../R2U2_C/src/AT/prognostic_filter/%.c R2U2_C/src/AT/prognostic_filter/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' -DDEBUG_ENABLE '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F427xx -c -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src/drivers -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Drivers/CMSIS/Include -I../Drivers/Sensors -I../Middlewares -I../Middlewares/Inc -I../Appl -I/usr/local/include -I../R2U2_C/src -I../R2U2_C/src/AT -I../R2U2_C/src/AT/extra_filters -I../R2U2_C/src/AT/fft_filter -I../R2U2_C/src/AT/prognostic_filter -I../R2U2_C/src/binParser -I../R2U2_c/src/TL -O0 -ffunction-sections -Wall -u _printf_float -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

