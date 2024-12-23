################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/7SEG.c \
../Core/Src/CLCD.c \
../Core/Src/alarm.c \
../Core/Src/buzzer.c \
../Core/Src/clock.c \
../Core/Src/main.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/stopWatch.c \
../Core/Src/swControll.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/uartLEDControll.c 

OBJS += \
./Core/Src/7SEG.o \
./Core/Src/CLCD.o \
./Core/Src/alarm.o \
./Core/Src/buzzer.o \
./Core/Src/clock.o \
./Core/Src/main.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/stopWatch.o \
./Core/Src/swControll.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/uartLEDControll.o 

C_DEPS += \
./Core/Src/7SEG.d \
./Core/Src/CLCD.d \
./Core/Src/alarm.d \
./Core/Src/buzzer.d \
./Core/Src/clock.d \
./Core/Src/main.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/stopWatch.d \
./Core/Src/swControll.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/uartLEDControll.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/7SEG.cyclo ./Core/Src/7SEG.d ./Core/Src/7SEG.o ./Core/Src/7SEG.su ./Core/Src/CLCD.cyclo ./Core/Src/CLCD.d ./Core/Src/CLCD.o ./Core/Src/CLCD.su ./Core/Src/alarm.cyclo ./Core/Src/alarm.d ./Core/Src/alarm.o ./Core/Src/alarm.su ./Core/Src/buzzer.cyclo ./Core/Src/buzzer.d ./Core/Src/buzzer.o ./Core/Src/buzzer.su ./Core/Src/clock.cyclo ./Core/Src/clock.d ./Core/Src/clock.o ./Core/Src/clock.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/stopWatch.cyclo ./Core/Src/stopWatch.d ./Core/Src/stopWatch.o ./Core/Src/stopWatch.su ./Core/Src/swControll.cyclo ./Core/Src/swControll.d ./Core/Src/swControll.o ./Core/Src/swControll.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/uartLEDControll.cyclo ./Core/Src/uartLEDControll.d ./Core/Src/uartLEDControll.o ./Core/Src/uartLEDControll.su

.PHONY: clean-Core-2f-Src

