################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/user/stm32f10x_it.c 

OBJS += \
./Src/user/stm32f10x_it.o 

C_DEPS += \
./Src/user/stm32f10x_it.d 


# Each subdirectory must supply rules for building sources it contributes
Src/user/%.o Src/user/%.su Src/user/%.cyclo: ../Src/user/%.c Src/user/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103ZETx -c -I../Inc -I"/Users/luwenjin/work/willing/willingOS-202401291529/Src/Libraries/CMSIS" -I"/Users/luwenjin/work/willing/willingOS-202401291529/Src/app/led" -I"/Users/luwenjin/work/willing/willingOS-202401291529/Src/core/port/cpus/ARM_Cotex_M3" -I"/Users/luwenjin/work/willing/willingOS-202401291529/Src/Libraries/STM32F10x_StdPeriph_Driver/inc" -I"/Users/luwenjin/work/willing/willingOS-202401291529/Src/user" -I"/Users/luwenjin/work/willing/willingOS-202401291529/Src/core/sys" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src-2f-user

clean-Src-2f-user:
	-$(RM) ./Src/user/stm32f10x_it.cyclo ./Src/user/stm32f10x_it.d ./Src/user/stm32f10x_it.o ./Src/user/stm32f10x_it.su

.PHONY: clean-Src-2f-user

