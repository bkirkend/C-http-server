#include "endpoint_parse.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

//TODO: can remove send calls,
// return char[] msgs and pass to socket.c
// separates socket from http msg generation

bool endpoint_check(const char *req_type, const char *endpoint, const char *req_type_str, const char *endpoint_str);
endpoint_t* get_req_info(char *http_req, char **req_type);
bool comp_req_type(char* http_req, char* req_type, char* endpoint);
void http_send_file(int incoming_sd, char *file, char* file_mode, char* content_type, char* connection);
void http_send_raw_msg(int incoming_sd, char *msg, char *connection);
