cmake_minimum_required(VERSION 3.5)


# Address of application reset vector
add_definitions(-DMBED_CONF_MBED_BOOTLOADER_APPLICATION_START_ADDRESS=0xFFE00000)

# FIRMWARE_METADATA_HEADER_ADDRESS = MBED_CONF_UPDATE_CLIENT_APPLICATION_DETAILS
add_definitions(-DFIRMWARE_METADATA_HEADER_ADDRESS=0xFFFF0000)

add_definitions(-DMBED_CONF_MBED_BOOTLOADER_MAX_APPLICATION_SIZE=0xF0000)

add_definitions(-DMAX_BOOT_RETRIES=3)

add_definitions(-DMBED_CLOUD_CLIENT_UPDATE_STORAGE=ARM_UCP_FLASHIAP)

#define boot_debug(x)
add_definitions(-Dboot_debug=uart_string_printf)


message("RX65N Cloud Kit Bootloader Build System")

# toolchain settings
include(toolchain.cmake)

project(RX65N-CK_bootloader C ASM)

# toolchain compiler & linker flags
include(rx65n-flags.cmake)

set(SDK_PATH SDK/Renesas_RX65N-CK)

include_directories(${SDK_PATH}/r_config)
include_directories(${SDK_PATH}/r_pincfg)

include_directories(${SDK_PATH}/FIT/r_bsp)
include_directories(${SDK_PATH}/FIT/r_bsp/board/generic_rx65n)

include_directories(${SDK_PATH}/FIT/r_flash_rx)
include_directories(${SDK_PATH}/FIT/r_flash_rx/src)
include_directories(${SDK_PATH}/FIT/r_flash_rx/src/flash_type_4)

include_directories(${SDK_PATH}/FIT/r_byteq)

include_directories(${SDK_PATH}/FIT/r_sci_rx)

include_directories(${SDK_PATH}/serial_term)


set(BOOTLOADER_SRCS
    ${SDK_PATH}/FIT/r_bsp/board/all/reset_program.S
    ${SDK_PATH}/FIT/r_bsp/board/all/resetprg.c

    ${SDK_PATH}/FIT/r_bsp/board/rx65n/vecttbl.c
    ${SDK_PATH}/FIT/r_bsp/board/rx65n/clkconf.c

    ${SDK_PATH}/FIT/r_bsp/board/generic_rx65n/r_bsp_hwsetup_reference.c

    ${SDK_PATH}/FIT/r_bsp/mcu/rx65n/cpu.c
    ${SDK_PATH}/FIT/r_bsp/mcu/rx65n/mcu_clocks.c
    ${SDK_PATH}/FIT/r_bsp/mcu/rx65n/mcu_locks.c
    ${SDK_PATH}/FIT/r_bsp/mcu/rx65n/mcu_interrupts.c
    ${SDK_PATH}/FIT/r_bsp/mcu/rx65n/mcu_mapped_interrupts.c
    ${SDK_PATH}/FIT/r_bsp/mcu/rx65n/mcu_init.c

    ${SDK_PATH}/FIT/r_bsp/mcu/all/r_bsp_common.c

    ${SDK_PATH}/FIT/r_flash_rx/src/r_flash_rx.c
    ${SDK_PATH}/FIT/r_flash_rx/src/r_flash_group.c
    ${SDK_PATH}/FIT/r_flash_rx/src/r_flash_fcu.c
    ${SDK_PATH}/FIT/r_flash_rx/src/flash_type_4/r_flash_type4.c

    ${SDK_PATH}/FIT/r_byteq/src/r_byteq.c

    ${SDK_PATH}/FIT/r_sci_rx/src/targets/rx65n/r_sci_rx65n_data.c
    ${SDK_PATH}/FIT/r_sci_rx/src/targets/rx65n/r_sci_rx65n.c
    ${SDK_PATH}/FIT/r_sci_rx/src/r_sci_rx.c

    ${SDK_PATH}/r_pincfg/r_sci_rx_pinset.c

    ${SDK_PATH}/serial_term/serial_term_uart.c

    ${SDK_PATH}/main.c
)

add_executable(bootloader.elf ${BOOTLOADER_SRCS})
