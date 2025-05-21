#include <stdio.h>
#include <stdlib.h>
#include "../headers/endpoint_parse.h"

q_params *init_params(){
  q_params* params = malloc(sizeof(q_params));
  if (!params) {
    printf("Error: allocating params\n");
    return NULL;
  }
  params->size = 0;
  params->cap = INIT_CAP;
  params->keys = malloc(sizeof(char *) * params->cap);
  params->values = malloc(sizeof(char *) * params->cap);
  return params;
}

endpoint_t *endpoint_init(){
  endpoint_t *ep = malloc(sizeof(endpoint_t));
  if (!ep) {
    printf("Error: allocating ep\n");
    return NULL;
  }
  ep->handle = NULL;
  ep->params = init_params();
  return ep;
}

size_t len(char *endpoint){
  size_t count = 0;
  while(*endpoint++) count++;
  return count;
}

endpoint_t *parse_endpoint(char *url){
  enum State {
      STATE_PATH,
      STATE_QUERY_KEY,
      STATE_QUERY_VALUE,
      STATE_DONE
  } state;

  size_t size = len(url);
  char *buffer = malloc((size + 1) * sizeof(char));
  if (!buffer) {
    printf("Error: failed to allocate parse buffer\n");
    return NULL;
  }

  endpoint_t *e = endpoint_init();

  state = STATE_PATH;
  size_t buf_size = 0;

  //switch the state and copy from buffer
  char b;
  while((b = *url++)){
    buffer[buf_size++] = b;
    switch (state){
      case STATE_PATH:
        if (b == '?'){
          //overwrite ?
          buffer[buf_size - 1] = '\0';
          if (e->handle) {
            printf("multiple ? parses, error\n");
            return NULL;
          }
          e->handle = buf_copy(buffer);
          // printf("adding handle: %s\n", e->handle);
          state = STATE_QUERY_KEY;
          buf_size = 0;
        }
        break;

      case STATE_QUERY_KEY:
        if (b == '='){
          buffer[buf_size - 1] = '\0';
          e->params->keys[e->params->size] = buf_copy(buffer);
          // printf("adding key: %s\n", e->params->keys[e->params->size]);
          state = STATE_QUERY_VALUE;
          buf_size = 0; 
        }
        break;

      case STATE_QUERY_VALUE:
        if (b == '&'){
          buffer[buf_size - 1] = '\0';
          e->params->values[e->params->size++] = buf_copy(buffer);
          // printf("adding value: %s\n", e->params->values[e->params->size - 1]);
          state = STATE_QUERY_KEY;
          buf_size = 0; 
        }
        break;

      case STATE_DONE:
        break;
    }
  }

  if (state == STATE_QUERY_VALUE){
    buffer[buf_size] = '\0';
    e->params->values[e->params->size++] = buf_copy(buffer);
  } else if (state == STATE_PATH) {
    e->handle = buf_copy(buffer);
  }

  return e;
}

char *buf_copy(char *buf){
  int size = len(buf);
  char *ret = malloc((size + 1) * sizeof(char));
  if (!ret) {
    printf("Error: failed to allocate buf copy\n");
    return NULL;
  }
  char *src = ret;
  while((*src++ = *buf++));
  return ret;
}

void print_endpoint(endpoint_t *e){
  printf("Path: %s\n", e->handle);
  for (int i = 0; i < e->params->size; i++){
    printf("Key: %s | Value: %s\n", e->params->keys[i], e->params->values[i]);
  }
}

// int main(int argc, char** argv){
//   endpoint *e = parse_endpoint(argv[1]);
//   print_endpoint(e);
//   return 0;
// }
