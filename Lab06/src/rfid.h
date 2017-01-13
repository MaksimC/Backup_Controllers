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