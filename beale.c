#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "beale.h"
#include "list.h"

int wordCount = 0;

void addCharacter(struct CharList *list, char ch) {
  struct CharNode *charNode = charListSearch(list, ch);

  if (!charNode) {
    charNode = charListInsert(list, ch);
  }

  keyListInsert(charNode->keyList, wordCount);
  wordCount++;
}

struct CharList *readCipherBook(char *cipherBookPath) {
  struct CharList *list = createCharList();

  FILE *book = fopen(cipherBookPath, "r");

  int lastSpace = 1;
  char ch = getc(book);

  while (ch != EOF) {
    if (lastSpace && !isspace(ch) && !ispunct(ch)) {
      ch = tolower(ch);
      addCharacter(list, ch);
    }

    lastSpace = isspace(ch) ? 1 : 0;

    ch = getc(book);
  }

  fclose(book);
  return list;
}

struct CharList *readKeysFile(char *keysFilePath) {
  struct CharList *list = createCharList();

  FILE *keys = fopen(keysFilePath, "r");

  int lastWasLinefeed = 1;
  int lastWasSpace = 0;
  char ch = getc(keys);
  char *numberStr = "";

  while (ch != EOF) {
    if (lastWasLinefeed) {
      printf("\n%c: ", ch);
    }

    if (strlen(numberStr) != 0) {
      if (ch != ' ') {
        printf("%s ", numberStr);
        numberStr = "";
      } else {
        strcat(numberStr, &ch);
      }
    }

    if (lastWasSpace) {
      numberStr = &ch;
    }

    lastWasSpace = (ch == ' ') ? 1 : 0;
    lastWasLinefeed = (ch == '\n') ? 1 : 0;

    ch = getc(keys);
  }

  fclose(keys);

  return list;
}

void writeKeyListFile(struct CharList *list, char *keysFilePath) {
  FILE *keyFile = fopen(keysFilePath, "w+");

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

void encrypt(char *cipherBookPath, char *originalMessage,
             char *encryptedMessagePath, char *keysFilePath) {
  printf("Reading '%s'\n", cipherBookPath);
  struct CharList *charList = readCipherBook(cipherBookPath);

  printf("Writing '%s'\n", keysFilePath);
  writeKeyListFile(charList, keysFilePath);

  FILE *encryptedMessageFile = fopen(encryptedMessagePath, "w+");

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

void decrypt(char *encryptedMessage, char *cipherBookPath, char *keysListPath,
             char *decryptedMessagePath) {
  readKeysFile(keysListPath);
}
