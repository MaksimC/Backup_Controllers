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


#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "rfid.h"
#include "hmi_msg.h"

card_t *head = NULL;

/*Summary: function will receive pointer to card struct and print out its info to stdout*/
void rfid_print_card_info(const card_t *card)
{
    printf_P(PSTR(CARD_UID_BRACK_MSG1));
    printf("%d", card->uid_size);
    printf_P(PSTR(CARD_UID_BRACK_MSG2));

    for (uint8_t i = 0; i < card->uid_size; i++)
    {
        printf("%02X", card->uid[i]);
    }
    printf_P(PSTR(CARD_HOLDER_MSG));
    printf("%s", card->user);
    printf_P(PSTR("\n"));
}


/*Summary: function iterates through linked list and prints out all cards in the list using rfid_print_card_info*/
void rfid_print_card_list(void)
{
    if (head == NULL)
    {
        printf_P(PSTR(CARD_LIST_EMPTY_MSG));
    }
    else
    {
        card_t *current;
        current = head;
        uint8_t list_nr = 1;
        printf_P(PSTR(CARD_LIST_PRINT_MSG));
        while (current != NULL)
        {
            printf("%d", list_nr);

            rfid_print_card_info(current);

            list_nr ++;
            current = current->next;
        }
    }
}


/*Summary: function will check if scanned card(receives pointer to card struck as param) already in the list,
check memory and add card structure to linked list*/
void rfid_add_card(const card_t *card)
{
    card_t *found_card = rfid_find_card(card);
    if (found_card)
    {
        printf_P(PSTR(CARD_EXISTS_MSG));
        rfid_print_card_info(found_card);
        return;
    }

    // Create structure with new card, and check possibility to allocate memory for new card, in no space - give error to console
    card_t *new_card;
    char *new_card_user;
    new_card = malloc(sizeof(card_t));
    new_card_user = malloc(strlen(card->user) + 1);
    if (!new_card || !new_card_user)
    {
        printf_P(PSTR(CARD_LIST_FULL_MSG));
        return;
    }
    else
    {
        new_card->uid_size = card->uid_size;
        memcpy(new_card->uid, card->uid, card->uid_size);
        strcpy(new_card_user, card->user);
        new_card->user = new_card_user;
        new_card->next = NULL;
    }

    // Add card struct to linked list
    // if this is the first card in the list
    if (head == NULL)
    {
        head = new_card;
    }
    else
    {
        card_t *current;
        current = head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_card;
    }
    printf_P(PSTR(CARD_ADDING_MSG));

    rfid_print_card_info(new_card);

    return;
}


/*Summary: function will receive pointer to card struct as a parameter, iterate through linked list of cards,
and do comparison of username and memory areas of UID for each card in list with received card. Returns pointer to card, if found match*/
card_t* rfid_find_card(const card_t *card)
{
    if (head == NULL)
    {
        return NULL;
    }
    else
    {
        card_t *current;
        current = head;
        while (current != NULL)
        {

            if (!memcmp(current->uid, card->uid, current->uid_size))
            {
                return current;
            }

            current = current->next;
        }
    }
    return NULL;
}


/*Summary: function convert input from console into char array. Iterate through linked list of cards, convert UID to hex and concatenate
into char array. Console array compared to array of the card. If match - linked list will adjusted and matched card removed from memory.*/
void rfid_remove_card_by_uid(const char *uid)
{

    card_t *current;
    card_t *prev;
    current = head;
    prev = NULL;
    char cli_buf[10];
    char uid_hex_buf[10];
    char uid_buf[10];

    sprintf(cli_buf, "%s", uid);

    if (head == NULL)
    {
        printf_P(PSTR(CARD_LIST_EMPTY_MSG));
        return;
    }
    else
    {
        while (current != NULL)
        {
            strcpy(uid_hex_buf, "");
            strcpy(uid_buf, "");
            for (uint8_t i = 0; i < current->uid_size; i++)
            {
                sprintf(uid_hex_buf,"%X", current->uid[i]);
                strcat(uid_buf, uid_hex_buf);
            }

            if (strcmp(cli_buf, uid_buf) == 0)
            {
                if(prev == NULL)
                // Special condition when first item is removed
                // No need to change next value. Just point head to next item
                {
                    head = current->next;
                    printf_P(PSTR(CARD_REMOVING_MSG));
                    printf("%s", uid_buf);
                    printf_P(PSTR(CARD_HOLDER_MSG));
                    printf("%s", current->user);
                    printf_P(PSTR("\n"));
                }
                else
                {
                    prev->next = current->next;
                    printf_P(PSTR(CARD_REMOVING_MSG));
                    printf("%s", uid_buf);
                    printf_P(PSTR(CARD_HOLDER_MSG));
                    printf("%s", current->user);
                    printf_P(PSTR("\n"));
                }
                free(current);
                return;
            }
            prev = current;
            current = current->next;
        }
    }
    printf_P(PSTR(CARD_NOT_FOUND_MSG));
}