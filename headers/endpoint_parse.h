#ifndef H_ENDPT_PARSE
#define H_ENDPT_PARSE

#include <stddef.h>
#define INIT_CAP 10

typedef struct {
  char **keys;
  char **values;
  size_t size;
  size_t cap;
} q_params;

typedef struct {
  char *handle;
  //char *req_type; todo: add req type and take in full msg
  q_params* params;
} endpoint_t;

endpoint_t *parse_endpoint(char *endpoint);
char *buf_copy(char *buf);
size_t len(char *endpoint);
endpoint_t *endpoint_init();
q_params *init_params();
void print_endpoint(endpoint_t *e);
void free_endpoint(endpoint_t *e);

#endif // H_ENDPT_PARSE
