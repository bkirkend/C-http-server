#include "../headers/hashmap.h"
#include "../headers/str.h"
#include <stdlib.h>
#include <stdio.h>

hashmap* init_map(void) {
    hashmap *hmap = malloc(sizeof(hashmap));
    if (!hmap) {
        printf("Error: Failed to allocate memory for hashmap\n");
        return NULL;
    }

    hmap->size = INIT_CAP;

    hmap->nodes = malloc(hmap->size * sizeof(hashnode*));
    if (!hmap->nodes) {
        free(hmap);
        printf("Error: Failed to allocate memory for hashmap nodes\n");
        return NULL;
    }

    // Initialize all nodes to NULL
    for (int i = 0; i < hmap->size; i++) {
        hmap->nodes[i] = NULL;
    }
    
    return hmap;
}

hashnode *init_hash_node(char *key, void (*func)(int)) {
    if (!key) {
        printf("Error: NULL key passed\n");
        return NULL;
    }

    hashnode* node = malloc(sizeof(hashnode));
    if (!node) return NULL;
    
    node->key = malloc(str_len(key) + 1);
    if (!node->key) {
        free(node);  
        return NULL;
    }

    str_cpy(key, node->key);
    node->handler = func;
    node->next = NULL;
    return node;
}

unsigned long hashString(char *str) {
    unsigned long hash = 5381;
    while (*str != '\0') {
        hash = ((hash << 5) + hash) + *str;  // hash * 33 + *str
        str++;
    }
    return hash;
}

void free_hashmap(hashmap *hmap) {
    for (int i = 0; i < hmap->size; i++) {
        free_hashnode(hmap->nodes[i]);  // Free each bucket's linked list
    }
    free(hmap->nodes);
    free(hmap);
}

void free_hashnode(hashnode *h) {
    hashnode *tmp;
    while (h) {
        tmp = h;
        h = h->next;
        free(tmp->key);  // Free the key
        free(tmp);  // Free the node itself
    }
}

hashnode* get_node(hashmap *hmap, char *key) {
    unsigned long hash = hashString(key);
    unsigned long index = hash % hmap->size;  // Ensure the index is within bounds

    hashnode *node = hmap->nodes[index];
    while (node) {
        if (str_cmp(key, node->key)) {
            // printf("node endpoint found: %s\n", node->key);
            return node;
        }
        node = node->next;
    }
    return NULL;
}

void add_node(hashmap *hmap, char *key, void (*func)(int)) {
    if (!hmap) {
        printf("Error: hashmap is NULL\n");
        return;
    }

    unsigned long hash = hashString(key);
    unsigned long index = hash % hmap->size;  // Ensure the index is within bounds
    
    // Ensure the node is created successfully
    hashnode *new_node = init_hash_node(key, func);
    if (!new_node) {
        printf("Error: Failed to allocate new node\n");
        return;
    }

    // Add new node at the beginning of the list (standard chaining)
    hashnode *node = hmap->nodes[index];
    if (node == NULL) {
        hmap->nodes[index] = new_node;  // Insert at the empty slot
    } else {
        new_node->next = node;  // Link the new node to the list
        hmap->nodes[index] = new_node;  // Update head of the list
    }
}
