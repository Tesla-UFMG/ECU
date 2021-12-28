INCLUDES := -I./Middlewares/Third_Party/FreeRTOS/Source/include -I./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I./Drivers/CMSIS/Include -I./Core/Inc -I./Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I./Drivers/CMSIS/Device/ST/STM32H7xx/Include -I./Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I./Drivers/STM32H7xx_HAL_Driver/Inc

BUILD_DIR := Build

ARM_CLANG_CC := clang
ARM_GNU_CC := arm-none-eabi-gcc
ARCH_CFLAGS += \
  -mthumb -mcpu=cortex-m7 \
  -mfloat-abi=hard -mfpu=fpv5-d16

CLANG_TIDY := clang-tidy

CLANG_TIDY_CHECKS := -checks=-*,$\
clang-analyzer-*,$\
-clang-analyzer-cplusplus*,$\
clang-diagnostic-*,$\
bugprone-*,$\
-bugprone-easily-swappable-parameters*,$\
cert-*,$\
llvm-*,$\
-llvm-include-order*,$\
-llvm-header-guard*,$\
google-*,$\
misc-*,$\
performance-*,$\
-performance-no-int-to-ptr*,$\
portability-*,$\
readability-*,$\
-readability-magic-numbers*

CLANG_TIDY_EXTRA_FLAGS ?=

CLANG_TIDY_EXPORT_PATH := $(BUILD_DIR)/clang-tidy-export

CLANG_TIDY_FLAGS := $(CLANG_TIDY_CHECKS) -export-fixes=$(CLANG_TIDY_EXPORT_PATH) $(CLANG_TIDY_EXTRA_FLAGS)

CLANG_FORMAT := clang-format
CLANG_FORMAT_FLAGS := -style=file -i

CLANG_GOALS := clang-tidy clang clang-format

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

  COMPILER_SPECIFIC_CFLAGS += Wextra

  COMPILER_SPECIFIC_LDFLAGS += \
    -L$(ARM_CORTEXM_SYSROOT)/lib/$(ARM_CORTEXM_MULTI_DIR) \
    $(ARM_CORTEXM_BUILTINS)
else
  COMPILER_SPECIFIC_CFLAGS += --specs=nano.specs
endif

LDFLAGS := $(COMPILER_SPECIFIC_LDFLAGS)
CFLAGS := $(INCLUDES) $(COMPILER_SPECIFIC_CFLAGS) -std=gnu11 -g3 -c -Og -Wall -fno-short-enums

C_SOURCES := $(shell find . -name '*.c')
C_EXECUTABLE := $(C_SOURCES:./%.c=$(BUILD_DIR)/%.o)
S_SOURCES := $(shell find . -name '*.s')
S_EXECUTABLE := $(S_SOURCES:./%.s=$(BUILD_DIR)/%.o)

FILTER_FROM_CORE := ! -name '*stm32h7xx*.*' ! -name '*system_stm32h7xx*.*' ! -name '*sysmem.*' ! -name '*syscalls.*' ! -name '*freertos.*' ! -name '*main.*'
C_SOURCES_CORE := $(shell find Core -name '*.c' $(FILTER_FROM_CORE))
HEADERS_CORE := $(shell find Core -name '*.h' $(FILTER_FROM_CORE))
# CLANG_TIDY_C_SOURCES := $(C_SOURCES_CORE:%=%.clang-tidy)
# CLANG_TIDY_HEADERS := $(HEADERS_CORE:%=%.clang-tidy)
# CLANG_TIDY_FILES := $(CLANG_TIDY_C_SOURCES) $(CLANG_TIDY_HEADERS)

RM := rm -rf

BUILD_ARTIFACT_NAME := ECU
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables
EXECUTABLES := $(BUILD_DIR)/ECU.elf

SIZE_OUTPUT := $(BUILD_DIR)/default.size.stdout

OBJDUMP_LIST := $(BUILD_DIR)/ECU.list

OBJCOPY_BIN := $(BUILD_DIR)/ECU.bin

OBJ_MAP := $(BUILD_DIR)/ECU.map

OBJECTS_LIST := $(BUILD_DIR)/objects.list


