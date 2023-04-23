#include <stdio.h>
#include <string.h>

#include "../include/beale.h"
#include "../include/list.h"

void writeKeyListFile(struct CharList *list, char *keysFilePath) {
  FILE *keyFile = fopen(keysFilePath, "w");

  struct CharNode *charNode = list->head;
  while (charNode != NULL) {
    fputs(strncat(&charNode->value, ": ", 3), keyFile);
    struct KeyNode *keyNode = charNode->keyList->head;

    while (keyNode != NULL) {
      char keyCh[64];
      sprintf(keyCh, "%d ", keyNode->value);
      fputs(keyCh, keyFile);
      keyNode = keyNode->next;
    }

    charNode = charNode->next;
    fputs("\n", keyFile);
  }

  fclose(keyFile);
}
