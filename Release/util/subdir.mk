################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../util/asoutput.c \
../util/convert.c \
../util/lang.c \
../util/opcode.c \
../util/parse.c \
../util/symbol.c 

OBJS += \
./util/asoutput.o \
./util/convert.o \
./util/lang.o \
./util/opcode.o \
./util/parse.o \
./util/symbol.o 

C_DEPS += \
./util/asoutput.d \
./util/convert.d \
./util/lang.d \
./util/opcode.d \
./util/parse.d \
./util/symbol.d 


# Each subdirectory must supply rules for building sources it contributes
util/%.o: ../util/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/Users/yigalalexander/git/ex14/util" -O3 -pedantic -pedantic-errors -Wall -Werror -c -fmessage-length=0 -ansi -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


