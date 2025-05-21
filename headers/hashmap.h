#ifndef H_ENDPT_PARSE
#include "endpoint_parse.h"

#endif // !H_ENDPT_PARSE
//
#define INIT_CAP 10

typedef void (*endpoint_handler)(int, q_params*);

typedef struct _hashNode{
  struct _hashNode* next;
  char *key;
  endpoint_handler handler;
} hashnode;

typedef struct {
  hashnode** nodes;
  int size;
} hashmap;

unsigned long hashString(char *str);
hashmap *init_map(void);
void free_hashmap(hashmap *hmap);
hashnode *init_hash_node(char *key, endpoint_handler func);
void free_hashnode(hashnode *h);

hashnode* get_node(hashmap *hmap, char *key);

void add_node(hashmap *hmap, char *key, endpoint_handler func);
