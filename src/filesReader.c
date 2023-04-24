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
  struct CharList *list = createCharList();

  FILE *keys = fopen(keysFilePath, "r");

  if (!keys) {
    perror("It was not possible to open the file");
    exit(1);
  }

  int lastWasLinefeed = 1;
  int lastWasSpace = 0;
  char ch = getc(keys);
  char numberStr[64] = "";

  struct CharNode *currCharNode;
  struct KeyList *currKeyList;

  while (ch != EOF) {
    if (lastWasLinefeed) {
      strcpy(numberStr, "");
      currCharNode = charListInsert(list, ch);
      currKeyList = currCharNode->keyList;
    } else if (lastWasSpace) {
      char chToStr[2] = "";
      chToStr[0] = ch;
      strcpy(numberStr, chToStr);
    } else if (strlen(numberStr) > 0) {

      if (ch == ' ') {
        keyListInsert(currKeyList, atoi(numberStr));
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
