################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/system_MKL25Z4.c 

OBJS += \
./CMSIS/system_MKL25Z4.o 

C_DEPS += \
./CMSIS/system_MKL25Z4.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/%.o: ../CMSIS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DPRINTF_FLOAT_ENABLE=0 -D__USE_CMSIS -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE=1 -D__MCUXPRESSO -DDEBUG -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -I"C:\Users\mathe\Documentos\Workspaces\MCUXpresso\Aulas_MCU\PWM_Servo_aula_v2\board" -I"C:\Users\mathe\Documentos\Workspaces\MCUXpresso\Aulas_MCU\PWM_Servo_aula_v2\source" -I"C:\Users\mathe\Documentos\Workspaces\MCUXpresso\Aulas_MCU\PWM_Servo_aula_v2" -I"C:\Users\mathe\Documentos\Workspaces\MCUXpresso\Aulas_MCU\PWM_Servo_aula_v2\drivers" -I"C:\Users\mathe\Documentos\Workspaces\MCUXpresso\Aulas_MCU\PWM_Servo_aula_v2\utilities" -I"C:\Users\mathe\Documentos\Workspaces\MCUXpresso\Aulas_MCU\PWM_Servo_aula_v2\startup" -I"C:\Users\mathe\Documentos\Workspaces\MCUXpresso\Aulas_MCU\PWM_Servo_aula_v2\CMSIS" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


