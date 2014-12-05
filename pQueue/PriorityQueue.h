#pragma once

typedef struct node_ {
    int priority;
    void* item;
} Node;

typedef struct pQueue_ {
    Node** array;
    int arraySize;

    int size;
} PriorityQueue;

PriorityQueue* makeQueue();

Node* makeNode(int priority, void* item);

void addItem(PriorityQueue*, int priority, void* item);

void deleteNode(Node* node);

void deleteQueue(PriorityQueue* p);

void insert(PriorityQueue* p, Node* node);

Node* removeTop(PriorityQueue* p);
