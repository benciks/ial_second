/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
  // Set all pointers in array to NULL
  for (int i = 0; i < HT_SIZE; i++) {
    (*table)[i] = NULL;
  }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  // Iterate over the table
  for (int i = 0; i < HT_SIZE; i++) {
    ht_item_t *item = (*table)[i];
    // Iterate over synonyms
    while (item) {
      // Return if key matches
      if (strcmp(item->key, key) == 0) {
        return item;
      }
      item = item->next;
    }
  }
  // Return NULL if no match found
  return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  // Find the item
  ht_item_t *item = ht_search(table, key);

  // Replace the value if item exists
  if (item) {
    item->value = value;
  } else {
    // Get the hash of the key
    int hash = get_hash(key);
    ht_item_t *new_item = malloc(sizeof(ht_item_t));

    // Set the key, value and pointer to the next item
    new_item->key = key;
    new_item->value = value;
    new_item->next = (*table)[hash];

    // Set the table pointer to the new item
    (*table)[hash] = new_item;
  }
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  // Return the value of the item if it exists
  ht_item_t *item = ht_search(table, key);
  if (item) {
    return &item->value;
  }

  // Return NULL if the item does not exist
  return NULL;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  // Iterate over the table
  for (int i = 0; i < HT_SIZE; i++) {
    ht_item_t *item = (*table)[i];
    ht_item_t *previous = NULL;

    // Iterate over synonyms
    while (item) {
      // If item is found and there is a previous item,
      // set it's next pointer to deleted item's next pointer
      if (strcmp(item->key, key) == 0) {
        if (previous) {
          previous->next = item->next;
        } else {
          (*table)[i] = item->next;
        }
        free(item);
        return;
      }
      previous = item;
      item = item->next;
    }
  }
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
  // Iterate over the table and synonyms and free each one.
  for (int i = 0; i < HT_SIZE; i++) {
    ht_item_t *item = (*table)[i];
    while (item) {
      ht_item_t *next = item->next;
      free(item);
      item = next;
    }
    (*table)[i] = NULL;
  }
}
