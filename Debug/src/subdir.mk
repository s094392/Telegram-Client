################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/gpio.c \
../src/main.c \
../src/spi.c \
../src/stm32l4xx_hal_msp.c \
../src/stm32l4xx_it.c \
../src/syscalls.c \
../src/system_stm32l4xx.c \
../src/usart.c 

OBJS += \
./src/gpio.o \
./src/main.o \
./src/spi.o \
./src/stm32l4xx_hal_msp.o \
./src/stm32l4xx_it.o \
./src/syscalls.o \
./src/system_stm32l4xx.o \
./src/usart.o 

C_DEPS += \
./src/gpio.d \
./src/main.d \
./src/spi.d \
./src/stm32l4xx_hal_msp.d \
./src/stm32l4xx_it.d \
./src/syscalls.d \
./src/system_stm32l4xx.d \
./src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32L4 -DSTM32L476RGTx -DNUCLEO_L476RG -DDEBUG -DSTM32L476xx -DUSE_HAL_DRIVER -I"C:/Users/MullerLee/workspace/hal/HAL_Driver/Inc/Legacy" -I"C:/Users/MullerLee/workspace/hal/inc" -I"C:/Users/MullerLee/workspace/hal/CMSIS/device" -I"C:/Users/MullerLee/workspace/hal/CMSIS/core" -I"C:/Users/MullerLee/workspace/hal/HAL_Driver/Inc" -I"C:/Users/MullerLee/workspace/hal/Utilities/STM32L4xx_Nucleo" -I"C:/Users/MullerLee/workspace/hal/User/Examples" -I"C:/Users/MullerLee/workspace/hal/User/Config" -I"C:/Users/MullerLee/workspace/hal/User/GUI" -I"C:/Users/MullerLee/workspace/hal/User/Fonts" -I"C:/Users/MullerLee/workspace/hal/User/epaper" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


