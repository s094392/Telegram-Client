################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/epaper/EPD_1in02d.c \
../User/epaper/EPD_1in54.c \
../User/epaper/EPD_1in54_V2.c \
../User/epaper/EPD_1in54b.c \
../User/epaper/EPD_1in54c.c \
../User/epaper/EPD_2in13.c \
../User/epaper/EPD_2in13_V2.c \
../User/epaper/EPD_2in13bc.c \
../User/epaper/EPD_2in13d.c \
../User/epaper/EPD_2in7.c \
../User/epaper/EPD_2in7b.c \
../User/epaper/EPD_2in9.c \
../User/epaper/EPD_2in9bc.c \
../User/epaper/EPD_2in9d.c \
../User/epaper/EPD_4in2.c \
../User/epaper/EPD_4in2bc.c \
../User/epaper/EPD_5in83.c \
../User/epaper/EPD_5in83bc.c \
../User/epaper/EPD_7in5.c \
../User/epaper/EPD_7in5_V2.c \
../User/epaper/EPD_7in5b_V2.c \
../User/epaper/EPD_7in5bc.c 

OBJS += \
./User/epaper/EPD_1in02d.o \
./User/epaper/EPD_1in54.o \
./User/epaper/EPD_1in54_V2.o \
./User/epaper/EPD_1in54b.o \
./User/epaper/EPD_1in54c.o \
./User/epaper/EPD_2in13.o \
./User/epaper/EPD_2in13_V2.o \
./User/epaper/EPD_2in13bc.o \
./User/epaper/EPD_2in13d.o \
./User/epaper/EPD_2in7.o \
./User/epaper/EPD_2in7b.o \
./User/epaper/EPD_2in9.o \
./User/epaper/EPD_2in9bc.o \
./User/epaper/EPD_2in9d.o \
./User/epaper/EPD_4in2.o \
./User/epaper/EPD_4in2bc.o \
./User/epaper/EPD_5in83.o \
./User/epaper/EPD_5in83bc.o \
./User/epaper/EPD_7in5.o \
./User/epaper/EPD_7in5_V2.o \
./User/epaper/EPD_7in5b_V2.o \
./User/epaper/EPD_7in5bc.o 

C_DEPS += \
./User/epaper/EPD_1in02d.d \
./User/epaper/EPD_1in54.d \
./User/epaper/EPD_1in54_V2.d \
./User/epaper/EPD_1in54b.d \
./User/epaper/EPD_1in54c.d \
./User/epaper/EPD_2in13.d \
./User/epaper/EPD_2in13_V2.d \
./User/epaper/EPD_2in13bc.d \
./User/epaper/EPD_2in13d.d \
./User/epaper/EPD_2in7.d \
./User/epaper/EPD_2in7b.d \
./User/epaper/EPD_2in9.d \
./User/epaper/EPD_2in9bc.d \
./User/epaper/EPD_2in9d.d \
./User/epaper/EPD_4in2.d \
./User/epaper/EPD_4in2bc.d \
./User/epaper/EPD_5in83.d \
./User/epaper/EPD_5in83bc.d \
./User/epaper/EPD_7in5.d \
./User/epaper/EPD_7in5_V2.d \
./User/epaper/EPD_7in5b_V2.d \
./User/epaper/EPD_7in5bc.d 


# Each subdirectory must supply rules for building sources it contributes
User/epaper/%.o: ../User/epaper/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32L4 -DSTM32L476RGTx -DNUCLEO_L476RG -DDEBUG -DSTM32L476xx -DUSE_HAL_DRIVER -I"C:/Users/MullerLee/workspace/hal/HAL_Driver/Inc/Legacy" -I"C:/Users/MullerLee/workspace/hal/inc" -I"C:/Users/MullerLee/workspace/hal/CMSIS/device" -I"C:/Users/MullerLee/workspace/hal/CMSIS/core" -I"C:/Users/MullerLee/workspace/hal/HAL_Driver/Inc" -I"C:/Users/MullerLee/workspace/hal/Utilities/STM32L4xx_Nucleo" -I"C:/Users/MullerLee/workspace/hal/User/Examples" -I"C:/Users/MullerLee/workspace/hal/User/Config" -I"C:/Users/MullerLee/workspace/hal/User/GUI" -I"C:/Users/MullerLee/workspace/hal/User/Fonts" -I"C:/Users/MullerLee/workspace/hal/User/epaper" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


