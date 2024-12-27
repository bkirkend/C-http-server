#include <stdio.h>
#include <stdlib.h>

void exit_with_error(char *msg);
void check_failure(int retcode);
void readfile(char* file_name, char* mode, char** buffer, int* buf_size);

