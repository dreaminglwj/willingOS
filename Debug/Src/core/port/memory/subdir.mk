################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/core/port/memory/memory.c 

OBJS += \
./Src/core/port/memory/memory.o 

C_DEPS += \
./Src/core/port/memory/memory.d 


# Each subdirectory must supply rules for building sources it contributes
Src/core/port/memory/%.o Src/core/port/memory/%.su Src/core/port/memory/%.cyclo: ../Src/core/port/memory/%.c Src/core/port/memory/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103ZETx -c -I../Inc -I"/Users/luwenjin/work/willing/willingOS-202401291529/Src/Libraries/CMSIS" -I"/Users/luwenjin/work/willing/willingOS-202401291529/Src/app/led" -I"/Users/luwenjin/work/willing/willingOS-202401291529/Src/core/port/cpus/ARM_Cotex_M3" -I"/Users/luwenjin/work/willing/willingOS-202401291529/Src/Libraries/STM32F10x_StdPeriph_Driver/inc" -I"/Users/luwenjin/work/willing/willingOS-202401291529/Src/user" -I"/Users/luwenjin/work/willing/willingOS-202401291529/Src/core/sys" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src-2f-core-2f-port-2f-memory

clean-Src-2f-core-2f-port-2f-memory:
	-$(RM) ./Src/core/port/memory/memory.cyclo ./Src/core/port/memory/memory.d ./Src/core/port/memory/memory.o ./Src/core/port/memory/memory.su

.PHONY: clean-Src-2f-core-2f-port-2f-memory

