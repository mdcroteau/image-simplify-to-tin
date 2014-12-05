/* llist.c */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "llist.h"

/* create and return an empty list */
LList* LList_init() {
    LList* list = (LList *) malloc(sizeof(LList));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

/* return size (number of elements)  of list */
int LList_size(LList* l) {
    return l->size; 
} 

/* print all values in the list in order form head to tail  */
void LList_print(LList * l) {
    Node* node = l->head;
    while (node != NULL) {
        printf("%d, ", node->value);
        node = node->next;
    }
    printf("\n");
}

/* return the head of the list */
Node *LList_head(LList *l) {
    return l->head;
}

/* return the tail of the list */
Node *LList_tail(LList *l) {
    return l->tail;
}

/* create a new node with value=key and insert it in the  list at the head */
void LList_insert_at_head(LList *l, int key) {
    Node* node = (Node *) malloc(sizeof(Node));
    node->value = key;
    node->prev = NULL;
    node->next = NULL;

    if (!l->size) {
        l->head = node;
        l->tail = node;
    } else if (l->size == 1) {
        l->head = node;
        node->next = l->tail;
        l->tail->prev = node;
    } else {
        node->next = l->head;
        l->head->prev = node;
        l->head = node;
    }
    ++l->size;
}

/* This operation assumes the list is sorted in increasing order of
the values.  It create a new node with value=key and insert it in the
list in the right place (ie before the first node that's larger than
the key) */
void LList_insert_in_order(LList *l, int key) {
    assert(LList_is_sorted(l));

    Node* newNode = (Node *) malloc(sizeof(Node)); 
    newNode->value = key;
    newNode->prev = NULL;
    newNode->next = NULL;

    if (!l->size) {
        l->head = newNode;
        l->tail = newNode;
    } else if (key < l->head->value) {
        newNode->next = l->head; 
        l->head->prev = newNode;
        l->head = newNode;
    } else if (key >= l->tail->value) {
        newNode->prev = l->tail; 
        l->tail->next = newNode;
        l->tail = newNode;
    } else {
        Node* node = l->head->next;
        while (node != NULL) {
            if (node->value > key) {
                node->prev->next = newNode;
                newNode->prev = node->prev;
                newNode->next = node;
                node->prev = newNode;
                break;
            }
            node = node->next;
        }
    }
    ++l->size;
}

/* this function checks if the list is sorted in incresing order;
   return 1 if list is sorted, 0 otherwise*/
int LList_is_sorted(LList *l) {
    if (!l->size) {
        return 1;
    }

    Node* node = l->head;
    while (node->next != NULL) {
        if (node->value > node->next->value) {
            return 0;
        }
        node = node->next;
    }
    return 1; 
}

/* delete the node at the tail from the list and return it. It does
   NOT free the node.  */ 
Node* LList_delete_at_tail(LList *l) {
    assert(l->size >= 1);

    Node* oldTail = l->tail;
    if (l->size == 1) {
        l->head = NULL;
        l->tail = NULL;
    } else {
        Node* newTail = oldTail->prev;
        newTail->next = NULL;
        l->tail = newTail;
    }
    --l->size;

    return oldTail;
}

/* assume n is pointing to a node in l; delete it from the list (link
   its predeccor to it's sucessor). It does NOT free the node.  */
void  LList_delete(LList* l, Node* n) {
    assert(l->size >= 1);

    if (l->size == 1) {
        l->head = NULL;
        l->tail = NULL;
    } else if (l->head == n) {
        l->head = l->head->next;    
        l->head->prev = NULL;
    } else if (l->tail == n) {
        l->tail = l->tail->prev;    
        l->tail->next = NULL;
    } else {
        n->prev->next = n->next;
        n->next->prev = n->prev;
    }
    --l->size;
}

/* delete and free all nodes in the list */
void LList_delete_all(LList* l) {
    Node* node = l->head;
    while (node != NULL) {
        Node* temp = node->next;
        free(node);
        node = temp;
    }

    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
}

/* delete and free all nodes in the list and the list  */
void LList_free(LList* l) {
    LList_delete_all(l);
    free(l);
}
