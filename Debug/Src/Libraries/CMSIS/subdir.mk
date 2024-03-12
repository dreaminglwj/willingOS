################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/Libraries/CMSIS/core_cm3.c \
../Src/Libraries/CMSIS/system_stm32f10x.c 

OBJS += \
./Src/Libraries/CMSIS/core_cm3.o \
./Src/Libraries/CMSIS/system_stm32f10x.o 

C_DEPS += \
./Src/Libraries/CMSIS/core_cm3.d \
./Src/Libraries/CMSIS/system_stm32f10x.d 


# Each subdirectory must supply rules for building sources it contributes
Src/Libraries/CMSIS/%.o Src/Libraries/CMSIS/%.su Src/Libraries/CMSIS/%.cyclo: ../Src/Libraries/CMSIS/%.c Src/Libraries/CMSIS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103ZETx -c -I../Inc -I"/Users/luwenjin/work/willing/willingOS-202401291529/Src/Libraries/CMSIS" -I"/Users/luwenjin/work/willing/willingOS-202401291529/Src/app/led" -I"/Users/luwenjin/work/willing/willingOS-202401291529/Src/core/port/cpus/ARM_Cotex_M3" -I"/Users/luwenjin/work/willing/willingOS-202401291529/Src/Libraries/STM32F10x_StdPeriph_Driver/inc" -I"/Users/luwenjin/work/willing/willingOS-202401291529/Src/user" -I"/Users/luwenjin/work/willing/willingOS-202401291529/Src/core/sys" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src-2f-Libraries-2f-CMSIS

clean-Src-2f-Libraries-2f-CMSIS:
	-$(RM) ./Src/Libraries/CMSIS/core_cm3.cyclo ./Src/Libraries/CMSIS/core_cm3.d ./Src/Libraries/CMSIS/core_cm3.o ./Src/Libraries/CMSIS/core_cm3.su ./Src/Libraries/CMSIS/system_stm32f10x.cyclo ./Src/Libraries/CMSIS/system_stm32f10x.d ./Src/Libraries/CMSIS/system_stm32f10x.o ./Src/Libraries/CMSIS/system_stm32f10x.su

.PHONY: clean-Src-2f-Libraries-2f-CMSIS

