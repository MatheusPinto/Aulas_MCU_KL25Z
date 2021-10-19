################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/diff_robot/diff_robot.c \
../source/diff_robot/gyro.c \
../source/diff_robot/line_sensor.c \
../source/diff_robot/motor_ctrl.c \
../source/diff_robot/ultrasonic.c 

OBJS += \
./source/diff_robot/diff_robot.o \
./source/diff_robot/gyro.o \
./source/diff_robot/line_sensor.o \
./source/diff_robot/motor_ctrl.o \
./source/diff_robot/ultrasonic.o 

C_DEPS += \
./source/diff_robot/diff_robot.d \
./source/diff_robot/gyro.d \
./source/diff_robot/line_sensor.d \
./source/diff_robot/motor_ctrl.d \
./source/diff_robot/ultrasonic.d 


# Each subdirectory must supply rules for building sources it contributes
source/diff_robot/%.o: ../source/diff_robot/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I../board -I../source -I../ -I../freertos -I../drivers -I../CMSIS -I../utilities -I../startup -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


