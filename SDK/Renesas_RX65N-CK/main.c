// ----------------------------------------------------------------------------
// Copyright 2020 ARM Ltd.
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

#include <stdio.h>
#include "r_bsp.h"
#include "serial_term_uart.h"

extern int mbed_bootloader_entrypoint(void);

void main(void)
{
    uart_config();

    uart_string_printf("\nRX65N-CK board initialized\n");

    uart_string_printf("Build: ");
    uart_string_printf(__DATE__);
    uart_string_printf("\t");
    uart_string_printf(__TIME__);
    uart_string_printf("\n");

    mbed_bootloader_entrypoint();

    while(1)
    {
        R_BSP_SoftwareDelay(1, BSP_DELAY_SECS);
        uart_string_printf(".");
    }
}
