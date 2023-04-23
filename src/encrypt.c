#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    if (originalMessage[i] == ' ') {
      fputs("-1 ", encryptedMessageFile);
      continue;
    }

    struct CharNode *charNode = charListSearch(charList, originalMessage[i]);

    if (!charNode)
      continue;

    char keyChar[64];
    sprintf(keyChar, "%d", charNode->keyList->head->value);

    fputs(keyChar, encryptedMessageFile);

    if (i != len - 1)
      putc(' ', encryptedMessageFile);
  }

  fclose(encryptedMessageFile);
}
