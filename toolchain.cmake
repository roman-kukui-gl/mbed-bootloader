SET(CMAKE_SYSTEM_NAME Generic)

# read toolchain path from enviroment variable
set(TOOLCHAIN_DIR $ENV{RXGCC_DIR})

# set toolchain suffix
set(TARGET_TRIPLET "rx-elf")

# combine toolchain dirs path
set(TOOLCHAIN_BIN_DIR ${TOOLCHAIN_DIR}/bin)

# select toolchain extension
if(WIN32)
    set(TOOLCHAIN_EXT ".exe")
else()
    SET(TOOLCHAIN_EXT "")
endif()

# set compiler binary
set(CMAKE_C_COMPILER ${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-gcc${TOOLCHAIN_EXT})
