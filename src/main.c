#include "../headers/util.h"
#include "../headers/restapi.h"
#include "../headers/parse_json.h"
#include "../headers/str.h"
#include "../headers/socket.h"
#include "../headers/queue.h"
#include "../headers/hashmap.h"
#include "../headers/http_endpoints.h"
#include "../headers/endpoint_handlers.h"

#define MAX_THREAD_COUNT 10
#define BACKLOG_SIZE 5

// global variable defs
int server_sd;
struct sockaddr_in* sv_addr;
pthread_t threadpool[MAX_THREAD_COUNT];
queue *work_queue;
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_cond = PTHREAD_COND_INITIALIZER;
hashmap *endpoint_map = NULL;

// cleanup on SIGINT
void handle_cleanup(int sig) {
    printf("Cleaning up and closing\n");
    close(server_sd);
    free(sv_addr);
    queue_free(work_queue);
    pthread_mutex_destroy(&queue_mutex);
    pthread_cond_destroy(&queue_cond);
    free_hashmap(endpoint_map);
    exit(EXIT_SUCCESS);
}

void* worker_thread_func(void* arg) {
    for (;;) {
        pthread_mutex_lock(&queue_mutex);

        while (queue_empty(work_queue)) {
            // releases mutex while waiting for signal
            pthread_cond_wait(&queue_cond, &queue_mutex);  
        }
      
        int incoming_sd = dequeue(work_queue);
        // printf("Worker thread dequeued socket %d\n", incoming_sd);
        // queue_print(work_queue);
        pthread_mutex_unlock(&queue_mutex);

        // Handle the client request
        char buffer[1024];
        int ret = recv_wrapper(incoming_sd, buffer, sizeof(buffer));
        if (ret == 1) {
            printf("Worker thread: No data received, closing connection\n");
            close(incoming_sd);
            continue;
        }

        char *req_type;
        endpoint_t *endpoint = get_req_info(buffer, &req_type);
        printf("Worker thread: req_type: %s, endpoint: %s\n", req_type, endpoint->handle);

        hashnode *endpoint_node;
        if((endpoint_node = get_node(endpoint_map, endpoint->handle)) != NULL) {
          // printf("Got endpoint %s\n", endpoint_node->key);
          endpoint_node->handler(incoming_sd, endpoint->params);
        } else {
          printf("Endpoint not found\n");
        }

        free(req_type);
        free_endpoint(endpoint);
        close(incoming_sd);
    }
    return NULL;
}

void create_worker_threads() {
    for (int i = 0; i < MAX_THREAD_COUNT; i++) {
        if (pthread_create(&threadpool[i], NULL, worker_thread_func, NULL) != 0) {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
    }
}


// Main function
int main(int argc, char **argv) {
    signal(SIGINT, handle_cleanup);

    if (argc != 2) {
        exit_with_error("Invalid argument number");
    }

    int port = get_port(argv[1]);
    server_sd = init_socket();
    sockopt_wrapper(server_sd);
    // printf("Main: Server socket initialized, fd: %d\n", server_sd);

    socklen_t sv_addr_len;
    sv_addr = bind_port(server_sd, port, &sv_addr_len);
    if (listen(server_sd, BACKLOG_SIZE) != 0) {
        close(server_sd);
        exit_with_error("Failed to listen on server socket");
    }
    // printf("Main: Listening on port %d\n", port);

    // initialize the queue and worker threads
    work_queue = queue_init();
    create_worker_threads();

    //initialize endpoint hashmap
    endpoint_map = create_hashmap();

    // connection loop
    for (;;) {
        int incoming_sd = accept_wrapper(server_sd, sv_addr, &sv_addr_len);
        // printf("Main: Accepted new connection, socket fd: %d\n", incoming_sd);

        // Add the client socket descriptor to the queue
        pthread_mutex_lock(&queue_mutex);
        enqueue(work_queue, incoming_sd);
        // printf("Main: Enqueued socket %d\n", incoming_sd);
        // queue_print(work_queue);
        pthread_cond_signal(&queue_cond);  // Wake up one worker thread
        pthread_mutex_unlock(&queue_mutex);
    }

    return EXIT_SUCCESS;
}
