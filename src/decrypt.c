#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/beale.h"
#include "../include/list.h"

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
    if (ch == '\n') {
      putc('\n', outFile);
      ch = getc(inFile);
      continue;
    }

    if (isdigit(ch) || ch == '-') {
      ungetc(ch, inFile);
      fscanf(inFile, "%d", &key);

      char decryptedChar;

      // If char doesn't exist on the keylist return '#'
      if (key == -1) {
        decryptedChar = ' ';
      } else if (key == -2) {
        decryptedChar = '#';
      } else {
        struct CharNode *charNode = keyListSearch(charList, key);
        decryptedChar = charNode ? charNode->value : '#';
      }

      putc(decryptedChar, outFile);
    }

    ch = getc(inFile);
  }

  freeCharList(charList);
  fclose(outFile);
  fclose(inFile);
}
