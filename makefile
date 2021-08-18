INCLUDES := -I./Middlewares/Third_Party/FreeRTOS/Source/include -I./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I./Drivers/CMSIS/Include -I./Core/Inc -I./Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I./Drivers/CMSIS/Device/ST/STM32H7xx/Include -I./Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I./Drivers/STM32H7xx_HAL_Driver/Inc
CC := arm-none-eabi-gcc
LDFLAGS :=
CFLAGS := $(INCLUDES) -std=gnu11 -g3 -c -Og -Wall --specs=nano.specs
STMFLAGS := -mcpu=cortex-m7 -DUSE_HAL_DRIVER -DSTM32H743xx -DDEBUG -ffunction-sections -fdata-sections -fstack-usage -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb
C_SOURCES := $(shell find . -name '*.c')
C_EXECUTABLE := $(C_SOURCES:./%.c=build/%.o)
S_SOURCES := $(shell find . -name '*.s')
S_EXECUTABLE := $(S_SOURCES:./%.s=build/%.o)

RM := rm -rf

BUILD_ARTIFACT_NAME := ECU
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
ECU.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
ECU.list \

OBJCOPY_BIN += \
ECU.bin \


# All Target
all: main-build

# Main-build Target
main-build: ECU.elf secondary-outputs

# Tool invocations
ECU.elf: $(C_EXECUTABLE) $(S_EXECUTABLE) ./STM32H743VITX_FLASH.ld makefile
	arm-none-eabi-gcc -o "build/ECU.elf" @"build/objects.list" $(LIBS) -mcpu=cortex-m7 -T"./STM32H743VITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="build/ECU.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '
	-$(RM) build/objects.list

default.size.stdout: $(EXECUTABLES) makefile objects.list
	arm-none-eabi-size  build/$(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

ECU.list: $(EXECUTABLES) makefile objects.list
	arm-none-eabi-objdump -h -S build/$(EXECUTABLES) > "build/ECU.list"
	@echo 'Finished building: $@'
	@echo ' '

ECU.bin: $(EXECUTABLES) makefile objects.list
	arm-none-eabi-objcopy  -O binary build/$(EXECUTABLES) "build/ECU.bin"
	@echo 'Finished building: $@'
	@echo ' '

objects.list:
	touch build/objects.list

printit:
	echo $(C_EXECUTABLE)
	echo $(S_EXECUTABLE)

# Other Targets
clean:
	-$(RM) $(SIZE_OUTPUT) build/$(OBJDUMP_LIST) build/$(EXECUTABLES)$(C_EXECUTABLE) $(C_EXECUTABLE:%.o=%.d) $(C_EXECUTABLE:%.o=%.su) $(S_EXECUTABLE) $(S_EXECUTABLE:%.o=%.d) $(S_EXECUTABLE:%.o=%.su) build/$(OBJCOPY_BIN) build/ECU.elf build/ECU.map build/objects.list
	find ./build/ -type d -empty -delete
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents fail-specified-linker-script-missing warn-no-linker-script-specified make-objects.list


$(C_EXECUTABLE): make-objects.list
	$(eval SRC_FILE := $(@:build/%.o=./%.c))
	@mkdir -p $(@D)
	@echo 'Compiling: $(SRC_FILE)'
	$(CC) "$(SRC_FILE)" $(LDFLAGS) $(CFLAGS) $(STMFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@"
	@echo '"$@"' >> build/objects.list

$(S_EXECUTABLE): make-objects.list
	$(eval SRC_FILE := $(@:build/%.o=./%.s))
	@mkdir -p $(@D)
	@echo 'Compiling Assembly: $(SRC_FILE)'
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$(SRC_FILE)"
	@echo '"$@"' >> build/objects.list


# automatically generate dependency rules

# -MF  write the generated dependency rule to a file
# -MG  assume missing headers will be generated and don't stop with an error
# -MM  generate dependency rule for prerequisite, skipping system headers
# -MP  add phony target for each header to prevent errors when header is missing
# -MT  add a target to the generated dependency
