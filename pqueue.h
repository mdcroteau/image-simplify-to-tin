#pragma once

typedef struct node_ {
    double priority;
    void* item;
} Node;

typedef struct pQueue_ {
    Node** array;
    int arraySize;

    int size;
} PriorityQueue;

PriorityQueue* makeQueue();

Node* makeNode(double priority, void* item);

void addItem(PriorityQueue* p, double priority, void* item);

void changePriority(PriorityQueue* p, int index, double newPriority);

void deleteNode(Node* node);

void deleteQueue(PriorityQueue* p);

void insert(PriorityQueue* p, Node* node);

Node* removeTop(PriorityQueue* p);

void assertHeap(PriorityQueue* p);
