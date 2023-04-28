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
  printf("Reading Cipher Book: '%s'...\n", cipherBookPath);

  struct CharList *list = createCharList();

  FILE *book = fopen(cipherBookPath, "r");

  if (!book) {
    perror("It was not possible to open the file");
    exit(1);
  }

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
  printf("Reading Keys File: '%s'...\n", keysFilePath);

  struct CharList *list = createCharList();

  FILE *keys = fopen(keysFilePath, "r");

  if (!keys) {
    perror("It was not possible to open the file");
    exit(1);
  }

  int lastWasLinefeed = 1;
  char ch = getc(keys);
  int keyNumber;

  struct CharNode *currCharNode;
  struct KeyList *currKeyList;

  while (ch != EOF) {
    if (lastWasLinefeed) {
      currCharNode = charListInsert(list, ch);
      currKeyList = currCharNode->keyList;
    } else if (isdigit(ch)) {
      ungetc(ch, keys);
      fscanf(keys, "%d", &keyNumber);
      keyListInsert(currKeyList, keyNumber);
    }

    lastWasLinefeed = (ch == '\n') ? 1 : 0;

    ch = getc(keys);
  }

  fclose(keys);

  return list;
}
