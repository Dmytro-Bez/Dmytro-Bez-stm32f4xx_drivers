################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/i2c_tx.c \
../Core/Src/stm32f4xx_gpio_driver.c \
../Core/Src/stm32f4xx_i2c_driver.c \
../Core/Src/stm32f4xx_spi_driver.c \
../Core/Src/syscalls.c \
../Core/Src/system_stm32f4xx.c 

OBJS += \
./Core/Src/i2c_tx.o \
./Core/Src/stm32f4xx_gpio_driver.o \
./Core/Src/stm32f4xx_i2c_driver.o \
./Core/Src/stm32f4xx_spi_driver.o \
./Core/Src/syscalls.o \
./Core/Src/system_stm32f4xx.o 

C_DEPS += \
./Core/Src/i2c_tx.d \
./Core/Src/stm32f4xx_gpio_driver.d \
./Core/Src/stm32f4xx_i2c_driver.d \
./Core/Src/stm32f4xx_spi_driver.d \
./Core/Src/syscalls.d \
./Core/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F407xx -c -I../Core/Inc -I../Drivers/CMSIS -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/i2c_tx.cyclo ./Core/Src/i2c_tx.d ./Core/Src/i2c_tx.o ./Core/Src/i2c_tx.su ./Core/Src/stm32f4xx_gpio_driver.cyclo ./Core/Src/stm32f4xx_gpio_driver.d ./Core/Src/stm32f4xx_gpio_driver.o ./Core/Src/stm32f4xx_gpio_driver.su ./Core/Src/stm32f4xx_i2c_driver.cyclo ./Core/Src/stm32f4xx_i2c_driver.d ./Core/Src/stm32f4xx_i2c_driver.o ./Core/Src/stm32f4xx_i2c_driver.su ./Core/Src/stm32f4xx_spi_driver.cyclo ./Core/Src/stm32f4xx_spi_driver.d ./Core/Src/stm32f4xx_spi_driver.o ./Core/Src/stm32f4xx_spi_driver.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su

.PHONY: clean-Core-2f-Src

