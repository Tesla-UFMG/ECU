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
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c
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

# Add the rest of the files. Any new file should be added here
target_sources(${EXE_NAME} PRIVATE
    "${ST_CONFIGURATION_SOURCES}"

    Core/Startup/startup_stm32h743vitx.s

    Core/Src/main.c
    Core/Src/stm32h7xx_it.c
    
    Core/Src/CAN/CAN_handler.c
    Core/Src/CAN/CAN_IDs.c
    Core/Src/CAN/general_can_data_manager.c
    Core/Src/CAN/general_can.c
    Core/Src/CAN/inverter_can_data_manager.c
    Core/Src/CAN/inverter_can_monitor.c
    Core/Src/CAN/inverter_can.c
    Core/Src/datalogging/datalog_acquisition.c
    Core/Src/datalogging/datalog_handler.c
    Core/Src/datalogging/datalogger.c
    Core/Src/datalogging/inverter_datalog.c
    Core/Src/datalogging/odometer_calc.c
    Core/Src/datalogging/odometer_save.c
    Core/Src/datalogging/speed.c
    Core/Src/driver_settings/dynamic_controls_choice.c
    Core/Src/driver_settings/modo.c
    Core/Src/driver_settings/pilot_reset.c
    Core/Src/driver_settings/RTD.c
    Core/Src/dynamic_controls/initializer_controls.c
    Core/Src/dynamic_controls/lateral_control.c
    Core/Src/dynamic_controls/longitudinal_control.c
    Core/Src/dynamic_controls/PID.c
    Core/Src/leds/debug_leds_handler.c
    Core/Src/leds/rgb_led_handler.c
    Core/Src/sensors/APPS.c
    Core/Src/sensors/encoder_speed.c
    Core/Src/sensors/steering.c
    Core/Src/torque_command/throttle_control.c
    Core/Src/torque_command/torque_manager.c
    Core/Src/torque_command/torque_message.c
    Core/Src/torque_command/torque_parameters.c

    Core/Src/util/CMSIS_extra/cmsis_extra.c
    Core/Src/util/CMSIS_extra/global_variables_handler.c
    Core/Src/util/buttons_handler.c
    Core/Src/util/error_treatment.c
    Core/Src/util/external_interrupt_handler.c
    Core/Src/util/flash_sector_h7.c
    Core/Src/util/global_variables.c
    Core/Src/util/initializers.c
    Core/Src/util/main_task.c
    Core/Src/util/util.c
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
