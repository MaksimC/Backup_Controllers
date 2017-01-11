#ifndef _RFID_H_ /* Header Guard */
#define _RFID_H_ /* Header Guard */

//Defining new type card_t, struct.
typedef struct card {
    uint8_t uid_size;   //size
    uint8_t uid[10];    //UID
    char *user;         //user
    struct card *next;  //link to next element in linked list
} card_t;


extern card_t *head;

extern void rfid_print_card_info(const card_t *card);
extern void rfid_print_card_list(void);
extern void rfid_add_card(const card_t *card);
extern card_t* rfid_find_card(const card_t *card);
extern void rfid_remove_card_by_uid(const char *uid);

#endif /* Header Guard */