################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/compress.c \
../src/data_structure.c \
../src/descompress.c \
../src/filezip.c 

O_SRCS += \
../src/compress.o \
../src/data_structure.o \
../src/descompress.o \
../src/filezip.o 

OBJS += \
./src/compress.o \
./src/data_structure.o \
./src/descompress.o \
./src/filezip.o 

C_DEPS += \
./src/compress.d \
./src/data_structure.d \
./src/descompress.d \
./src/filezip.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


