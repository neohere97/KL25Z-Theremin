################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/cbfifo.c \
../source/command_processor.c \
../source/dac.c \
../source/hexdump.c \
../source/main.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/sin.c \
../source/systick.c \
../source/tone_to_sample.c \
../source/uart0.c 

OBJS += \
./source/cbfifo.o \
./source/command_processor.o \
./source/dac.o \
./source/hexdump.o \
./source/main.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/sin.o \
./source/systick.o \
./source/tone_to_sample.o \
./source/uart0.o 

C_DEPS += \
./source/cbfifo.d \
./source/command_processor.d \
./source/dac.d \
./source/hexdump.d \
./source/main.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/sin.d \
./source/systick.d \
./source/tone_to_sample.d \
./source/uart0.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\shala\Documents\MCUXpressoIDE_11.5.0_7232\workspace\PESFinalProject\board" -I"C:\Users\shala\Documents\MCUXpressoIDE_11.5.0_7232\workspace\PESFinalProject\source" -I"C:\Users\shala\Documents\MCUXpressoIDE_11.5.0_7232\workspace\PESFinalProject" -I"C:\Users\shala\Documents\MCUXpressoIDE_11.5.0_7232\workspace\PESFinalProject\startup" -I"C:\Users\shala\Documents\MCUXpressoIDE_11.5.0_7232\workspace\PESFinalProject\CMSIS" -I"C:\Users\shala\Documents\MCUXpressoIDE_11.5.0_7232\workspace\PESFinalProject\drivers" -I"C:\Users\shala\Documents\MCUXpressoIDE_11.5.0_7232\workspace\PESFinalProject\utilities" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