.PHONY: clang clang-tidy clang-format create-build-dir
clang: main-build
clang-tidy: create-build-dir
	$(CLANG_TIDY) $(CLANG_TIDY_FLAGS) $(C_SOURCES_CORE) $(HEADERS_CORE) -- $(LDFLAGS) $(CFLAGS) $(STMFLAGS)

clang-format: create-build-dir
	$(CLANG_FORMAT) $(CLANG_FORMAT_FLAGS) $(C_SOURCES_CORE) $(HEADERS_CORE)

# $(CLANG_TIDY_FILES):
# 	@echo "Analyzing $(@:%.clang-tidy=%)"
# 	$(CLANG_TIDY) $(CLANG_TIDY_FLAGS) $(@:%.clang-tidy=%) -- $(LDFLAGS) $(CFLAGS) $(STMFLAGS)

# All Target
all: main-build

# Main-build Target
main-build: $(EXECUTABLES) secondary-outputs

# Tool invocations
$(EXECUTABLES): $(C_EXECUTABLE) $(S_EXECUTABLE) ./STM32H743VITX_FLASH.ld
	arm-none-eabi-gcc -o "$(EXECUTABLES)" @"$(OBJECTS_LIST)" $(LIBS) -mcpu=cortex-m7 -T"./STM32H743VITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="$(OBJ_MAP)" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

$(SIZE_OUTPUT): $(EXECUTABLES) $(OBJECTS_LIST)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

$(OBJDUMP_LIST): $(EXECUTABLES) $(OBJECTS_LIST)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "$(OBJDUMP_LIST)"
	@echo 'Finished building: $@'
	@echo ' '

$(OBJCOPY_BIN): $(EXECUTABLES) $(OBJECTS_LIST)
	arm-none-eabi-objcopy -O binary $(EXECUTABLES) "$(OBJCOPY_BIN)"
	@echo 'Finished building: $@'
	@echo ' '

create-build-dir:
	@echo 'Creating build directory: $(BUILD_DIR)'
	@mkdir -p $(BUILD_DIR)

$(OBJECTS_LIST): create-build-dir
	touch $(OBJECTS_LIST)

printit:
	echo $(C_EXECUTABLE)
	echo $(S_EXECUTABLE)

# Other Targets
clean:
	-$(RM) $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(EXECUTABLES) $(C_EXECUTABLE) $(C_EXECUTABLE:%.o=%.d) $(C_EXECUTABLE:%.o=%.su) $(S_EXECUTABLE) $(S_EXECUTABLE:%.o=%.d) $(S_EXECUTABLE:%.o=%.su) $(OBJCOPY_BIN) $(CLANG_TIDY_EXPORT_PATH) $(EXECUTABLES) $(OBJ_MAP) $(OBJECTS_LIST)
	find $(BUILD_DIR) -mindepth 1 -type d -empty -delete
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents fail-specified-linker-script-missing warn-no-linker-script-specified make-objects.list


$(C_EXECUTABLE): $(OBJECTS_LIST)
	$(eval SRC_FILE := $(@:$(BUILD_DIR)/%.o=%.c))
	@mkdir -p $(@D)
	@echo 'Compiling: $(SRC_FILE)'
	$(CC) "$(SRC_FILE)" $(LDFLAGS) $(CFLAGS) $(STMFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@"
	@if ! grep -q '"$@"' '$(OBJECTS_LIST)'; then \
		echo '"$@"' >> $(OBJECTS_LIST); \
	fi\

$(S_EXECUTABLE): $(OBJECTS_LIST)
	$(eval SRC_FILE := $(@:$(BUILD_DIR)/%.o=%.s))
	@mkdir -p $(@D)
	@echo 'Compiling Assembly: $(SRC_FILE)'
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$(SRC_FILE)"
	@if ! grep -q '"$@"' '$(OBJECTS_LIST)'; then \
		echo '"$@"' >> $(OBJECTS_LIST); \
	fi\


# automatically generate dependency rules

# -MF  write the generated dependency rule to a file
# -MG  assume missing headers will be generated and don't stop with an error
# -MM  generate dependency rule for prerequisite, skipping system headers
# -MP  add phony target for each header to prevent errors when header is missing
# -MT  add a target to the generated dependency
