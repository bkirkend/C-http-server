#include "../headers/str.h"
#include "../headers/util.h"
#include "../headers/http_endpoints.h"

bool endpoint_check(const char *req_type, const char *endpoint, const char *req_type_str, const char *endpoint_str){
  return str_cmp(endpoint, endpoint_str) && str_cmp(req_type, req_type_str);
}

endpoint_t* get_req_info(char *http_req, char **req_type){
  int req_type_len = 0;
  int url_len = 0; 
  char *http_ptr = http_req;
  while(*http_ptr++ != ' ') req_type_len++;

  // extract request type
  *req_type = malloc((req_type_len + 1) * sizeof(char));
  str_n_cpy(http_req, *req_type, req_type_len);
  (*req_type)[req_type_len] = '\0';
  printf("In func req_type:\n %s\n", *req_type);

  http_req = http_ptr;
  // extract request endpoint
  while(*http_ptr++ != ' ') url_len++;
  char *url = malloc((url_len + 1) * sizeof(char));
  str_n_cpy(http_req, url, url_len);
  (url)[url_len] = '\0';

  endpoint_t *endpoint = parse_endpoint(url);
  printf("In func endpoint:\n %s\n", url);
  printf("Printing endpoint parse: \n");
  print_endpoint(endpoint);

  return endpoint;
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

void http_send_raw_msg(int incoming_sd, char *msg, char *connection) {
  const char *header_template = 
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain\r\n"
      "Content-Length: %d\r\n"
      "Connection: %s\r\n"
      "\r\n";

  int msg_len = str_len(msg);
  int header_len = snprintf(NULL, 0, header_template, msg_len, connection);

  char *header = malloc(header_len + 1);
  if (!header) {
    close(incoming_sd);
    exit_with_error("Memory allocation failed for HTTP header");
  }

  snprintf(header, header_len + 1, header_template, msg_len, connection);

  int total_len = header_len + msg_len;
  char *http_msg = malloc(total_len + 1);
  if (!http_msg) {
    free(header);
    close(incoming_sd);
    exit_with_error("Memory allocation failed for HTTP response");
  }

  str_n_cpy(header, http_msg, header_len);
  str_n_cpy(msg, http_msg + header_len, msg_len + 1);

  // printf("Sending MSG:\n %s\n", http_msg);
  int ret = send(incoming_sd, http_msg, total_len, 0);
  
  free(header);
  free(http_msg);

  if (ret == -1) {
    close(incoming_sd);
    exit_with_error("Failed to send raw HTTP response");
  }
}
