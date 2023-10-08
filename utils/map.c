// Implements methods and structures in map.h
// Author: Hoang Son Le a1691819
// Date: 8/10/2023

#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SEED 0x12345678

void List_Init(linked_list_t* LL) {
    LL->size = 0;
    LL->head = LL->tail = NULL;
    pthread_mutex_init(&LL->lock, NULL);
}

int List_Insert(linked_list_t* LL, char* title, char* value) {
    // Allocate memory for a new node
    node_t* tmp = (node_t*)malloc(sizeof(node_t));
    if (tmp == NULL) {
        perror("malloc: failed to allocate memory for List_Insert\n");
        return 1;
    }
    tmp->title = (char*)malloc(sizeof(char) * (strlen(title) + 1));
    if (tmp->title == NULL) {
        perror("malloc: failed to allocate node title\n");
        return 1;
    }
    tmp->value = (char*)malloc(sizeof(char) * (strlen(value) + 1));
    if (tmp->value == NULL) {
        perror("malloc: failed to allcoate node content\n");
        return 1;
    }
    // Copy the title and content to new node
    tmp->book_next = NULL;
    tmp->next = NULL;

    strcpy(tmp->title, title);
    strcpy(tmp->value, value);
    // Add new node to list
    pthread_mutex_lock(&LL->lock);
    LL->size += 1;
    if (LL->head == NULL)
        LL->head = tmp;
    if (LL->tail == NULL)
        LL->tail = tmp;
    else {
        LL->tail->book_next = tmp;
        LL->tail = tmp;
    }
    pthread_mutex_unlock(&LL->lock);
    return 0;
}

bool List_Contains(linked_list_t* LL, char* title) {
    pthread_mutex_lock(&LL->lock);
    node_t* current_node = LL->head;
    while (current_node != NULL) {
        if (strcmp(title, current_node->title) == 0) {
            pthread_mutex_unlock(&LL->lock);
            return true;
        }
        current_node = current_node->book_next;
    }
    pthread_mutex_unlock(&LL->lock);
    return false;
}

void List_Free(linked_list_t* LL) {
    pthread_mutex_lock(&LL->lock);
    node_t* current_node = LL->head;
    while (current_node != NULL) {
        node_t* new_node = current_node->book_next;
        free(current_node);
        current_node = new_node;
    }
    LL->head = LL->tail = NULL;
    pthread_mutex_unlock(&LL->lock);
}

unsigned long hash(char* str) {
    // One-byte-at-a-time hash based on Murmur's mix
    // Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp
    unsigned long h = (int)SEED;
    for (; *str; ++str) {
        h ^= *str;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}

void Map_Init(map_t* M) {
    for (int i = 0; i < CAPACITY; i++)
        List_Init(&M->lists[i]);
}

int Map_Insert(map_t* M, char* title, char* value) {
    unsigned long index = hash(title);
    return List_Insert(&M->lists[index], title, value);
}

void Map_Free(map_t* M) {
    for (int i = 0; i < CAPACITY; i++)
        List_Free(&M->lists[i]);
}

linked_list_t* Map_Get(map_t* M, char* title) {
    return &M->lists[hash(title)];
}
