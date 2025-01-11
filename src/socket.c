#include "../headers/socket.h"
#include "../headers/util.h"

int init_socket(void){
  int sd = socket(PF_INET, SOCK_STREAM, 0);
  if (sd == -1) exit_with_error("Failed to create socket");
  return sd;
}

void sockopt_wrapper(int server_sd){
  int opt = 1;
  int ret = setsockopt(server_sd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  if (ret < 0) exit_with_error("Failed to set socket options");
}

struct sockaddr_in* bind_port(int sd, int port, socklen_t *sv_addr_len){
  struct sockaddr_in* sv_addr = malloc(sizeof(struct sockaddr_in));
  if (sv_addr == NULL){
    close(sd);
    exit_with_error("Failed to allocate sockaddr_in");
  }
  sv_addr->sin_family = PF_INET;
  sv_addr->sin_addr.s_addr = INADDR_ANY;
  sv_addr->sin_port = htons(port);
  *sv_addr_len = sizeof(*sv_addr);
  int ret = bind(sd, (struct sockaddr*)sv_addr, *sv_addr_len);
  if (ret != 0){
    close(sd);
    free(sv_addr);
    exit_with_error("failed to bind");
  } 
  return sv_addr;
}

int get_port(char* arg){
  int port = atoi(arg);
  if (port <= 0 || port > 65535) exit_with_error("Invalid port argument");
  return port;
}

int accept_wrapper(int server_sd, struct sockaddr_in* sv_addr, socklen_t *sv_addr_len){
  int incoming_sd = accept(server_sd, (struct sockaddr*)sv_addr, sv_addr_len);
  if (incoming_sd == -1){
    close(server_sd);
    free(sv_addr);
    exit_with_error("Failed to accept");
  }
  return incoming_sd;
}

int recv_wrapper(int incoming_sd, char* buffer, size_t buf_size){
  int ret = recv(incoming_sd, buffer, buf_size - 1, 0);
  if (ret > 0){
    buffer[ret] = '\0';
    printf("Received msg:\n%s", buffer); 
    return 0;
  }
  if (ret < 0){
    close(incoming_sd);
    exit_with_error("failed to recv");
  }
  printf("Connection closed\n");
  close(incoming_sd);
  return 1;
}
