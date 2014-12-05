#pragma once

typedef struct node_t {
  void* item;
  struct node_t *next;
  struct node_t *prev;
} LNode;


typedef struct llist_t {
  LNode* head;
  LNode* tail;
  int size;  //number of elements in the list 
} LList;




/* create and return an empty list */
LList* LList_init();


/* return size (number of elements)  of list */
int LList_size(LList* l);



/* return the head of the list */
LNode *LList_head(LList *l);


/* return the tail of the list */
LNode *LList_tail(LList *l);


/* create a new node with the given item and insert it in the  list at the head */
void LList_insert_at_head(LList *l, void* item);

void LList_insert_at_tail(LList *l, void* item);


/* delete the node at the tail from the list and return it. It does
   NOT free the node.  */
LNode* LList_delete_at_tail(LList *l);


/* assume n is pointing to a node in l; delete it from the list (link
   its predeccor to it's sucessor). It does NOT free the node.  */
void  LList_delete(LList* l, LNode* n);


/* delete and free all nodes in the list */
void LList_delete_all(LList* l);

/* delete and free all nodes in the list and the list  */
void LList_free(LList* l);

