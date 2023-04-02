struct KeyNode {
  int value;
  struct KeyNode *next;
};

struct KeyList {
  struct KeyNode *head;
  int size;
};

struct CharNode {
  char value;
  struct KeyList *keyList;
  struct CharNode *next;
};

struct CharList {
  struct CharNode *head;
};

struct CharList *createCharList();

struct KeyList *createKeyList();

struct CharNode *charListInsert(struct CharList *list, char charValue);

struct CharNode *charListSearch(struct CharList *list, char charValue);

void keyListInsert(struct KeyList *list, int keyValue);

struct CharNode *keyListSearch(struct CharList *list, int keyValue);
