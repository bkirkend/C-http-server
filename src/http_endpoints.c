#include "../headers/str.h"
#include "../headers/util.h"
#include "../headers/http_endpoints.h"

bool endpoint_check(const char *req_type, const char *endpoint, const char *req_type_str, const char *endpoint_str){
  return str_cmp(endpoint, endpoint_str) && str_cmp(req_type, req_type_str);
}

void get_req_info(char *http_req, char **req_type, char **endpoint){
  int req_type_len = 0;
  int endpoint_len = 0; 
  char *http_ptr = http_req;
  while(*http_ptr++ != ' ') req_type_len++;

  // extract request type
  *req_type = malloc((req_type_len + 1) * sizeof(char));
  str_n_cpy(http_req, *req_type, req_type_len);
  (*req_type)[req_type_len] = '\0';
  printf("In func req_type:\n %s\n", *req_type);

  http_req = http_ptr;
  // extract request endpoint
  while(*http_ptr++ != ' ') endpoint_len++;
  *endpoint = malloc((endpoint_len + 1) * sizeof(char));
  str_n_cpy(http_req, *endpoint, endpoint_len);
  (*endpoint)[endpoint_len] = '\0';
  printf("In func endpoint:\n %s\n", *endpoint);
}

bool comp_req_type(char* http_req, char* req_type, char* endpoint){
  /*  Msg of form: GET / HTTP/1.1 */
  while(*http_req != ' '){
    printf("comparing %c and %c\n", *http_req, *req_type);
    if(*http_req++ != *req_type++){
      return false;
    }
  }
  if (*req_type != '\0') return false;

  printf("Successfully found req_type: %s\n", req_type);
  
  http_req++;
  while(*http_req != ' '){
    printf("comparing %c and %c\n", *http_req, *endpoint);
    if(*http_req++ != *endpoint++){
      return false;
    }
  }
  return *endpoint == '\0';
}

void http_send_file(int incoming_sd, char *file, char* file_mode, char* content_type, char* connection) {
  char *file_buf;
  int file_buf_len;
  readfile(file, file_mode, &file_buf, &file_buf_len);

  const char *header = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "Connection: %s\r\n"
        "\r\n";

  int header_size = snprintf(NULL, 0, header, content_type, file_buf_len, connection);
  if (header_size < 0) {
      close(incoming_sd);
      exit_with_error("Failed to get HTTP header size");
  }

  char *http_header = malloc((header_size + 1) * sizeof(char));
  snprintf(http_header, header_size + 1, header, content_type, file_buf_len, connection);
  int ret = send(incoming_sd, http_header, header_size, 0);
  free(http_header);
 
  if (ret == -1) {
      close(incoming_sd);
      exit_with_error("Failed to send HTTP response");
  }
  
  ret = send(incoming_sd, file_buf, file_buf_len, 0);
  free(file_buf);

  if (ret == -1) {
      close(incoming_sd);
      exit_with_error("Failed to send HTTP response");
  }
}
