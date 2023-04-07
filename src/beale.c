#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/beale.h"
#include "../include/list.h"

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
  char numberStr[64] = "";

  struct CharNode *currCharNode;
  struct KeyList *currKeyList;

  while (ch != EOF) {
    if (lastWasLinefeed) {
      strcpy(numberStr, "");
      // printf("\n%c: ", ch);
      currCharNode = charListInsert(list, ch);
      currKeyList = currCharNode->keyList;
    } else if (lastWasSpace) {
      char chToStr[2] = "";
      chToStr[0] = ch;
      strcpy(numberStr, chToStr);
    } else if (strlen(numberStr) > 0) {

      if (ch == ' ') {
        keyListInsert(currKeyList, atoi(numberStr));
        // printf("%s ", numberStr);
        strcpy(numberStr, "");
      } else {
        char chToStr[2] = "";
        chToStr[0] = ch;
        strcat(numberStr, chToStr);
      }
    }

    lastWasSpace = (ch == ' ') ? 1 : 0;
    lastWasLinefeed = (ch == '\n') ? 1 : 0;

    ch = getc(keys);
  }

  fclose(keys);

  return list;
}

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

  struct CharList *charList;

  if (strlen(keysListPath) > 0) {
    charList = readKeysFile(keysListPath);
  } else if (strlen(cipherBookPath) > 0) {
    charList = readCipherBook(cipherBookPath);
  } else {
    printf("Neither the cipher book or key list was provided");
    exit(1);
  }

  FILE *outFile = fopen(decryptedMessagePath, "w");
  FILE *inFile = fopen(encryptedMessage, "r");

  if (!outFile || !inFile) {
    perror("It was not possible to open the file");
    exit(1);
  }

  int key;
  char ch = getc(inFile);

  while (ch != EOF) {
    if (isdigit(ch) || ch == '-') {
      ungetc(ch, inFile);
      fscanf(inFile, "%d", &key);

      char decryptedChar;

      if (key == -1) {
        decryptedChar = ' ';
      } else {
        decryptedChar = keyListSearch(charList, key)->value;
        if (!decryptedChar) {
          decryptedChar = '-';
        }
      }

      putc(decryptedChar, outFile);
    }

    ch = getc(inFile);
  }

  fclose(outFile);
  fclose(inFile);
}
