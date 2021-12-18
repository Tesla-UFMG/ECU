INCLUDES := -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32H7xx_HAL_Driver/Inc
ARM_CLANG_CC := clang
ARM_GNU_CC := arm-none-eabi-gcc
ARCH_CFLAGS += \
  -mthumb -mcpu=cortex-m7 \
  -mfloat-abi=hard -mfpu=fpv5-d16

CLANG_TIDY := clang-tidy

CLANG_TIDY_CHECKS := -checks=-*,$\
clang-analyzer-*,$\
-clang-analyzer-cplusplus*,$\
bugprone-*,$\
-bugprone-easily-swappable-parameters*,$\
cert-*,$\
llvm-*,$\
-llvm-include-order*,$\
-llvm-header-guard*,$\
misc-*,$\
performance-*,$\
-performance-no-int-to-ptr*,$\
portability-*,$\
readability-*,$\
-readability-magic-numbers*

CLANG_TIDY_EXPORT_PATH := ./clang-tidy-export

CLANG_TIDY_FLAGS := $(CLANG_TIDY_CHECKS) -export-fixes=$(CLANG_TIDY_EXPORT_PATH)

CLANG_GOALS := clang-tidy clang

ifneq (,$(filter $(CLANG_GOALS),$(MAKECMDGOALS)))
   USING_COMPILER := clang
else
   USING_COMPILER := gnu
endif


ifeq (clang,$(USING_COMPILER))
  CC := $(ARM_CLANG_CC)
else
  CC := $(ARM_GNU_CC)
endif


STMFLAGS := $(ARCH_CFLAGS) -DUSE_HAL_DRIVER -DSTM32H743xx -DDEBUG -ffunction-sections -fdata-sections

COMPILER_SPECIFIC_CFLAGS :=
COMPILER_SPECIFIC_LDFLAGS :=

ifeq (clang,$(USING_COMPILER))
  ARM_CORTEXM_SYSROOT ?= \
    $(shell $(ARM_GNU_CC) $(ARCH_CFLAGS) -print-sysroot 2>&1)

  ARM_CORTEXM_MULTI_DIR ?= \
    $(shell $(ARM_GNU_CC) $(ARCH_CFLAGS) -print-multi-directory 2>&1)

  ARM_CORTEXM_BUILTINS ?= \
    $(shell $(ARM_GNU_CC) $(ARCH_CFLAGS) -print-libgcc-file-name 2>&1)

  COMPILER_SPECIFIC_CFLAGS += \
    --target=arm-none-eabi \
    --sysroot=$(ARM_CORTEXM_SYSROOT)

  COMPILER_SPECIFIC_LDFLAGS += \
    -L$(ARM_CORTEXM_SYSROOT)/lib/$(ARM_CORTEXM_MULTI_DIR) \
    $(ARM_CORTEXM_BUILTINS)
else
  COMPILER_SPECIFIC_CFLAGS += --specs=nano.specs
endif

LDFLAGS := $(COMPILER_SPECIFIC_LDFLAGS)
CFLAGS := $(INCLUDES) $(COMPILER_SPECIFIC_CFLAGS) -std=gnu11 -g3 -c -Og -Wall -fno-short-enums

C_SOURCES := $(shell find .. -name '*.c')
C_EXECUTABLE := $(C_SOURCES:../%.c=./%.o)
S_SOURCES := $(shell find .. -name '*.s')
S_EXECUTABLE := $(S_SOURCES:../%.s=./%.o)

FILTER_FROM_CORE := ! -name '*stm32h7xx*.*' ! -name '*system_stm32h7xx*.*' ! -name '*sysmem.*' ! -name '*syscalls.*' ! -name '*freertos.*' ! -name '*main.*'
C_SOURCES_CORE := $(shell find ../Core -name '*.c' $(FILTER_FROM_CORE))
HEADERS_CORE := $(shell find ../Core -name '*.h' $(FILTER_FROM_CORE))
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


.PHONY: clang clang-tidy
clang: main-build
clang-tidy:
	$(CLANG_TIDY) $(CLANG_TIDY_FLAGS) $(C_SOURCES_CORE) $(HEADERS_CORE) -- $(LDFLAGS) $(CFLAGS) $(STMFLAGS)

# All Target
all: main-build

# Main-build Target
main-build: ECU.elf secondary-outputs

# Tool invocations
ECU.elf: $(C_EXECUTABLE) $(S_EXECUTABLE) ../STM32H743VITX_FLASH.ld makefile
	arm-none-eabi-gcc -o "ECU.elf" @"objects.list" $(LIBS) -mcpu=cortex-m7 -T"../STM32H743VITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="ECU.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

ECU.list: $(EXECUTABLES) makefile objects.list
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "ECU.list"
	@echo 'Finished building: $@'
	@echo ' '

ECU.bin: $(EXECUTABLES) makefile objects.list
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "ECU.bin"
	@echo 'Finished building: $@'
	@echo ' '

objects.list:
	touch objects.list

printit:
	echo $(C_EXECUTABLE)
	echo $(S_EXECUTABLE)

# Other Targets
clean:
	-$(RM) $(SIZE_OUTPUT)$(OBJDUMP_LIST)$(EXECUTABLES)$(C_EXECUTABLE) $(C_EXECUTABLE:%.o=%.d) $(C_EXECUTABLE:%.o=%.su) $(S_EXECUTABLE) $(S_EXECUTABLE:%.o=%.d) $(S_EXECUTABLE:%.o=%.su) $(OBJCOPY_BIN) $(CLANG_TIDY_EXPORT_PATH) ECU.elf ECU.map objects.list
	find . -type d -empty -delete
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents fail-specified-linker-script-missing warn-no-linker-script-specified make-objects.list


$(C_EXECUTABLE): make-objects.list
	$(eval SRC_FILE := $(@:%.o=../%.c))
	@mkdir -p $(@D)
	@echo 'Compiling: $(SRC_FILE)'
	$(CC) "$(SRC_FILE)" $(LDFLAGS) $(CFLAGS) $(STMFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@"
	@echo '"$@"' >> objects.list

$(S_EXECUTABLE): make-objects.list
	$(eval SRC_FILE := $(@:%.o=../%.s))
	@mkdir -p $(@D)
	@echo 'Compiling Assembly: $(SRC_FILE)'
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$(SRC_FILE)"
	@echo '"$@"' >> objects.list


# automatically generate dependency rules

# -MF  write the generated dependency rule to a file
# -MG  assume missing headers will be generated and don't stop with an error
# -MM  generate dependency rule for prerequisite, skipping system headers
# -MP  add phony target for each header to prevent errors when header is missing
# -MT  add a target to the generated dependency
