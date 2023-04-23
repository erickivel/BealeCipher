#include <stdio.h>
#include <stdlib.h>

#include "../include/list.h"

struct CharList *createCharList() {
  struct CharList *newCharList = malloc(sizeof(struct CharList));

  if (!newCharList) {
    printf("Error when creating char list");
    exit(1);
  }

  newCharList->head = NULL;
  return newCharList;
}

struct KeyList *createKeyList() {
  struct KeyList *newKeyList = malloc(sizeof(struct KeyList));

  if (!newKeyList) {
    printf("Error when creating key list");
    exit(1);
  }

  newKeyList->head = NULL;
  newKeyList->size = 0;
  return newKeyList;
}

struct CharNode *createCharNode(char charValue) {
  struct CharNode *newCharNode = malloc(sizeof(struct CharNode));

  if (!newCharNode) {
    printf("Error when creating a char node");
    exit(1);
  }

  newCharNode->value = charValue;
  newCharNode->keyList = createKeyList();
  newCharNode->next = NULL;
  return newCharNode;
}

struct KeyNode *createKeyNode(int keyValue) {
  struct KeyNode *newKeyNode = malloc(sizeof(struct KeyNode));

  if (!newKeyNode) {
    printf("Error when creating a key node");
    exit(1);
  }

  newKeyNode->next = NULL;
  newKeyNode->value = keyValue;
  return newKeyNode;
}

struct CharNode *charListInsert(struct CharList *list, char charValue) {
  struct CharNode *newCharNode = createCharNode(charValue);
  struct CharNode *lastNode = list->head;

  // Head insertion
  if (!lastNode) {
    list->head = newCharNode;
    return newCharNode;
  }

  // Head insertion
  if (lastNode->value > newCharNode->value) {
    struct CharNode *tempNode = lastNode;
    list->head = newCharNode;
    newCharNode->next = tempNode;
    return newCharNode;
  }

  while (lastNode) {
    // Tail insertion
    if (!lastNode->next) {
      lastNode->next = newCharNode;
      return newCharNode;
    }

    // Middle Insertion
    if (newCharNode->value > lastNode->value &&
        newCharNode->value < lastNode->next->value) {
      struct CharNode *oldNode = lastNode->next;
      lastNode->next = newCharNode;
      newCharNode->next = oldNode;
      return newCharNode;
    }

    lastNode = lastNode->next;
  }

  return newCharNode;
}

struct CharNode *charListSearch(struct CharList *list, char charValue) {
  struct CharNode *currentCharNode = list->head;

  while (currentCharNode != NULL && currentCharNode->value != charValue) {
    currentCharNode = currentCharNode->next;
  }

  return currentCharNode;
}

void keyListInsert(struct KeyList *list, int keyValue) {
  struct KeyNode *newKeyNode = createKeyNode(keyValue);
  struct KeyNode *lastHead = list->head;
  list->head = newKeyNode;
  newKeyNode->next = lastHead;
  list->size++;
}

struct CharNode *keyListSearch(struct CharList *list, int keyValue) {
  struct CharNode *currentCharNode = list->head;

  while (currentCharNode != NULL) {
    struct KeyNode *keyNode = currentCharNode->keyList->head;

    while (keyNode != NULL && keyNode->value != keyValue) {
      keyNode = keyNode->next;
    }

    if (keyNode && keyNode->value == keyValue) {
      return currentCharNode;
    }

    currentCharNode = currentCharNode->next;
  }

  return NULL;
}
