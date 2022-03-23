################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
discoboard_new.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
discoboard_new.list \

OBJCOPY_BIN += \
discoboard_new.bin \


# All Target
all: discoboard_new.elf secondary-outputs

# Tool invocations
discoboard_new.elf: $(OBJS) $(USER_OBJS) /Users/aaurandt/Desktop/IowaState/CPR\ E\ 491/cysat-ece-senior-design/CubeSAT/dev/OBC/discoboard_new/STM32F429ZITX_FLASH.ld
	arm-none-eabi-gcc -o "discoboard_new.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/Users/aaurandt/Desktop/IowaState/CPR E 491/cysat-ece-senior-design/CubeSAT/dev/OBC/discoboard_new/STM32F429ZITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="discoboard_new.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

discoboard_new.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "discoboard_new.list"
	@echo 'Finished building: $@'
	@echo ' '

discoboard_new.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "discoboard_new.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
