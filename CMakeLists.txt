cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(PROJECT_NAME ECU)
set(EXE_NAME ${PROJECT_NAME}.elf)
set(OPENOCD_CFG_NAME ${PROJECT_NAME}.cfg)

# CMAKE project name, languages and version
project(${PROJECT_NAME}
    # C and assembly
    LANGUAGES C ASM
    VERSION 0.1
)

# Reduce binary size. Used to fit code in the MCU
option(GARBAGE_COLLECT_SECTIONS "Use -f{function,data}-sections and -Wl,--gc-sections." TRUE)
if (${GARBAGE_COLLECT_SECTIONS})
add_compile_options(-ffunction-sections -fdata-sections)
add_link_options(-Wl,--gc-sections)
endif()

# separate ST HAL Drivers library to ignore warnings from generated files
add_library(ST STATIC)

set(HAL_DRIVERS_SOURCES
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_fdcan.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_iwdg.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c

    Middlewares/Third_Party/FreeRTOS/Source/croutine.c
    Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
    Middlewares/Third_Party/FreeRTOS/Source/list.c
    Middlewares/Third_Party/FreeRTOS/Source/queue.c
    Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
    Middlewares/Third_Party/FreeRTOS/Source/tasks.c
    Middlewares/Third_Party/FreeRTOS/Source/timers.c

    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c

    Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
    Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
)

# Add HAL sources files
target_sources(ST PRIVATE
    "${HAL_DRIVERS_SOURCES}"
)
    
# Add include directory for HAL 
target_include_directories(ST PUBLIC
    Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
    Drivers/CMSIS/Include
    Drivers/CMSIS/Device/ST/STM32H7xx/Include
    Drivers/STM32H7xx_HAL_Driver/Inc
    Middlewares/Third_Party/FreeRTOS/Source/include
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    Core/Inc
)

# Add separate compile commands that ignore some warnings for generated files
target_compile_options(ST PRIVATE 
    -Wno-unused-parameter
    -Wno-shadow
)

target_compile_definitions(ST PUBLIC 
    USE_HAL_DRIVER
    STM32H743xx
)

add_executable(${EXE_NAME})

# Generated files that we also want to ignore warnings
set(ST_CONFIGURATION_SOURCES
    Core/Src/freertos.c
    Core/Src/hook_handlers.c
    Core/Src/stm32h7xx_hal_msp.c
    Core/Src/stm32h7xx_hal_timebase_tim.c
    Core/Src/syscalls.c
    Core/Src/sysmem.c
    Core/Src/system_stm32h7xx.c
)

# The correct way to get the sources is placing the path to each file, 
# but this is a repo used by a lot of users, which the minority will use 
# CMake integration, so to avoid having to update this file in every pull
# let's grab all files from the Src folder
FILE(GLOB ALL_USER_FILES
    Core/Src/*/*.c
    Core/Src/*/*/*.c
)

# Add the rest of the files.
target_sources(${EXE_NAME} PRIVATE
    "${ST_CONFIGURATION_SOURCES}"
    
    Core/Startup/startup_stm32h743vitx.s
    
    Core/Src/main.c
    Core/Src/stm32h7xx_it.c

    "${ALL_USER_FILES}"
)

target_include_directories(${EXE_NAME} PRIVATE
    Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
    Drivers/CMSIS/Include
    Drivers/CMSIS/Device/ST/STM32H7xx/Include
    Drivers/STM32H7xx_HAL_Driver/Inc
    Middlewares/Third_Party/FreeRTOS/Source/include
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    Core/Inc
)

# Ignore warnings in generated files
set_source_files_properties(${ST_CONFIGURATION_SOURCES}
    PROPERTIES COMPILE_OPTIONS -Wno-unused-parameter)

target_compile_options(${EXE_NAME} PRIVATE
    -Wall
    -Wextra
    -Wshadow
)

target_link_options(${EXE_NAME} PRIVATE
    -T${CMAKE_CURRENT_SOURCE_DIR}/STM32H743VITX_FLASH.ld
    -Wl,--gc-sections,--no-warn-rwx-segment
    -specs=nano.specs
)

target_compile_definitions(${EXE_NAME} PUBLIC
    USE_HAL_DRIVER
    STM32H743xx
)

target_link_libraries(${EXE_NAME} PRIVATE
    ST
)

add_custom_target(flash
    openocd -f ${CMAKE_CURRENT_SOURCE_DIR}/${OPENOCD_CFG_NAME} -c "program ${EXE_NAME} reset exit"
    VERBATIM
)
