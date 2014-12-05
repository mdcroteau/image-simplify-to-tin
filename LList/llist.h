/* llist.h */

#ifndef __llist_h
#define __llist_h



typedef struct node_t {
  int value;
  struct node_t *next;
  struct node_t *prev;
} Node;


typedef struct llist_t {
  Node* head;
  Node* tail;
  int size;  //number of elements in the list 
} LList; 




/* create and return an empty list */
LList* LList_init();


/* return size (number of elements)  of list */
int LList_size(LList* l);



/* print all values in the list in order form head to tail  */
void LList_print(LList * l);


/* return the head of the list */
Node *LList_head(LList *l);


/* return the tail of the list */
Node *LList_tail(LList *l);


/* create a new node with value=key and insert it in the  list at the head */
void LList_insert_at_head(LList *l, int key);


/* This operation assumes the list is sorted in increasing order of
the values.  It create a new node with value=key and insert it in the
list in the right place (ie before the first node that's larger than
the key) */
void LList_insert_in_order(LList *l, int key);



/* this function checks if the list is sorted in incresing order;
   return 1 if list is sorted, 0 otherwise*/
int LList_is_sorted(LList *l);


/* delete the node at the tail from the list and return it. It does
   NOT free the node.  */ 
Node* LList_delete_at_tail(LList *l);


/* assume n is pointing to a node in l; delete it from the list (link
   its predeccor to it's sucessor). It does NOT free the node.  */
void  LList_delete(LList* l, Node* n);


/* delete and free all nodes in the list */
void LList_delete_all(LList* l);

/* delete and free all nodes in the list and the list  */
void LList_free(LList* l);


#endif
