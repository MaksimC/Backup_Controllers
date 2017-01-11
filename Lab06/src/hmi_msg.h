#ifndef _HMI_MSG_H_ /* Header Guard */
#define _HMI_MSG_H_ /* Header Guard */


#define HELP_CMD "help"
#define HELP_HELP "Get help"

#define VER_CMD "version"
#define VER_HELP "Print FW version"

#define ASCII_CMD "ascii"
#define ASCII_HELP "print ASCII tables"

#define MONTH_CMD "month"
#define MONTH_HELP "Find matching month from lookup list. Usage: month <string>"

#define READ_CMD "read"
#define READ_HELP "Read Mifare card and print Card ID"

#define ADD_CMD "add"
#define ADD_HELP "Add Micare card to list. Usage: add <card holder name>"

#define REMOVE_CMD "rm"
#define REMOVE_HELP "Remove Mifare card from list Usage: rm <card uid>"

#define PRINT_CMD "print"
#define PRINT_HELP "Print stored access card list"

#define MEM_CMD "mem"
#define MEM_HELP "Print memory usage and change compared to previous call"

#define VER_GCC "avr-gcc version: " __VERSION__
#define VER_FW "Version: %S built on: %S %S\n"
#define VER_LIBC "avr-libc version: %S"
#define PROMPT_FIRST_LETTER "Enter month's first letter >"
#define LBL_UPTIME "Uptime"
#define STUD_NAME "Maksim T?eljabov"

#define CLI_START_MSG "Use backspace to delete entry and enter to confirm.\nArrow keys and del do not work currently.\nUse Help command to learn about commands"
#define CLI_HELP_MSG "Implemented commands:\n"
#define CLI_MISSING_CMD_MSG "Command not implemented.\nUse <help> to get help.\n"
#define CLI_ARGS_MSG "To few or to many arguments for this command\nUse <help>\n"

#define CARD_ACCESS_DENIED_MSG "Access denied!"
#define CARD_LIST_FULL_MSG "\nCards list is full. You have to remove some cards, before you can add new ones.\n"
#define CARD_EXISTS_MSG "\nScanned card alrady exists in the list. Please scan another card!\n1"
#define CARD_LIST_PRINT_MSG "\nAccess cards in list:\n"
#define CARD_LIST_EMPTY_MSG "\nCard list is empty.\n"
#define CARD_UNABLE_TO_SELECT_MSG "\nUnable to select card.\n"
#define CARD_ADDING_MSG "\nAdded card"
#define CARD_NOT_FOUND_MSG "\nCard not found in the list. Please try again.\n"
#define CARD_SELECTED_MSG "\nCard selected!"
#define CARD_UID_SIZE_MSG "\nUID size: 0x%02X\n"
#define CARD_UID_SAK_MSG "UID sak: 0x%02X\n"
#define CARD_UID_MSG "Card UID: "
#define CARD_REMOVING_MSG "\nRemoving card UID: "
#define CARD_HOLDER_MSG " holder "
#define CARD_UID_BRACK_MSG1 ". UID["
#define CARD_UID_BRACK_MSG2 "]: "

#define MEM_HEAPSTAT_MSG "\nHeap statistics\n"
#define MEM_USEDMEM_MSG "Used: %d\n"
#define MEM_FREEMEM_MSG "Free: %d\n"
#define MEM_SPACE_HEAP_STACK_MSG "\nSpace between stack and heap:\n"
#define MEM_SPACE_CURRENT_MSG "Current  %d\n"
#define MEM_SPACE_PREV_MSG  "Previous %d\n"
#define MEM_SPACE_CHANGE_MSG "Change   %d\n"
#define MEM_FREELIST_MSG "\nFreelist\n"
#define MEM_FREELIST_SIZE_MSG "Freelist size: %d\n"
#define MEM_FREELIST_BLOCKS_MSG "Blocks in freelist:        %d\n"
#define MEM_LARGEST_BLOCK_MSG "Largest block in freelist: %d\n"
#define MEM_LARGEST_FL_BLOCK_MSG "Largest freelist block:    %d\n"
#define MEM_LARGEST_ALL_BLOCK_MSG "Largest allocable block:   %d\n"

extern PGM_P const months[];

extern const char help_cmd[];
extern const char help_help[];
extern const char ver_cmd[];
extern const char ver_help[];
extern const char ascii_cmd[];
extern const char ascii_help[];
extern const char month_cmd[];
extern const char month_help[];
extern const char read_cmd[];
extern const char read_help[];
extern const char add_cmd[];
extern const char add_help[];
extern const char remove_cmd[];
extern const char remove_help[];
extern const char print_cmd[];
extern const char print_help[];
extern const char access_denied_msg[];
extern const char mem_cmd[];
extern const char mem_help[];

#endif /* Header Guard */