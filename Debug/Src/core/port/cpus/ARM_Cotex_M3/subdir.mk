################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Src/core/port/cpus/ARM_Cotex_M3/cpu.s 

C_SRCS += \
../Src/core/port/cpus/ARM_Cotex_M3/port.c 

OBJS += \
./Src/core/port/cpus/ARM_Cotex_M3/cpu.o \
./Src/core/port/cpus/ARM_Cotex_M3/port.o 

S_DEPS += \
./Src/core/port/cpus/ARM_Cotex_M3/cpu.d 

C_DEPS += \
./Src/core/port/cpus/ARM_Cotex_M3/port.d 


# Each subdirectory must supply rules for building sources it contributes
Src/core/port/cpus/ARM_Cotex_M3/%.o: ../Src/core/port/cpus/ARM_Cotex_M3/%.s Src/core/port/cpus/ARM_Cotex_M3/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m3 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"
Src/core/port/cpus/ARM_Cotex_M3/%.o Src/core/port/cpus/ARM_Cotex_M3/%.su Src/core/port/cpus/ARM_Cotex_M3/%.cyclo: ../Src/core/port/cpus/ARM_Cotex_M3/%.c Src/core/port/cpus/ARM_Cotex_M3/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103ZETx -c -I../Inc -I"/Users/luwenjin/work/willing/willingOS-202401291529/Src/Libraries/CMSIS" -I"/Users/luwenjin/work/willing/willingOS-202401291529/Src/app/led" -I"/Users/luwenjin/work/willing/willingOS-202401291529/Src/core/port/cpus/ARM_Cotex_M3" -I"/Users/luwenjin/work/willing/willingOS-202401291529/Src/Libraries/STM32F10x_StdPeriph_Driver/inc" -I"/Users/luwenjin/work/willing/willingOS-202401291529/Src/user" -I"/Users/luwenjin/work/willing/willingOS-202401291529/Src/core/sys" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src-2f-core-2f-port-2f-cpus-2f-ARM_Cotex_M3

clean-Src-2f-core-2f-port-2f-cpus-2f-ARM_Cotex_M3:
	-$(RM) ./Src/core/port/cpus/ARM_Cotex_M3/cpu.d ./Src/core/port/cpus/ARM_Cotex_M3/cpu.o ./Src/core/port/cpus/ARM_Cotex_M3/port.cyclo ./Src/core/port/cpus/ARM_Cotex_M3/port.d ./Src/core/port/cpus/ARM_Cotex_M3/port.o ./Src/core/port/cpus/ARM_Cotex_M3/port.su

.PHONY: clean-Src-2f-core-2f-port-2f-cpus-2f-ARM_Cotex_M3

