#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/beale.h"
#include "../include/list.h"

void encrypt(char *cipherBookPath, char *originalMessage,
             char *encryptedMessagePath, char *keysFilePath) {
  printf("Reading '%s'\n", cipherBookPath);
  struct CharList *charList = readCipherBook(cipherBookPath);

  printf("Writing '%s'\n", keysFilePath);
  writeKeyListFile(charList, keysFilePath);

  FILE *encryptedMessageFile = fopen(encryptedMessagePath, "w+");

  if (!encryptedMessageFile) {
    perror("It was not possible to open the file");
    exit(1);
  }

  int len = strlen(originalMessage);

  for (int i = 0; i < len; i++) {
    // Handle Space
    if (originalMessage[i] == ' ') {
      fputs("-1 ", encryptedMessageFile);
      continue;
    }

    struct CharNode *charNode = charListSearch(charList, originalMessage[i]);

    // Handle non-existing chars on the key list
    if (!charNode) {
      fputs("-2 ", encryptedMessageFile);
      continue;
    }

    struct KeyNode *keyNode = charNode->keyList->head;

    srand(clock());

    int keyIndex = rand() % charNode->keyList->size;

    char keyChar[64];
    for (int i = 0; i < keyIndex; i++) {
      keyNode = keyNode->next;
    }

    sprintf(keyChar, "%d", keyNode->value);

    fputs(keyChar, encryptedMessageFile);

    // Space between each number
    if (i != len - 1)
      putc(' ', encryptedMessageFile);
  }

  freeCharList(charList);
  fclose(encryptedMessageFile);
}
