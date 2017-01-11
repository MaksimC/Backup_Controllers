#include <avr/pgmspace.h>
#include "hmi_msg.h"

static const char string_1[] PROGMEM = "January";
static const char string_2[] PROGMEM = "February";
static const char string_3[] PROGMEM = "March";
static const char string_4[] PROGMEM = "April";
static const char string_5[] PROGMEM = "May";
static const char string_6[] PROGMEM = "June";

PGM_P const months[] PROGMEM = {
    string_1,
    string_2,
    string_3,
    string_4,
    string_5,
    string_6
};

const char help_cmd[] PROGMEM = HELP_CMD;
const char help_help[] PROGMEM = HELP_HELP;
const char ver_cmd[] PROGMEM = VER_CMD;
const char ver_help[] PROGMEM = VER_HELP;
const char ascii_cmd[] PROGMEM = ASCII_CMD;
const char ascii_help[] PROGMEM = ASCII_HELP;
const char month_cmd[] PROGMEM = MONTH_CMD;
const char month_help[] PROGMEM = MONTH_HELP;
const char read_cmd[] PROGMEM = READ_CMD;
const char read_help[] PROGMEM = READ_HELP;
const char add_cmd[] PROGMEM = ADD_CMD;
const char add_help[] PROGMEM = ADD_HELP;
const char remove_cmd[] PROGMEM = REMOVE_CMD;
const char remove_help[] PROGMEM = REMOVE_HELP;
const char print_cmd[] PROGMEM = PRINT_CMD;
const char print_help[] PROGMEM = PRINT_HELP;
const char access_denied_msg[] PROGMEM = CARD_ACCESS_DENIED_MSG;
const char mem_cmd[] PROGMEM = MEM_CMD;
const char mem_help[] PROGMEM = MEM_HELP;