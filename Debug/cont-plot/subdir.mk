################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../cont-plot/contagem.c 

OBJS += \
./cont-plot/contagem.o 

C_DEPS += \
./cont-plot/contagem.d 


# Each subdirectory must supply rules for building sources it contributes
cont-plot/%.o: ../cont-plot/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


