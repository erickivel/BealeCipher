#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void printList(struct CharList *list) {
  struct CharNode *charNode = list->head;
  while (charNode != NULL) {
    printf("%c: ", charNode->value);
    struct KeyNode *keyNode = charNode->keyList->head;

    while (keyNode != NULL) {
      printf("%d ", keyNode->value);
      keyNode = keyNode->next;
    }

    charNode = charNode->next;
    printf("\n");
  }
}

void writeKeyListFile(struct CharList *list) {
  FILE *keyFile = fopen("keyList.txt", "w+");

  struct CharNode *charNode = list->head;
  while (charNode != NULL) {
    fputs(strncat(&charNode->value, ": ", 3), keyFile);
    struct KeyNode *keyNode = charNode->keyList->head;

    while (keyNode != NULL) {
      char keyChar[64];
      sprintf(keyChar, "%d ", keyNode->value);
      fputs(keyChar, keyFile);
      // printf("%d ", keyNode->value);
      keyNode = keyNode->next;
    }

    charNode = charNode->next;
    fputs("\n", keyFile);
  }

  fclose(keyFile);
}

void readCipherBook() {
  struct CharList *list = createCharList();

  FILE *book = fopen("cipherBook.txt", "r");

  int lastSpace = 1;
  char ch = getc(book);
  while (ch != EOF) {
    if (lastSpace && !isspace(ch) && !ispunct(ch)) {
      ch = tolower(ch);
      addCharacter(list, ch);
    }

    if (isspace(ch)) {
      lastSpace = 1;
    } else {
      lastSpace = 0;
    }
    ch = getc(book);
  }

  fclose(book);
  printList(list);
  writeKeyListFile(list);
}

int main() {
  readCipherBook();

  return 0;
}
