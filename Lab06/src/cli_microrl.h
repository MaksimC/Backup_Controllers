#ifndef _CLI_MICRORL_H_  // header guard
#define _CLI_MICRORL_H_  // header guard

#define NUM_ELEMS(x)        (sizeof(x) / sizeof((x)[0]))
#define UART_STATUS_MASK    0x00FF

void cli_print(const char * str);
char cli_get_char(void);
void cli_print_help(const char *const *argv);
void cli_print_ver(const char *const *argv);
void print_version(FILE *stream);
void cli_print_ascii_tbls(const char *const *argv);
void cli_handle_month(const char *const *argv);
void cli_print_cmd_error(void);
void cli_print_cmd_arg_error(void);
void cli_rfid_read(const char *const *argv);
void cli_rfid_add(const char *const *argv);
void cli_rfid_remove(const char *const *argv);
void cli_rfid_print(const char *const *argv);
void cli_mem_stat(const char *const *argv);
int cli_execute(int argc, const char *const *argv);



#endif // header guard