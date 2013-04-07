################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../testers/opc.c 

OBJS += \
./testers/opc.o 

C_DEPS += \
./testers/opc.d 


# Each subdirectory must supply rules for building sources it contributes
testers/%.o: ../testers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/Users/yigalalexander/git/ex14/util" -O3 -pedantic -pedantic-errors -Wall -Werror -c -fmessage-length=0 -ansi -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


