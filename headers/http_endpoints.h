#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

//TODO: can remove send calls,
// return char[] msgs and pass to socket.c
// separates socket from http msg generation

bool endpoint_check(const char *req_type, const char *endpoint, const char *req_type_str, const char *endpoint_str);
void get_req_info(char *http_req, char **req_type, char **endpoint);
bool comp_req_type(char* http_req, char* req_type, char* endpoint);
void http_resp(int incoming_sd);
void http_json_from_file(int incoming_sd, char *jsonfile);
void http_html_from_file(int incoming_sd, char *htmlfile);
void http_img_from_file(int incoming_sd, char *faviconfile);


