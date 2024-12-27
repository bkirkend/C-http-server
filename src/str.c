#include "../headers/str.h"

bool str_cmp(const char *str1, const char *str2){
  while(*str1 != '\0'){
    if(*str1++ != *str2++){
      return false;
    }
  }
  return *str2 == '\0';
}

void str_cpy(const char *src, char *tgt){
  while((*tgt++ = *src++));
}

void str_n_cpy(const char *src, char *tgt, int n){
  for (int i = 0; i < n && *src; i++){
    tgt[i] = src[i];
  }
}

int str_len(const char *str){
  int len = 0;
  while(*str++) len++;
  return len;
}
