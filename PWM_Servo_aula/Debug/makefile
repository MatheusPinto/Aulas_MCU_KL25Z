################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utilities/subdir.mk
-include startup/subdir.mk
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
all: PWM_Servo_aula.axf

# Tool invocations
PWM_Servo_aula.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -print-memory-usage -Xlinker --gc-sections -Xlinker -Map="PWM_Servo_aula.map" -mcpu=cortex-m0plus -mthumb -T "PWM_Servo_aula_Debug.ld" -o "PWM_Servo_aula.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) PWM_Servo_aula.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "PWM_Servo_aula.axf"; # arm-none-eabi-objcopy -v -O binary "PWM_Servo_aula.axf" "PWM_Servo_aula.bin" ; # checksum -p MKL25Z128xxx4 -d "PWM_Servo_aula.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
