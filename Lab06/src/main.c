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
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "uart-wrapper.h"
#include "print_helper.h"
#include "hmi_msg.h"
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "cli_microrl.h"
#include "../lib/helius_microrl/microrl.h"
#include "../lib/matejx_avr_lib/mfrc522.h"
#include "rfid.h"


#warning REMOVE ATMEL STUFF
//For Atmel Studio
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#warning REMOVE ATMEL STUFF
/*For Atmel studio
#include "C:\Users\emaktse\Documents\HITSA\GIT Repository\Controllers\Lab06\Lab06\lib\hd44780_111\hd44780.h"
#include "C:\Users\emaktse\Documents\HITSA\GIT Repository\Controllers\Lab06\Lab06\lib\andygock_avr-uart\uart.h"
#include "C:\Users\emaktse\Documents\HITSA\GIT Repository\Controllers\Lab06\Lab06\lib\helius_microrl\microrl.h"

ALSO DELETE F_CPU definition in HD44_SETTINGS.H!!!
ALSO REPLACE in void print_version(FILE *stream), microrl.c, GIT_VER!!

*/

#define BAUD 9600

volatile uint32_t system_time;

#define DOOR_INIT DDRA |= _BV(DDA1)
#define DOOR_OPEN PORTA |= _BV(PORTA1)
#define DOOR_CLOSE PORTA &= ~_BV(PORTA1)

// Create microrl object and pointer on it
microrl_t rl;
microrl_t *prl = &rl;


static inline void init_system_clock(void)
{
    TCCR5A = 0; // Clear control register A
    TCCR5B = 0; // Clear control register B
    TCCR5B |= _BV(WGM52) | _BV(CS52); // CTC and fCPU/256
    OCR5A = 62549; // 1 s
    TIMSK5 |= _BV(OCIE5A); // Output Compare A match Interrupt Enable
}


static inline void initialize_io()
{
    /* Set pin 3 of PORTA for output */
    DDRA |= _BV(DDA3);
    /* Init in/out console in UART0*/
    uart0_init(UART_BAUD_SELECT(BAUD, F_CPU));
    stdin = stdout = &uart0_io;
    /* Init error console in UART3*/
    uart3_init(UART_BAUD_SELECT(BAUD, F_CPU));
    stderr = &uart3_out;
}


static inline void initialize_interrupts()
{
    /* System Enable Interrupts */
    sei();
}


static inline void initialize_counter()
{
    /* Initialize counter */
    init_system_clock();
}


static inline void initialize_lcd()
{
    /* Initialize display and clear screen */
    lcd_init();
    lcd_clrscr();
}


static inline void initialize_rfid(void)
{
    //Initialize RFID reader HW
    MFRC522_init();
    PCD_Init();
}


static inline void print_console (void)
{
    print_version(stderr);
    //print student name
    fprintf_P(stdout, PSTR(STUD_NAME));
    fputc('\n', stdout); // Add a new line to the uart printout
    fprintf_P(stdout, PSTR(CLI_START_MSG));
    lcd_puts_P(PSTR(STUD_NAME));
    fputc('\n', stdout); // Add a new line to the uart printout
}


static inline void start_cli(void)
{
    // Call init with ptr to microrl instance and print callback
    microrl_init (prl, cli_print);
    // Set callback for execute
    microrl_set_execute_callback (prl, cli_execute);
}


static inline void heartbeat ()
{
    static uint32_t current_time;
    uint32_t system_time_copy;
    /*Copy of system_time using atomic block*/
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        system_time_copy = system_time;
    }

    if (current_time != system_time_copy)
    {
        /* Toggle led */
        PORTA ^= _BV(PORTA3);
        /* Printout system uptime */
        fprintf_P(stderr, PSTR(LBL_UPTIME ": %lu s\n"), system_time_copy);
    }

    current_time = system_time_copy;
}


static inline void handle_door()
{
    Uid uid;
    card_t card;
    uint32_t time_cur = system_time;//time();
    static uint32_t message_start;
    static uint32_t door_open_start;
    if (PICC_IsNewCardPresent())
    {
        PICC_ReadCardSerial(&uid);
        card.uid_size = uid.size;
        memcpy(&card.uid, &uid.uidByte, uid.size);
        card.user = NULL;
        card_t *found_card = rfid_find_card(&card);
        if (found_card)
        {
            lcd_clr(0X40, 16);
            lcd_goto(0x40);
            lcd_puts(found_card->user);

            DOOR_OPEN;
        }
        else
        {
            DOOR_CLOSE;
            lcd_clr(0X40, 16);
            lcd_goto(0x40);
            lcd_puts_P(access_denied_msg);
        }
        door_open_start = time_cur;
        message_start = time_cur;
    }

    if ((message_start + 3) < time_cur)
    {
        message_start = time_cur + 3;
        lcd_clr(0X40, 16);
    }

    if ((door_open_start + 2) < time_cur)
    {
        DOOR_CLOSE;
    }
}


void main (void)
{
    initialize_io();
    initialize_counter();
    initialize_lcd();
    initialize_interrupts();
    initialize_rfid();
    print_console();
    start_cli();

    while (1)
    {
        heartbeat();
        // CLI commands are handled in cli_execute()
        microrl_insert_char(prl, cli_get_char());
        handle_door();
    }
}


/* System clock ISR */
ISR(TIMER5_COMPA_vect)
{
    system_time++;
}