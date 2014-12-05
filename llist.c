#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "llist.h"



/* create and return an empty list */
LList* LList_init() {
  LList* list;
  list = malloc(sizeof(LList));
  assert(list);
  list->head = NULL;
  list->tail = NULL;
  list->size = 0;
  return list;
}

/* return size (number of elements)  of list */
int LList_size(LList* l) {
  return l->size;
} 

LNode* LList_head(LList *l) {
  return l->head;
}

/* return the tail of the list */
LNode* LList_tail(LList *l) {
  return l->tail;
}

/* create a new node with value=key and insert it in the  list at the head */
void LList_insert_at_head(LList *l, void* item) {
  LNode* newHead = malloc(sizeof(LNode));
  assert(newHead);

  // Case of empty list
  if(l->head == NULL){
    l->head = newHead;
    l->tail = newHead;
    newHead->prev = NULL;
    newHead->next = NULL;
    newHead->item = item;
    l->size = 1;
    return;
  }

  newHead->item = item;
  newHead->next = l->head;
  newHead->prev = NULL;
  l->head->prev = newHead;
  l->head = newHead;
  l->size = l->size +1;
  return;
}

void LList_insert_at_tail(LList *l, void* item)\
{
  LNode* newTail = malloc(sizeof(LNode));

  // Case of empty list
  if(l->head == NULL){
    l->head = newTail;
    l->tail = newTail;
    newTail->prev = NULL;
    newTail->next = NULL;
    newTail->item = item;
    l->size = 1;
    return;
  }

  newTail->item = item;
  newTail->next = NULL;
  newTail->prev = l->tail;
  l->tail->next = newTail;
  l->tail = newTail;
  l->size = l->size + 1;
}

/* delete the node at the tail from the list and return it. It does
   NOT free the node.  */ 
LNode* LList_delete_at_tail(LList *l) {
  if(l == NULL) return NULL;
  if(l->tail == NULL) return NULL; // Empty list
  LNode* toReturn = l->tail;
  
  if(l->tail == l->head){
    l->tail = NULL;
    l->head = NULL;
    l->size = 0;
    return toReturn;
  }

  l->tail = l->tail->prev;
  l->tail->next = NULL;
  l->size = l->size -1;
  return toReturn;
}

/* assume n is pointing to a node in l; delete it from the list (link
   its predeccor to it's sucessor). It does NOT free the node.  */
void  LList_delete(LList* l, LNode* n) {
  n->prev->next = n->next;
  n->next->prev = n->prev;
  l->size = l->size -1;
}


/* delete and free all nodes in the list */
void LList_delete_all(LList* l) {
  LNode* nextDeletion = l->head;
  while(nextDeletion != NULL){
    LNode* temp = nextDeletion;
    nextDeletion = nextDeletion->next;
    free(temp);
  }
  l->size = 0;
}

/* delete and free all nodes in the list and the list  */
void LList_free(LList* l) {
  LList_delete_all(l);
  free(l);
}

