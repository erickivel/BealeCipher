#ifndef BEALE_H
#define BEALE_H

void encrypt(char *cipherBookPath, char *originalMessageFilePath,
             char *encryptedMessagePath, char *keysFilePath);

void decrypt(char *encryptedMessage, char *cipherBookPath, char *keysListPath,
             char *decryptedMessagePath);

struct CharList *readCipherBook(char *cipherBookPath);

struct CharList *readKeysFile(char *keysFilePath);

void writeKeyListFile(struct CharList *list, char *keysFilePath);

#endif
