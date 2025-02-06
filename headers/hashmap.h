#define INIT_CAP 10

typedef struct _hashNode{
  struct _hashNode* next;
  char *key;
  void (*handler)(int);
} hashnode;

typedef struct {
  hashnode** nodes;
  int size;
} hashmap;

unsigned long hashString(char *str);
hashmap *init_map(void);
void free_hashmap(hashmap *hmap);
hashnode *init_hash_node(char *key, void (*func)(int));
void free_hashnode(hashnode *h);

hashnode* get_node(hashmap *hmap, char *key);

void add_node(hashmap *hmap, char *key, void (*func)(int));
