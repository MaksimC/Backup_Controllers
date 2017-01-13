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
#include <string.h>
#include <avr/pgmspace.h>
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "hmi_msg.h"
#include "print_helper.h"
#include "cli_microrl.h"
#include "../lib/matejx_avr_lib/mfrc522.h"
#include "rfid.h"
#include "../lib/andy_brown_memdebug/memdebug.h"
#include <stdlib.h>

typedef struct cli_cmd
{
    PGM_P cmd;
    PGM_P help;
    void (*func_p)();
    const uint8_t func_argc;
} cli_cmd_t;

/*Array of structures, help commands, help messages, relevant function, number of arguments*/
const cli_cmd_t cli_cmds[] = {
    {help_cmd, help_help, cli_print_help, 0},
    {ver_cmd, ver_help, cli_print_ver, 0},
    {ascii_cmd, ascii_help, cli_print_ascii_tbls, 0},
    {month_cmd, month_help, cli_handle_month, 1},
    {read_cmd, read_help, cli_rfid_read, 0},
    {add_cmd, add_help, cli_rfid_add, 1},
    {print_cmd, print_help, cli_rfid_print, 0},
    {remove_cmd, remove_help, cli_rfid_remove, 1},
    {mem_cmd, mem_help, cli_mem_stat, 0}
};


void cli_print(const char *str)
{
    printf("%s", str);
}


/*Summary: non-blocking method receives input from CLI*/
char cli_get_char(void)
{
    if (uart0_peek() != UART_NO_DATA)
    {
        return uart0_getc() & UART_STATUS_MASK;
    }
    else
    {
        return 0x00;
    }
}


/*Summary: prints out list of CLI implemented commands, by iterating in structure's array cli_cmd_t */
void cli_print_help(const char *const *argv)
{
    (void) argv;
    printf_P(PSTR("\n"));
    printf_P(PSTR(CLI_HELP_MSG));

    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++)
    {
        printf_P(cli_cmds[i].cmd);
        printf_P(PSTR(" : "));
        printf_P(cli_cmds[i].help);
        printf_P(PSTR("\n"));
    }
}


/*Summary: Prints GIT info and translator version to console*/
void print_version(FILE *stream)
{
    //Print version to UART3 console
    fprintf_P(stream, PSTR(VER_FW),
    PSTR("GIT VERSION! Replace!"/*GIT_DESCR*/), PSTR(__DATE__), PSTR(__TIME__));
    fprintf_P(stream, PSTR(VER_LIBC " " VER_GCC "\n"),
    PSTR(__AVR_LIBC_VERSION_STRING__));
}


void cli_print_ver(const char *const *argv)
{
    (void) argv;
    printf_P(PSTR("\n"));
    print_version (stdout);
}


/*Summary: Prints ASCII tables to console*/
void cli_print_ascii_tbls(const char *const *argv)
{
    (void) argv;
    putc('\n', stdout);
    //Print ASCII table
    print_ascii_tbl(stdout);
    unsigned char charArray[128];

    for (unsigned char i = 0; i < sizeof(charArray); i++)
    {
        charArray[i] = i;
    }

    print_for_human(stdout, charArray, 128);
}


/*Summary: function compares string received from CLI input(second argument) with strings in Lookup list months, and if found,
prints relevant months names to console and to LCD.*/
void cli_handle_month(const char *const *argv)
{
    putc('\n', stdout);
    // Clear second line of display. Then goto beginning of line on display
    lcd_clr(0X40, 16);
    lcd_goto(0x40);

    // Compare the counted input chars with same amount of chars in strings in months[] and in case of success output month to LCD.
    for (int i = 0; i < 6; i++)
    {
        if (!strncmp_P(argv[1], (PGM_P)pgm_read_word(&months[i]), strlen(argv[1])))
        {
            fprintf_P(stdout, PSTR("%S\n"), (PGM_P)pgm_read_word(&months[i]));
            // Output month(s) to display
            lcd_puts_P((PGM_P)pgm_read_word(&months[i]));
            lcd_putc(' '); //Put space between months, if more than 1 matches
        }
    }
}


/*Summary: print error message, if CLI command is not found*/
void cli_print_cmd_error(void)
{
    printf_P(PSTR("\n"));
    printf_P(PSTR(CLI_MISSING_CMD_MSG));
}


/*Summary: print error message, if arguments for CLI command are missing or excessive*/
void cli_print_cmd_arg_error(void)
{
    printf_P(PSTR("\n"));
    printf_P(PSTR(CLI_ARGS_MSG));
}


