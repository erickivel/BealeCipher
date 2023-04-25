#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/beale.h"
#include "../include/list.h"

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
    char *originalMessageFilePath = "";
    char *encryptedMessagePath = "";
    char *keysFilePath = "";

    for (int i = 2; i < argc; i++) {
      if (!strcmp("-b", argv[i])) {
        if (strlen(cipherBookPath) != 0) {
          printf("Only one '-b' flag allowed\n");
          exit(1);
        }
        cipherBookPath = argv[i + 1];
        i++;
      } else if (!strcmp("-m", argv[i])) {
        if (strlen(originalMessageFilePath) != 0) {
          printf("Only one '-m' flag allowed\n");
          exit(1);
        }
        originalMessageFilePath = argv[i + 1];
        i++;
      } else if (!strcmp("-o", argv[i])) {
        if (strlen(encryptedMessagePath) != 0) {
          printf("Only one '-o' flag allowed\n");
          exit(1);
        }
        encryptedMessagePath = argv[i + 1];
        i++;
      } else if (!strcmp("-c", argv[i])) {
        if (strlen(keysFilePath) != 0) {
          printf("Only one '-c' flag allowed\n");
          exit(1);
        }
        keysFilePath = argv[i + 1];
        i++;
      } else {
        printf("Invalid '%s' flag\n", argv[i]);
        exit(1);
      }
    }

    encrypt(cipherBookPath, originalMessageFilePath, encryptedMessagePath,
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
          printf("Only one '-i' flag allowed\n");
          exit(1);
        }
        encryptedMessage = argv[i + 1];
        i++;
      } else if (!strcmp("-b", argv[i])) {
        if (strlen(cipherBookPath) != 0) {
          printf("Only one '-b' flag allowed\n");
          exit(1);
        }
        cipherBookPath = argv[i + 1];
        i++;
      } else if (!strcmp("-c", argv[i])) {
        if (strlen(keysFilePath) != 0) {
          printf("Only one '-c' flag allowed\n");
          exit(1);
        }
        keysFilePath = argv[i + 1];
        i++;
      } else if (!strcmp("-o", argv[i])) {
        if (strlen(decryptedMessagePath) != 0) {
          printf("Only one '-o' flag allowed\n");
          exit(1);
        }
        decryptedMessagePath = argv[i + 1];
        i++;
      } else {
        printf("Invalid '%s' flag\n", argv[i]);
        exit(1);
      }
    }

    decrypt(encryptedMessage, cipherBookPath, keysFilePath,
            decryptedMessagePath);
  } else {
    printf("Invalid '%s' flag", argv[1]);
    exit(1);
  }

  return 0;
}
