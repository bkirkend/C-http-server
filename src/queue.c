#include <stdio.h>
#include <stdlib.h>
#include "../headers/queue.h"

static q_item* q_item_init(int sd){
  q_item* item = malloc(sizeof(q_item));
  if(!item) return NULL;
  item->sd = sd;
  item->next = NULL;
  return item;
}

queue* queue_init(void){
  queue *q = malloc(sizeof(queue));
  if(!q) return NULL;

  q->head = NULL;
  q->tail = NULL;
  return q;
}

void enqueue(queue *q, int sd){
  q_item *item = q_item_init(sd);
  if (!item) return;
  if(!q->tail){
    q->head = q->tail = item;
  } else {
    q->tail->next = item;
    q->tail = item;
  }
}

int dequeue(queue *q){
  q_item *item = q->head;
  int sd = item->sd;
  q->head = item->next;
  if(!q->head) q->tail = NULL;
  free(item);
  return sd;
}

bool queue_empty(queue *q){
  return q->head == NULL;
}

void queue_free(queue *q){
  q_item *item = q->head;
  while(item){
    q_item *curr = item;
    item = item->next;
    free(curr);
  }
  free(q);
}

void queue_print(queue *q){
  q_item *item = q->head;
  if (!item){
    printf("Empty queue\n");
    return;
  } 
  while(item){
    printf("%d ", item->sd);
    item = item->next;
  }
  printf("\n");
}