/*Summary: methods polls the RFID reader, and if card detected, prints information of card to console*/
void cli_rfid_read(const char *const *argv)
{
    (void) argv;
    Uid uid;
    Uid *uid_ptr = &uid;
    if (PICC_IsNewCardPresent())
    {
        printf(CARD_SELECTED_MSG);
        PICC_ReadCardSerial(uid_ptr);
        printf_P(PSTR(CARD_UID_SIZE_MSG), uid.size);
        printf_P(PSTR(CARD_UID_SAK_MSG), uid.sak);
        printf_P(PSTR(CARD_UID_MSG));

        for (uint8_t i = 0; i < uid.size; i++)
        {
            printf("%02X", uid.uidByte[i]);
        }

        printf_P(PSTR("\n"));
    }
    else
    {
        printf_P((PSTR(CARD_UNABLE_TO_SELECT_MSG)));
    }
}


/*Summary: function polls the RFID reader constantly, and if card detected, stores card UID and UID size to card_t struct instance. 
  User is copied from CLI input. Validation of UID and add a new user happen in method rfid_add_user.*/
void cli_rfid_add(const char *const *argv)
{
    (void) argv;
    Uid uid;
    card_t card;
    if (PICC_IsNewCardPresent())
    {
        PICC_ReadCardSerial(&uid);
        card.uid_size = uid.size;
        memcpy(&card.uid, &uid.uidByte, uid.size);
        char *user = malloc(strlen(argv[1]) + 1);
        strcpy(user, argv[1]);
        card.user = user;
        rfid_add_card(&card);
        free(user);
    }
    else
    {
        printf_P(PSTR(CARD_UNABLE_TO_SELECT_MSG));
    }
}


/*Summary: read input from console and call frid_remove_card_by_uid function, sending argument copied from CLI 2nd word(argv[1]) input*/
void cli_rfid_remove(const char *const *argv)
{
    (void) argv;
    rfid_remove_card_by_uid(argv[1]);
}


/*Summary: read input from console and call cli_rfid_print function, sending argument copied from CLI 2nd word(argv[1]) input*/
void cli_rfid_print(const char *const *argv)
{
    (void) argv;
    rfid_print_card_list();
}


/*Summary: prints to console memory usage statistics and dynamics, using methods from "library andy_brown_memdebug"*/
void cli_mem_stat(const char *const *argv)
{
    (void) argv;
    extern int __heap_start, *__brkval;
    int v;
    int space;
    static int prev_space;
    space = (int) &v - (__brkval == 0 ? (int) & __heap_start : (int) __brkval);
    printf_P(PSTR(MEM_HEAPSTAT_MSG));
    printf_P(PSTR(MEM_USEDMEM_MSG), getMemoryUsed());
    printf_P(PSTR(MEM_FREEMEM_MSG), getFreeMemory());
    printf_P(PSTR(MEM_SPACE_HEAP_STACK_MSG));
    printf_P(PSTR(MEM_SPACE_CURRENT_MSG), space);
    printf_P(PSTR(MEM_SPACE_PREV_MSG), prev_space);
    printf_P(PSTR(MEM_SPACE_CHANGE_MSG), space - prev_space);
    printf_P(PSTR(MEM_FREELIST_MSG));
    printf_P(PSTR(MEM_FREELIST_SIZE_MSG), getFreeListSize());
    printf_P(PSTR(MEM_FREELIST_BLOCKS_MSG),
    getNumberOfBlocksInFreeList());
    printf_P(PSTR(MEM_LARGEST_BLOCK_MSG), getLargestBlockInFreeList());
    printf_P(PSTR(MEM_LARGEST_FL_BLOCK_MSG),
    getLargestAvailableMemoryBlock());
    printf_P(PSTR(MEM_LARGEST_ALL_BLOCK_MSG), getLargestNonFreeListBlock());
    prev_space = space;
}


/*Summary: Handles CLI inputs, validates it and calls relevant functions, connected to command names*/
int cli_execute(int argc, const char *const *argv)
{

    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++)
    {
        if (!strcmp_P(argv[0], cli_cmds[i].cmd))
        {
            // Test do we have correct arguments to run command
            // Function arguments count shall be defined in struct
            if ((argc - 1) != cli_cmds[i].func_argc)
            {
                cli_print_cmd_arg_error();
                return 0;
            }

            // Hand argv over to function pointer,
            // cross fingers and hope that function handles it properly
            cli_cmds[i].func_p (argv);
            return 0;
        }
    }

    cli_print_cmd_error();
    return 0;
}