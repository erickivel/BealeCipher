#define BEALE_H

void encrypt(char *cipherBookPath, char *originalMessage,
             char *encryptedMessagePath, char *keysFilePath);

void decrypt(char *encryptedMessage, char *cipherBookPath, char *keysListPath,
             char *decryptedMessagePath);
