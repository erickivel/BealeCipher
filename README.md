=== Beale Ciphers ===

* Program Execution Flow
  The main function is on beale.c where it receives the program arguments, verifies if the number of arguments is correct,
  if the flags provided are valid, if the flags aren't duplicated, then execute the 'encrypt' or 'decrypt' function.

  - Encrypt:
    beale.c -> encrypt.c -> filesReader.c (read Cipher Book) -> writeKeyListFile.c -> list.c (search characters to find the encrypted keys) -> write output file (Encrypted Message).
  - Decrypt:
    beale.c -> decrypt.c -> filesReader.c (read Cipher Book or Keys File) -> list.c (search numbers/keys to find the decrypted characters) -> write output file (Decrypted Message).

* Main Data Structures
  The main data structure is declared on list.h, it consists of a linked list of characters (struct CharList)
  where each character node (struct CharNode) has its own linked list of keys/numbers (struct KeyList),
  this KeyList is made of key nodes (struct KeyNode).


* Main Algorithms
  - readCipherBook: 
    Implemented on filesReader.c its main function is to translate the Cipher Book to a struct CharList. 
    To accomplish that the function reads character by character of the Cipher book, when a space (' ') is identified
    it means that a new word begins, so it registers the first letter of it on a struct CharNode (if it's not created yet) 
    and insert this CharNode on the CharList with the number of the current word on the CharNode's KeyList. In the end,
    it returns the struct CharList assembled.

  - readKeysFile: 
    Implemented on filesReader.c its main function is to translate the Keys File to a struct CharList. 
    To accomplish that the function reads character by character of the keys file, when a new line ('\n') is identified
    it means the next one is a character to insert in the CharList. If the current character is a digit, the function reads it 
    formatted as a number and then inserts it on the current CharNode's KeyList. In the end, it returns the struct CharList assembled.

  - writeKeyListFile
    Implemented on generateKeyFile.c it receives the CharList assembled on the filesReader.c and prints it on the output keys file.

  - encrypt: 
    Implemented on encrypt.c it reads the Original Message file character by character and searches each one on the CharList 
    (that returns a CharNode) to get the correspondent encrypted key/number the function gets a random key on KeyList of the
    returned CharNode, then writes the obtained key on the output file (Encrypted Message). If the scanned character is a 
    space (' ') the function writes -1, if the character doesn't exist on the CharList the function writes -2, and if the 
    character is a new line ('\n') the function adds a new line on the output file too.

  - decrypt: 
    Implemented on decrypt.c it reads the Encrypted Message file character by character and if a digit is found it reads formatted 
    the number and searches it on the CharList to obtain the decrypted character, then the function writes it on the output file 
    (Decrypted Message). If the scanned number is -1 the function writes a space (' '), if the number is -2 (the original message 
    character didn't exist on the CharList) the function writes '#', and if the scanned character is a new line ('\n') the function 
    adds a new line on the output file too.
