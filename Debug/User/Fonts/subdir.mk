################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/Fonts/font12.c \
../User/Fonts/font12CN.c \
../User/Fonts/font16.c \
../User/Fonts/font20.c \
../User/Fonts/font24.c \
../User/Fonts/font24CN.c \
../User/Fonts/font8.c 

OBJS += \
./User/Fonts/font12.o \
./User/Fonts/font12CN.o \
./User/Fonts/font16.o \
./User/Fonts/font20.o \
./User/Fonts/font24.o \
./User/Fonts/font24CN.o \
./User/Fonts/font8.o 

C_DEPS += \
./User/Fonts/font12.d \
./User/Fonts/font12CN.d \
./User/Fonts/font16.d \
./User/Fonts/font20.d \
./User/Fonts/font24.d \
./User/Fonts/font24CN.d \
./User/Fonts/font8.d 


# Each subdirectory must supply rules for building sources it contributes
User/Fonts/%.o: ../User/Fonts/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32L4 -DSTM32L476RGTx -DNUCLEO_L476RG -DDEBUG -DSTM32L476xx -DUSE_HAL_DRIVER -I"C:/Users/MullerLee/workspace/hal/HAL_Driver/Inc/Legacy" -I"C:/Users/MullerLee/workspace/hal/inc" -I"C:/Users/MullerLee/workspace/hal/CMSIS/device" -I"C:/Users/MullerLee/workspace/hal/CMSIS/core" -I"C:/Users/MullerLee/workspace/hal/HAL_Driver/Inc" -I"C:/Users/MullerLee/workspace/hal/Utilities/STM32L4xx_Nucleo" -I"C:/Users/MullerLee/workspace/hal/User/Examples" -I"C:/Users/MullerLee/workspace/hal/User/Config" -I"C:/Users/MullerLee/workspace/hal/User/GUI" -I"C:/Users/MullerLee/workspace/hal/User/Fonts" -I"C:/Users/MullerLee/workspace/hal/User/epaper" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


