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

void addItem(PriorityQueue* p, int priority, void* item);

void changePriority(PriorityQueue* p, int index, int newPriority);

void deleteNode(Node* node);

void deleteQueue(PriorityQueue* p);

void insert(PriorityQueue* p, Node* node);

Node* removeTop(PriorityQueue* p);
