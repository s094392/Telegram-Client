################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/Examples/EPD_1in02_test.c \
../User/Examples/EPD_1in54_V2_test.c \
../User/Examples/EPD_1in54_test.c \
../User/Examples/EPD_1in54b_test.c \
../User/Examples/EPD_1in54c_test.c \
../User/Examples/EPD_2in13_V2_test.c \
../User/Examples/EPD_2in13_test.c \
../User/Examples/EPD_2in13bc_test.c \
../User/Examples/EPD_2in13d_test.c \
../User/Examples/EPD_2in7_test.c \
../User/Examples/EPD_2in7b_test.c \
../User/Examples/EPD_2in9_test.c \
../User/Examples/EPD_2in9bc_test.c \
../User/Examples/EPD_2in9d_test.c \
../User/Examples/EPD_4in2_test.c \
../User/Examples/EPD_4in2bc_test.c \
../User/Examples/EPD_5in83_test.c \
../User/Examples/EPD_5in83bc_test.c \
../User/Examples/EPD_7in5_V2_test.c \
../User/Examples/EPD_7in5_test.c \
../User/Examples/EPD_7in5b_V2_test.c \
../User/Examples/EPD_7in5bc_test.c \
../User/Examples/ImageData.c 

OBJS += \
./User/Examples/EPD_1in02_test.o \
./User/Examples/EPD_1in54_V2_test.o \
./User/Examples/EPD_1in54_test.o \
./User/Examples/EPD_1in54b_test.o \
./User/Examples/EPD_1in54c_test.o \
./User/Examples/EPD_2in13_V2_test.o \
./User/Examples/EPD_2in13_test.o \
./User/Examples/EPD_2in13bc_test.o \
./User/Examples/EPD_2in13d_test.o \
./User/Examples/EPD_2in7_test.o \
./User/Examples/EPD_2in7b_test.o \
./User/Examples/EPD_2in9_test.o \
./User/Examples/EPD_2in9bc_test.o \
./User/Examples/EPD_2in9d_test.o \
./User/Examples/EPD_4in2_test.o \
./User/Examples/EPD_4in2bc_test.o \
./User/Examples/EPD_5in83_test.o \
./User/Examples/EPD_5in83bc_test.o \
./User/Examples/EPD_7in5_V2_test.o \
./User/Examples/EPD_7in5_test.o \
./User/Examples/EPD_7in5b_V2_test.o \
./User/Examples/EPD_7in5bc_test.o \
./User/Examples/ImageData.o 

C_DEPS += \
./User/Examples/EPD_1in02_test.d \
./User/Examples/EPD_1in54_V2_test.d \
./User/Examples/EPD_1in54_test.d \
./User/Examples/EPD_1in54b_test.d \
./User/Examples/EPD_1in54c_test.d \
./User/Examples/EPD_2in13_V2_test.d \
./User/Examples/EPD_2in13_test.d \
./User/Examples/EPD_2in13bc_test.d \
./User/Examples/EPD_2in13d_test.d \
./User/Examples/EPD_2in7_test.d \
./User/Examples/EPD_2in7b_test.d \
./User/Examples/EPD_2in9_test.d \
./User/Examples/EPD_2in9bc_test.d \
./User/Examples/EPD_2in9d_test.d \
./User/Examples/EPD_4in2_test.d \
./User/Examples/EPD_4in2bc_test.d \
./User/Examples/EPD_5in83_test.d \
./User/Examples/EPD_5in83bc_test.d \
./User/Examples/EPD_7in5_V2_test.d \
./User/Examples/EPD_7in5_test.d \
./User/Examples/EPD_7in5b_V2_test.d \
./User/Examples/EPD_7in5bc_test.d \
./User/Examples/ImageData.d 


# Each subdirectory must supply rules for building sources it contributes
User/Examples/%.o: ../User/Examples/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32L4 -DSTM32L476RGTx -DNUCLEO_L476RG -DDEBUG -DSTM32L476xx -DUSE_HAL_DRIVER -I"C:/Users/MullerLee/workspace/hal/HAL_Driver/Inc/Legacy" -I"C:/Users/MullerLee/workspace/hal/inc" -I"C:/Users/MullerLee/workspace/hal/CMSIS/device" -I"C:/Users/MullerLee/workspace/hal/CMSIS/core" -I"C:/Users/MullerLee/workspace/hal/HAL_Driver/Inc" -I"C:/Users/MullerLee/workspace/hal/Utilities/STM32L4xx_Nucleo" -I"C:/Users/MullerLee/workspace/hal/User/Examples" -I"C:/Users/MullerLee/workspace/hal/User/Config" -I"C:/Users/MullerLee/workspace/hal/User/GUI" -I"C:/Users/MullerLee/workspace/hal/User/Fonts" -I"C:/Users/MullerLee/workspace/hal/User/epaper" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


