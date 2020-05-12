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

#include "bootloader_common.h"
#ifdef TARGET_LIKE_MBED
#include "mbed.h"
#include "hal/serial_api.h"
#include "SerialWireOutput.h"
#endif

/* buffer used in storage operations */
uint8_t buffer_array[BUFFER_SIZE];

/* lookup table for printing hexadecimal values */
const char hexTable[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
                          };


#if DEVICE_SERIAL && SHOW_SERIAL_OUTPUT

static serial_t uart = {};

/* module variable for keeping track of initialization */
static bool not_initialized = true;

/**
 * @brief Initialization serial port if needed.
 *.
 */
static void init_serial()
{
    if (not_initialized)
    {
        not_initialized = false;

        serial_init(&uart, STDIO_UART_TX, STDIO_UART_RX);
#if MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE
        serial_baud(&uart, MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);
#endif
    }
}

/**
 * @brief Function that directly outputs to serial port in blocking mode.
 *
 * @param string outputed to serial port.
 */
void boot_debug(const char *s)
{
    init_serial();

    while(*s) {
        serial_putc(&uart, *s);
        s++;
    }
}

#elif DEVICE_ITM && SHOW_SWO_OUTPUT

void boot_debug(const char *s)
{
    static SerialWireOutput swo;
    swo.write(s, strlen(s));
}

#elif __RXv2__
    #error "Target RXv2"
#else

/**
 * @brief Fake function for boot debug.
 *
 * @param unused.
 */
void boot_debug(const char *s)
{
    (void)s;
}

#endif // DEVICE_SERIAL && SHOW_SERIAL_OUTPUT

