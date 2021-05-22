################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utilities/subdir.mk
-include startup/subdir.mk
-include source/emb_util/subdir.mk
-include source/display/subdir.mk
-include source/delayer/subdir.mk
-include source/subdir.mk
-include drivers/subdir.mk
-include board/subdir.mk
-include CMSIS/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: lcd_keypad_kl25z.axf

# Tool invocations
lcd_keypad_kl25z.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -Map="lcd_keypad_kl25z.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m0plus -mthumb -T "TPM_PWM_FreeRTOS_Debug.ld" -o "lcd_keypad_kl25z.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) lcd_keypad_kl25z.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "lcd_keypad_kl25z.axf"; # arm-none-eabi-objcopy -v -O binary "lcd_keypad_kl25z.axf" "lcd_keypad_kl25z.bin" ; # checksum -p MKL25Z128xxx4 -d "lcd_keypad_kl25z.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
