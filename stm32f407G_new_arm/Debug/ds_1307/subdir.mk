################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ds_1307/ds_1307.c \
../ds_1307/lcd_16x2.c 

OBJS += \
./ds_1307/ds_1307.o \
./ds_1307/lcd_16x2.o 

C_DEPS += \
./ds_1307/ds_1307.d \
./ds_1307/lcd_16x2.d 


# Each subdirectory must supply rules for building sources it contributes
ds_1307/%.o ds_1307/%.su ds_1307/%.cyclo: ../ds_1307/%.c ds_1307/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F407xx -c -I../Core/Inc -I"/home/dzimitori2012/STM32CubeIDE/workspace_1.18.1/new_f407G/ds_1307" -I../Drivers/CMSIS -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-ds_1307

clean-ds_1307:
	-$(RM) ./ds_1307/ds_1307.cyclo ./ds_1307/ds_1307.d ./ds_1307/ds_1307.o ./ds_1307/ds_1307.su ./ds_1307/lcd_16x2.cyclo ./ds_1307/lcd_16x2.d ./ds_1307/lcd_16x2.o ./ds_1307/lcd_16x2.su

.PHONY: clean-ds_1307

