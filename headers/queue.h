#include <stdbool.h>

typedef struct _q_item{
  int sd;
  struct _q_item *next;
} q_item;

typedef struct _queue {
  q_item *head;
  q_item *tail; 
} queue;

queue* queue_init(void);
void enqueue(queue *q, int sd);
int dequeue(queue *q);
bool queue_empty(queue *q);
void queue_free(queue *q);
static q_item* q_item_init(int sd);
void queue_print(queue *q);
