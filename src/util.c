#include "../headers/util.h"
#include "../headers/str.h"

void exit_with_error(char *msg){
  perror(msg);
  exit(EXIT_FAILURE);
}

void check_failure(int retcode){
  if(retcode > 0){
    perror("failed");
    exit(retcode);
  }
}

void readfile(char* file_name, char* mode, char** buffer, int* buf_size){
  FILE* fptr = fopen(file_name, mode);
  if (fptr == NULL){
    printf("fptr is NULL\n");
    exit(1);
  }

  check_failure(fseek(fptr, 0L, SEEK_END));
  *buf_size = ftell(fptr);
  rewind(fptr);

  *buffer = malloc(sizeof(char) * (*buf_size + 1));

  size_t read = fread(*buffer, sizeof(char), *buf_size, fptr);
  if (read <= 0){
    fclose(fptr);
    check_failure(read);
  } else {
    if(!str_cmp(mode, "rb"))
      (*buffer)[read] = '\0';
  }
}
