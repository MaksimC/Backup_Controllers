#include <stdio.h>
#include "../lib/andygock_avr-uart/uart.h"

//for atmel studio
#ifndef F_CPU
#define F_CPU 16000000UL
#endif



int uart0_putchar(char c, FILE *stream)
{
    (void)stream;

    if (c == '\n') {
        uart0_putc('\r');
    }

    uart0_putc((uint8_t)c);
    return 0;
}


int uart0_getchar(FILE *stream)
{
    (void)stream;

    while (uart0_peek() == UART_NO_DATA) {
    }

    return (uart0_getc() & 0x00ff);
}


int uart3_putchar(char c, FILE *stream)
{
    (void)stream;

    if (c == '\n') {
        uart3_putc('\r');
    }

    uart3_putc((uint8_t)c);
    return 0;
}