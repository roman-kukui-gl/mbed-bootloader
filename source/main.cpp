// ----------------------------------------------------------------------------
// Copyright 2016-2017 ARM Ltd.
//
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------------------------------------------------------

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <inttypes.h>
#include <stdlib.h>
#ifdef TARGET_LIKE_MBED
#include "mbed.h"
#endif

#include "update-client-paal/arm_uc_paal_update_api.h"
#include "update-client-common/arm_uc_types.h"

#include "mbed_bootloader_info.h"
#include "bootloader_platform.h"
#include "active_application.h"
#include "bootloader_common.h"
#ifdef TARGET_LIKE_MBED
#include "mbed_application.h"
#endif
#include "upgrade.h"

const arm_uc_installer_details_t bootloader = {
    .arm_hash = BOOTLOADER_ARM_SOURCE_HASH,
    .oem_hash = BOOTLOADER_OEM_SOURCE_HASH,
    .layout   = BOOTLOADER_STORAGE_LAYOUT
};

#if defined(MBED_BOOTLOADER_NONSTANDARD_ENTRYPOINT)
extern "C"
int mbed_bootloader_entrypoint(void)
#else
int main(void)
#endif
{
    // this forces the linker to keep bootloader object now that it's not
    // printed anymore
    *const_cast<volatile uint32_t *>(&bootloader.layout);

    /* Use malloc to allocate uint64_t version number on the heap */
    heapVersion = (uint64_t *) malloc(sizeof(uint64_t));
    bootCounter = (uint8_t *) malloc(1);

    /*************************************************************************/
    /* Print bootloader information                                          */
    /*************************************************************************/

    boot_debug("\r\nMbed Bootloader\r\n");

#if MBED_CONF_MBED_TRACE_ENABLE
    mbed_trace_init();
    mbed_trace_print_function_set(boot_debug);
#endif // MBED_CONF_MBED_TRACE_ENABLE

#if MBED_CONF_MBED_BOOTLOADER_STARTUP_DELAY
    ThisThread::sleep_for(MBED_CONF_MBED_BOOTLOADER_STARTUP_DELAY);
#endif // MBED_CONF_MBED_BOOTLOADER_STARTUP_DELAY

    /* Initialize PAL */
    int32_t ucp_result = MBED_CLOUD_CLIENT_UPDATE_STORAGE.Initialize();
    if (ucp_result != ERR_NONE) {
        boot_debug("Failed to initialize update storage\r\n");
        return -1;
    }

    /*************************************************************************/
    /* Update                                                                */
    /*************************************************************************/

    /* Initialize internal flash */
    if (!activeStorageInit()) {
        boot_debug("Failed to initialize active storage\r\n");
        return -1;
    }

    /* Try to update firmware from journal */
    if (upgradeApplicationFromStorage()) {
        /* deinit storage driver */
        activeStorageDeinit();

        /* forward control to ACTIVE application if it is deemed sane */
        boot_debug("booting...\r\n\r\n");

        #ifdef TARGET_LIKE_MBED
        mbed_start_application(MBED_CONF_APP_APPLICATION_JUMP_ADDRESS);
        #else
        /// \todo Add app start
        #endif
    }

    /* Reset bootCounter; this allows a user to reapply a new bootloader
       without having to power cycle the device.
    */
    if (bootCounter) {
        *bootCounter = 0;
    }

    boot_debug("Failed to jump to application!\r\n");
    return -1;
}
