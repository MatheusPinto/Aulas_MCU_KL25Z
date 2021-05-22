################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/delayer/delayer.c 

OBJS += \
./source/delayer/delayer.o 

C_DEPS += \
./source/delayer/delayer.d 


# Each subdirectory must supply rules for building sources it contributes
source/delayer/%.o: ../source/delayer/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DPRINTF_FLOAT_ENABLE=0 -D__USE_CMSIS -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE=1 -D__MCUXPRESSO -DDEBUG -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -I"D:\Documentos\MCUXpressoIDE_10.1.1_606\workspace\TPM_PWM_aula\board" -I"D:\Documentos\MCUXpressoIDE_10.1.1_606\workspace\TPM_PWM_aula\source" -I"D:\Documentos\MCUXpressoIDE_10.1.1_606\workspace\TPM_PWM_aula" -I"D:\Documentos\MCUXpressoIDE_10.1.1_606\workspace\TPM_PWM_aula\drivers" -I"D:\Documentos\MCUXpressoIDE_10.1.1_606\workspace\TPM_PWM_aula\utilities" -I"D:\Documentos\MCUXpressoIDE_10.1.1_606\workspace\TPM_PWM_aula\startup" -I"D:\Documentos\MCUXpressoIDE_10.1.1_606\workspace\TPM_PWM_aula\CMSIS" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


