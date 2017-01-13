/*   Copyright (C) 2017 Maksim Tseljabov <Maksim.Tseljabov@rigold.ee>
*
*   This file is a part of RVLP Home Project.
*
*   RVLP Home Project is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   RVLP Home Project is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with RVLP Home Project.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <stdio.h>
#include "../lib/andygock_avr-uart/uart.h"


#warning REMOVE ATMEL STUFF
//for atmel studio
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

static int uart0_putchar(char c, FILE *stream)
{
    (void)stream;

    if (c == '\n')
    {
        uart0_putc('\r');
    }

    uart0_putc((uint8_t)c);
    return 0;
}


static int uart0_getchar(FILE *stream)
{
    (void)stream;

    while (uart0_peek() == UART_NO_DATA)
    {
    }

    return (uart0_getc() & 0x00ff);
}


static int uart3_putchar(char c, FILE *stream)
{
    (void)stream;

    if (c == '\n') {
        uart3_putc('\r');
    }

    uart3_putc((uint8_t)c);
    return 0;
}


FILE uart0_io = FDEV_SETUP_STREAM(uart0_putchar, uart0_getchar, _FDEV_SETUP_RW);
FILE uart3_out = FDEV_SETUP_STREAM(uart3_putchar, NULL, _FDEV_SETUP_WRITE);