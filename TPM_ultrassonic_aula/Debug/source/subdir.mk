################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/TPM_ultrassonic_aula.c \
../source/mtb.c \
../source/semihost_hardfault.c 

OBJS += \
./source/TPM_ultrassonic_aula.o \
./source/mtb.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/TPM_ultrassonic_aula.d \
./source/mtb.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\mathe\OneDrive\Documentos\Workspaces\MCUXpresso\Aulas_MCU\TPM_ultrassonic_aula\board" -I"C:\Users\mathe\OneDrive\Documentos\Workspaces\MCUXpresso\Aulas_MCU\TPM_ultrassonic_aula\source" -I"C:\Users\mathe\OneDrive\Documentos\Workspaces\MCUXpresso\Aulas_MCU\TPM_ultrassonic_aula" -I"C:\Users\mathe\OneDrive\Documentos\Workspaces\MCUXpresso\Aulas_MCU\TPM_ultrassonic_aula\startup" -I"C:\Users\mathe\OneDrive\Documentos\Workspaces\MCUXpresso\Aulas_MCU\TPM_ultrassonic_aula\CMSIS" -I"C:\Users\mathe\OneDrive\Documentos\Workspaces\MCUXpresso\Aulas_MCU\TPM_ultrassonic_aula\drivers" -I"C:\Users\mathe\OneDrive\Documentos\Workspaces\MCUXpresso\Aulas_MCU\TPM_ultrassonic_aula\utilities" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


