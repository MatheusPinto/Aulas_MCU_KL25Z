################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_flash.c \
../drivers/fsl_gpio.c \
../drivers/fsl_lpsci.c \
../drivers/fsl_smc.c \
../drivers/fsl_tpm.c \
../drivers/fsl_uart.c 

OBJS += \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_flash.o \
./drivers/fsl_gpio.o \
./drivers/fsl_lpsci.o \
./drivers/fsl_smc.o \
./drivers/fsl_tpm.o \
./drivers/fsl_uart.o 

C_DEPS += \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_flash.d \
./drivers/fsl_gpio.d \
./drivers/fsl_lpsci.d \
./drivers/fsl_smc.d \
./drivers/fsl_tpm.d \
./drivers/fsl_uart.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DPRINTF_FLOAT_ENABLE=0 -D__USE_CMSIS -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE=1 -D__MCUXPRESSO -DDEBUG -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -I"C:\Users\mathe\Documentos\Workspaces\MCUXpresso\Aulas_MCU\PWM_Servo_aula_v2\board" -I"C:\Users\mathe\Documentos\Workspaces\MCUXpresso\Aulas_MCU\PWM_Servo_aula_v2\source" -I"C:\Users\mathe\Documentos\Workspaces\MCUXpresso\Aulas_MCU\PWM_Servo_aula_v2" -I"C:\Users\mathe\Documentos\Workspaces\MCUXpresso\Aulas_MCU\PWM_Servo_aula_v2\drivers" -I"C:\Users\mathe\Documentos\Workspaces\MCUXpresso\Aulas_MCU\PWM_Servo_aula_v2\utilities" -I"C:\Users\mathe\Documentos\Workspaces\MCUXpresso\Aulas_MCU\PWM_Servo_aula_v2\startup" -I"C:\Users\mathe\Documentos\Workspaces\MCUXpresso\Aulas_MCU\PWM_Servo_aula_v2\CMSIS" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


