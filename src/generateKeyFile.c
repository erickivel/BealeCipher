#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/beale.h"
#include "../include/list.h"

void writeKeyListFile(struct CharList *list, char *keysFilePath) {
  FILE *keyFile = fopen(keysFilePath, "w");

  if (!keyFile) {
    perror("It was not possible to open the file");
    exit(1);
  }

  struct CharNode *charNode = list->head;
  while (charNode) {
    fprintf(keyFile, "%c: ", charNode->value);
    struct KeyNode *keyNode = charNode->keyList->head;

    while (keyNode) {
      fprintf(keyFile, "%d ", keyNode->value);
      keyNode = keyNode->next;
    }

    charNode = charNode->next;
    fputs("\n", keyFile);
  }

  fclose(keyFile);
}
