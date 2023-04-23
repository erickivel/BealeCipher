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

int main(int argc, char *argv[]) {
  if (argc > 10) {
    printf("Too much arguments\n");
    exit(1);
  }

  if (argc < 8) {
    printf("Too few arguments\n");
    exit(1);
  }

  if (!strcmp(argv[1], "-e")) {
    printf("ENCRYPT:\n");

    char *cipherBookPath = "";
    char *originalMessage = "";
    char *encryptedMessagePath = "";
    char *keysFilePath = "";

    for (int i = 2; i < argc; i++) {
      if (!strcmp("-b", argv[i])) {
        if (strlen(cipherBookPath) != 0) {
          printf("Only one '-b' tag allowed\n");
          exit(1);
        }
        cipherBookPath = argv[i + 1];
        i++;
      } else if (!strcmp("-m", argv[i])) {
        if (strlen(originalMessage) != 0) {
          printf("Only one '-m' tag allowed\n");
          exit(1);
        }
        originalMessage = argv[i + 1];
        i++;
      } else if (!strcmp("-o", argv[i])) {
        if (strlen(encryptedMessagePath) != 0) {
          printf("Only one '-o' tag allowed\n");
          exit(1);
        }
        encryptedMessagePath = argv[i + 1];
        i++;
      } else if (!strcmp("-c", argv[i])) {
        if (strlen(keysFilePath) != 0) {
          printf("Only one '-c' tag allowed\n");
          exit(1);
        }
        keysFilePath = argv[i + 1];
        i++;
      } else {
        printf("Invalid '%s' tag\n", argv[i]);
        exit(1);
      }
    }

    encrypt(cipherBookPath, originalMessage, encryptedMessagePath,
            keysFilePath);
  } else if (!strcmp(argv[1], "-d")) {
    printf("DECRYPT\n");

    char *encryptedMessage = "";
    char *cipherBookPath = "";
    char *keysFilePath = "";
    char *decryptedMessagePath = "";

    for (int i = 2; i < argc; i++) {
      if (!strcmp("-i", argv[i])) {
        if (strlen(encryptedMessage) != 0) {
          printf("Only one '-i' tag allowed\n");
          exit(1);
        }
        encryptedMessage = argv[i + 1];
        i++;
      } else if (!strcmp("-b", argv[i])) {
        if (strlen(cipherBookPath) != 0) {
          printf("Only one '-b' tag allowed\n");
          exit(1);
        }
        cipherBookPath = argv[i + 1];
        i++;
      } else if (!strcmp("-c", argv[i])) {
        if (strlen(keysFilePath) != 0) {
          printf("Only one '-c' tag allowed\n");
          exit(1);
        }
        keysFilePath = argv[i + 1];
        i++;
      } else if (!strcmp("-o", argv[i])) {
        if (strlen(decryptedMessagePath) != 0) {
          printf("Only one '-o' tag allowed\n");
          exit(1);
        }
        decryptedMessagePath = argv[i + 1];
        i++;
      } else {
        printf("Invalid '%s' tag\n", argv[i]);
        exit(1);
      }
    }

    decrypt(encryptedMessage, cipherBookPath, keysFilePath,
            decryptedMessagePath);
  } else {
    printf("Invalid '%s' tag", argv[1]);
    exit(1);
  }

  return 0;
}
