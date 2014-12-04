#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "PriorityQueue.h"

// Internal functions for the queue
void resizeQueue(PriorityQueue* p);
void fillGap(PriorityQueue* p, int index);
void siftUp(PriorityQueue* p, int index);
void siftDown(PriorityQueue* p, int index);

PriorityQueue* makeQueue(){
  PriorityQueue* out = malloc(sizeof(PriorityQueue));

  out->array = malloc(7* sizeof(Node*));
  out->arraySize = 7;
  out->size = 0;

  return out;
}

Node* makeNode(int priority, void* item)
{
  Node* out = malloc(sizeof(Node));

  out->priority = priority;
  out->item = item;

  return out;
}

void deleteNode(Node* node)
{
  free(node);
}

void deleteQueue(PriorityQueue* p)
{
  free(p->array);
  free(p);
}

void addItem(PriorityQueue* p, int priority, void* item)
{
  insert(p, makeNode(priority, item));
}

void insert(PriorityQueue* p, Node* node)
{
  if(p->arraySize == p->size){
    resizeQueue(p);
  }

  p->array[p->size] = node;
  p->size++;

  siftUp(p, p->size-1);
}

void changePriority(PriorityQueue* p, int index, int newPriority)
{
  int diff = newPriority - p->array[index]->priority;
  p->array[index]->priority = newPriority;

  if(diff >= 0){
    siftUp(p, index);
  }
  else{
    siftDown(p, index);
  }
}

void resizeQueue(PriorityQueue* p)
{
  p->array = realloc(p->array, (2*p->size+1)*sizeof(Node*));
  p->arraySize = 2*p->size + 1;
}

Node* removeTop(PriorityQueue* p)
{
  if(p->size == 0) return NULL;
  Node* top = p->array[0];
  fillGap(p, 0);
  p->size--;

  return top;
}

// Internal function, takes queue and index of a NULL Node and filters
// the Node down to the leaves
void fillGap(PriorityQueue* p, int index)
{
  // Node cannot possibly have children
  if(index*2 + 1 > (p->size - 2)){

    // The value is at the end of the queue
    if(index == p->size-1){
      p->array[index] = NULL;
      return;
    }

    // Swap with node at end of queue
    p->array[index] = p->array[p->size - 1];
    p->array[p->size - 1] = NULL;

    // Ensure heap properties by moving up the node
    siftUp(p, index);

    return;
  }

  Node* c1 = p->array[index*2 + 1];
  Node* c2 = p->array[index*2 + 2];

  int nextIndex;

  // Choose higher priority child
  if(c1->priority < c2->priority){
    nextIndex = index * 2 + 2;
  }
  else{
    nextIndex = index * 2 + 1;
  }

  // Swap with child
  p->array[index] = p->array[nextIndex];
  p->array[nextIndex] = NULL;

  // Continue moving down heap
  fillGap(p, nextIndex);
}

// Takes a node index and moves it up the heap to an appropriate level
// Note, appropriate level may be no levels
void siftUp(PriorityQueue* p, int index)
{
  // It's the head, no upwards motion to be had
  if(index == 0) return;

  // child in this case refers to the given index
  Node* child = p->array[index];
  Node* parent = p->array[(index-1)/2];

  // If it has higer priority, swap them
  if(child->priority > parent->priority){
    p->array[(index-1)/2] = child;
    p->array[index] = parent;

    // Give it the chance to move up more if possible
    siftUp(p, (index-1)/2);
  }
}

void siftDown(PriorityQueue* p, int index)
{
  // Node cannot possibly have children
  if(index*2 + 1 > (p->size - 2)) return;

  // NOT null
  Node* item = p->array[index];

  // None, one or both of these could be null
  Node* c1 = p->array[index*2 + 1];
  Node* c2 = p->array[index*2 + 2];

  // Both are null
  if(c1 == NULL) return;

  // Just c2 is null
  if(c2 == NULL) {
    if(c1->priority > item->priority) {
      p->array[index] = c1;
      p->array[index*2 + 1] = item;
      siftDown(p, index*2 + 1);
      return;
    }
  }

  // Neither are null, and both have lower priority than item in question
  if(item->priority > c1->priority && item->priority > c2-> priority) {
    return;
  }

  if(c1->priority > c2->priority){
    p->array[index] = c1;
    p->array[index*2 + 1] = item;
    siftDown(p, index*2 + 2);
    return;
  }
  else{
    p->array[index] = c2;
    p->array[index*2 + 2] = item;
    siftDown(p, index*2 + 2);
    return;
  }
}

void printQueuePriorities(PriorityQueue* p)
{
  printf(     "Size is: %d\n", p->size);
  for(int i=0; i<p->size; i++){
    Node* n = p->array[i];

    printf("     Priority: %d\n", n->priority);
  }
  printf("______-----------______\n");
}

// Check to make sure the PriorityQueue maintians Heap qualities
void assertHeap(PriorityQueue* p)
{
  for(int i=0; i<p->size; i++){
    int priority = p->array[i]->priority;
    if(2*i+1 < p->size){
      assert(p->array[2*i+1]->priority <= priority);
      break;
    }
    if(2*i+2 < p->size){
      assert(p->array[2*i+2]->priority <= priority);
      break;
    }
  }

  for(int i=p->size; i<p->arraySize; i++){
    assert(!p->array[i]);
  }
}

// Tester
int main(int argc, char** argv)
{
  PriorityQueue* p = makeQueue();

  Node* n;

  printf("Building the queue\n");
  for(int i=0; i<10; i++){
    //n = makeNode(rand()%10000, (void*)0);
    addItem(p, 10-i, n);
    //insert(p, n);
    assertHeap(p);
  }
  changePriority(p, 3, 3);
  assertHeap(p);
  changePriority(p, 3, 20);
  assertHeap(p);
  changePriority(p, 5, 17);
  assertHeap(p);
  changePriority(p, 1, 2);
  assertHeap(p);
  changePriority(p, 9, 2);
  assertHeap(p);
  changePriority(p, 8, 100);
  assertHeap(p);
  printQueuePriorities(p);

  while(removeTop(p) != NULL);

  printf("Deleting the queue\n");
  deleteQueue(p);
  return 0;
}
