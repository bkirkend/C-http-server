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

// currently unused
void http_resp(int incoming_sd){
    const char *json_body =
        "{\n"
        "  \"status\": \"success\",\n"
        "  \"data\": {\n"
        "    \"id\": 1,\n"
        "    \"name\": \"John Doe\",\n"
        "    \"email\": \"john.doe@example.com\"\n"
        "  }\n"
        "}";

    char response[1024];
    int json_body_len = str_len(json_body);

    // Construct HTTP response dynamically
    int ret = snprintf(response, sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %d\r\n"
        "Connection: keep-alive\r\n"
        "\r\n"
        "%s", json_body_len, json_body);

    if (ret < 0 || ret >= sizeof(response)) {
        close(incoming_sd);
        exit_with_error("Failed to construct HTTP response");
    }

    // Send the response
    ret = send(incoming_sd, response, str_len(response), 0);
    if (ret == -1) {
        close(incoming_sd);
        exit_with_error("Failed to send HTTP response");
    }
}

void http_json_from_file(int incoming_sd, char* jsonfile) {
    char *json_data;
    int json_data_len;

    readfile(jsonfile, "r", &json_data, &json_data_len);
    if (json_data == NULL) {
        close(incoming_sd);
        exit_with_error("Failed to allocate json_data");
    }

    int header_size = snprintf(NULL, 0,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %d\r\n"
        "Connection: keep-alive\r\n"
        "\r\n", json_data_len);
    
    if (header_size < 0) {
        close(incoming_sd);
        exit_with_error("Failed to calculate header size");
    }

    int total_size = header_size + json_data_len + 1; // +1 for the null terminator
    char *http_response = (char *)malloc(total_size);
    if (http_response == NULL) {
        close(incoming_sd);
        exit_with_error("Failed to allocate memory for HTTP response");
    }

    int ret = snprintf(http_response, total_size,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %d\r\n"
        "Connection: keep-alive\r\n"
        "\r\n"
        "%s", json_data_len, json_data);

    free(json_data);

    if (ret < 0 || ret >= total_size) {
        close(incoming_sd);
        free(http_response);
        exit_with_error("Failed to construct HTTP response");
    }

    // printf("%s", http_response);

    ret = send(incoming_sd, http_response, total_size - 1, 0); // Don't send the null terminator
    if (ret == -1) {
        close(incoming_sd);
        free(http_response);
        exit_with_error("Failed to send HTTP response");
    }

    free(http_response);
}

void http_html_from_file(int incoming_sd, char *htmlfile){
  char *http_payload;
  int http_payload_size;

  readfile(htmlfile, "r", &http_payload, &http_payload_size);

  const char *header_template = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n"
        "\r\n";

  int header_size = snprintf(NULL, 0, header_template, http_payload_size);
  if (header_size < 0) {
      close(incoming_sd);
      exit_with_error("Failed to get HTTP header size");
  }
  
  char* header = malloc((header_size + 1) * sizeof(char)); 
  snprintf(header, header_size + 1, header_template, http_payload_size);
  int ret = send(incoming_sd, header, header_size, 0);

  free(header);
  if (ret == -1) {
      close(incoming_sd);
      exit_with_error("Failed to send HTTP response");
  }

  ret = send(incoming_sd, http_payload, http_payload_size, 0);

  free(http_payload);
  if (ret == -1) {
      close(incoming_sd);
      exit_with_error("Failed to send HTTP response");
  }
}

void http_img_from_file(int incoming_sd, char *faviconfile){
  char *img_bin;
  int img_bin_len;
  readfile(faviconfile, "rb", &img_bin, &img_bin_len);

  const char *header = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: image/x-icon\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n"
        "\r\n";

  int header_size = snprintf(NULL, 0, header, img_bin_len);
  if (header_size < 0) {
      close(incoming_sd);
      exit_with_error("Failed to get HTTP header size");
  }

  char *http_header = malloc((header_size + 1) * sizeof(char));
  snprintf(http_header, header_size + 1, header, img_bin_len);
  int ret = send(incoming_sd, http_header, header_size, 0);
  free(http_header);
 
  if (ret == -1) {
      close(incoming_sd);
      exit_with_error("Failed to send HTTP response");
  }
  
  ret = send(incoming_sd, img_bin, img_bin_len, 0);
  free(img_bin);

  if (ret == -1) {
      close(incoming_sd);
      exit_with_error("Failed to send HTTP response");
  }
}

void http_jscript_from_file(int incoming_sd, char* jscriptfile) {
    char *jscript_data;
    int jscript_data_len;

    readfile(jscriptfile, "r", &jscript_data, &jscript_data_len);
    if (jscript_data == NULL) {
        close(incoming_sd);
        exit_with_error("Failed to allocate jscript_data");
    }

    int header_size = snprintf(NULL, 0,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/javascript\r\n"
        "Content-Length: %d\r\n"
        "Connection: keep-alive\r\n"
        "\r\n", jscript_data_len);
    
    if (header_size < 0) {
        close(incoming_sd);
        exit_with_error("Failed to calculate header size");
    }

    int total_size = header_size + jscript_data_len + 1; // +1 for the null terminator
    char *http_response = (char *)malloc(total_size);
    if (http_response == NULL) {
        close(incoming_sd);
        exit_with_error("Failed to allocate memory for HTTP response");
    }

    int ret = snprintf(http_response, total_size,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/jscript\r\n"
        "Content-Length: %d\r\n"
        "Connection: keep-alive\r\n"
        "\r\n"
        "%s", jscript_data_len, jscript_data);

    free(jscript_data);

    if (ret < 0 || ret >= total_size) {
        close(incoming_sd);
        free(http_response);
        exit_with_error("Failed to construct HTTP response");
    }

    ret = send(incoming_sd, http_response, total_size - 1, 0); // Don't send the null terminator
    if (ret == -1) {
        close(incoming_sd);
        free(http_response);
        exit_with_error("Failed to send HTTP response");
    }

    free(http_response);
}
