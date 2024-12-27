#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BACKLOG_SIZE 5

int init_socket(void);
struct sockaddr_in* bind_port(int sd, int port, socklen_t *sv_addr_len);
int get_port(char* arg);
int accept_wrapper(int server_sd, struct sockaddr_in* sv_addr, socklen_t *sv_addr_len);
int recv_wrapper(int incoming_sd, char* buffer, size_t buf_size);
