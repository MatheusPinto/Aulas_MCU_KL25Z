################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/GPIO_aula.c \
../source/mtb.c \
../source/semihost_hardfault.c 

OBJS += \
./source/GPIO_aula.o \
./source/mtb.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/GPIO_aula.d \
./source/mtb.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DPRINTF_FLOAT_ENABLE=0 -D__USE_CMSIS -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -DDEBUG -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -I"C:\Users\mathe\Documentos\Workspaces\MCUXpresso\Aulas_MCU\GPIO_aula\board" -I"C:\Users\mathe\Documentos\Workspaces\MCUXpresso\Aulas_MCU\GPIO_aula\source" -I"C:\Users\mathe\Documentos\Workspaces\MCUXpresso\Aulas_MCU\GPIO_aula" -I"C:\Users\mathe\Documentos\Workspaces\MCUXpresso\Aulas_MCU\GPIO_aula\drivers" -I"C:\Users\mathe\Documentos\Workspaces\MCUXpresso\Aulas_MCU\GPIO_aula\utilities" -I"C:\Users\mathe\Documentos\Workspaces\MCUXpresso\Aulas_MCU\GPIO_aula\startup" -I"C:\Users\mathe\Documentos\Workspaces\MCUXpresso\Aulas_MCU\GPIO_aula\CMSIS" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


