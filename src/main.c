#include "../headers/util.h"
#include "../headers/restapi.h"
#include "../headers/parse_json.h"
#include "../headers/str.h"
#include "../headers/socket.h"
#include "../headers/http_endpoints.h"

// define globally for cleanup in sig handler
int server_sd;
struct sockaddr_in* sv_addr;

void handle_cleanup(int sig){
  printf("Cleaning up and closing\n");
  close(server_sd);
  free(sv_addr);
  exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
  signal(SIGINT, handle_cleanup);

  if (argc != 2) 
    exit_with_error("Invalid argument number");

  int port = get_port(argv[1]);
  server_sd = init_socket();
  printf("%d\n", server_sd);
  socklen_t sv_addr_len; 
  sv_addr = bind_port(server_sd, port, &sv_addr_len);
  if(listen(server_sd, BACKLOG_SIZE) != 0){
    close(server_sd);
    exit_with_error("failed to connect");
  }

  //connection loop
  char buffer[1024];
  for (;;) {
    // need to occur per-connection
    int incoming_sd = accept_wrapper(server_sd, sv_addr, &sv_addr_len);
    for(;;){
      int ret = recv_wrapper(incoming_sd, buffer, sizeof(buffer));
      if(ret == 1) break;
      char *req_type;
      char *endpoint;
      get_req_info(buffer, &req_type, &endpoint);
      printf("req_type:\n %s\n", req_type);
      printf("endpoint:\n %s\n", endpoint);

      if (endpoint_check(req_type, endpoint, "GET", "/")){
        printf("default endpoint\n");
      } else if (endpoint_check(req_type, endpoint, "GET", "/json")){
        http_send_file(incoming_sd, "json/nfl.json", "rb", "application/json", "keep-alive");
        printf("send json payload\n");
      } else if (endpoint_check(req_type, endpoint, "GET", "/print")){
        printf("Hello World\n");
      } else if (endpoint_check(req_type, endpoint, "GET", "/favicon.ico")){
        http_send_file(incoming_sd, "img/B.ico", "rb", "image/x-icon", "close");
        printf("After sending img\n");
      } else if (endpoint_check(req_type, endpoint, "GET", "/index.html")){
        http_send_file(incoming_sd, "html/index.html", "r", "text/html", "keep-alive");
        printf("Sending html\n");
      } else if (endpoint_check(req_type, endpoint, "GET", "/index.js")){
        http_send_file(incoming_sd, "javascript/index.js", "r", "application/javascript", "keep-alive");
        printf("Sending javascript\n");
      } else {
        printf("Unrecognized cmd\n");
      }

      free(req_type);
      free(endpoint);
    }
  }
  return EXIT_SUCCESS;
}
