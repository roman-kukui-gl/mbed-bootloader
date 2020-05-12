#!/bin/bash

export RXGCC_DIR="$HOME/toolchains/gcc_8.3.0.201904_gnurx-elf"

rm -fr ./build
mkdir build
cd build

cmake ./.. -G "Unix Makefiles"

export TARGET_NAME="bootloader"

rm $TARGET_NAME.mot

make -j1 $TARGET_NAME.elf

rx-elf-objcopy $TARGET_NAME.elf -O srec -I elf32-rx-be-ns $TARGET_NAME.mot

rx-elf-objdump -h $TARGET_NAME.elf
rx-elf-objdump -h $TARGET_NAME.mot