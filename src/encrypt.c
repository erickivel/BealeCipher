#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/beale.h"
#include "../include/list.h"

void encrypt(char *cipherBookPath, char *originalMessageFilePath,
             char *encryptedMessagePath, char *keysFilePath) {
  printf("Reading '%s'\n", cipherBookPath);
  struct CharList *charList = readCipherBook(cipherBookPath);

  printf("Writing '%s'\n", keysFilePath);
  writeKeyListFile(charList, keysFilePath);

  FILE *encryptedMessageFile = fopen(encryptedMessagePath, "w");
  FILE *originalMessageFile = fopen(originalMessageFilePath, "r");

  if (!encryptedMessageFile || !originalMessageFile) {
    perror("It was not possible to open the file");
    exit(1);
  }

  int ch = getc(originalMessageFile);

  while (ch != EOF) {
    // Handle Space
    if (isspace(ch)) {
      if (ch == '\n')
        putc('\n', encryptedMessageFile);
      else
        fputs("-1 ", encryptedMessageFile);
      ch = getc(originalMessageFile);
      continue;
    }

    struct CharNode *charNode = charListSearch(charList, tolower(ch));

    if (isspace(ch))
      printf("%c: %d\n", ch, isalnum(ch));

    // Handle non-existing chars on the key list
    if (!charNode) {
      fputs("-2 ", encryptedMessageFile);
      ch = getc(originalMessageFile);
      continue;
    }

    struct KeyNode *keyNode = charNode->keyList->head;

    // Get random key
    srand(clock());

    int keyIndex = rand() % charNode->keyList->size;

    for (int i = 0; i < keyIndex; i++) {
      keyNode = keyNode->next;
    }

    fprintf(encryptedMessageFile, "%d ", keyNode->value);

    ch = getc(originalMessageFile);
  }

  freeCharList(charList);
  fclose(encryptedMessageFile);
  fclose(originalMessageFile);
}
