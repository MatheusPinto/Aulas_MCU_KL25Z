################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utilities/subdir.mk
-include startup/subdir.mk
-include source/libraries/emb_util/subdir.mk
-include source/libraries/delay/subdir.mk
-include source/generic_drivers/lcd/subdir.mk
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
all: LCD_keypad_aula.axf

# Tool invocations
LCD_keypad_aula.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -Map="LCD_keypad_aula.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m0plus -mthumb -T "LCD_keypad_aula_Debug.ld" -o "LCD_keypad_aula.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) LCD_keypad_aula.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "LCD_keypad_aula.axf"; # arm-none-eabi-objcopy -v -O binary "LCD_keypad_aula.axf" "LCD_keypad_aula.bin" ; # checksum -p MKL25Z128xxx4 -d "LCD_keypad_aula.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
