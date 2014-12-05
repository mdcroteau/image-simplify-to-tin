//File: simplification.c
//Authors: Croteau, Imhoff, Zeller
//Date: 11/09/14

#include "simplification.h"
#include "pQueue/PriorityQueue.h"

#include <stdlib.h>
#include <math.h>

void initializeTriangle(Triangle *t, Grid* g, int row1, int col1, int row2, int col2, int row3, int col3) 
{
    // First vertex
    t.triangle.v1 = (Vertex *) malloc(sizeof(Vertex));
    t.triangle.v1.row = row1;
    t.triangle.v1.col = col1;
    t.triangle.v1.value = get(g, row1, col1);
    set(g, row1, col1, g->NODATA_value);
    
    // Second vertex
    t.triangle.v2 = (Vertex *) malloc(sizeof(Vertex));
    t.triangle.v2.row = row2;
    t.triangle.v2.col = col2;
    t.triangle.v2.value = get(g, row2, col2);
    set(g, row2, col2, g->NODATA_value);
    
    // Third vertex
    t.triangle.v3 = (Vertex *) malloc(sizeof(Vertex));
    t.triangle.v3.row = row3;
    t.triangle.v3.col = col3;
    t.triangle.v3.value = get(g, row3, col3);
    set(g, row3, col3, g->NODATA_value);
}

// TODO
double linearlyInterpolate(Vertex* v1, Vertex* v2, Vertex* v3, int row, int col) 
{
}

double computeError(Triangle* t, Vertex* v)
{
    Vertex v1;
    v1.row = 0;
    v1.col = 0;
    v1.value = get(g, v1.row, v1.col);

    Vertex v2;
    if (row > col) {
        v2.row = row - 1;
        v2.col = 0;
    } else {
        v2.row = 0;
        v2.col = col - 1;
    }
    v2.value = get(g, v2.row, v2.col);

    Vertex v3;
    v3.row = row - 1;
    v3.col = col - 1;
    v3.value = get(g, v3.row, v3.col);

    double fromTin = linearlyInterpolate(&v1, &v2, &v3, row, col);
    return abs(fromTin - (double)get(g, row, col));
}

// TODO
int triangleContains(Vertex* v, Triangle* t) 
{
}

TIN* simplify(TIN* tin, Grid* g, double epsilon) 
{
    // Initialize TIN with 4 corner points
    // TODO make triangle initilization prettier
    Triangle* bottomLeft = (Triangle *) malloc(sizeof(Triangle));
    initializeTriangle(bottomLeft, g, 0, 0, g.rows - 1, 0, g.rows - 1, g.cols - 1);

    Triangle* topRight = (Triangle *) malloc(sizeof(Triangle));
    initializeTriangle(topRight, g, 0, 0, 0, g.cols - 1, g.rows - 1, g.cols - 1);

    bottomLeft->t1 = topRight;
    bottomLeft->t2 = NULL:
    bottomLeft->t3 = NULL:

    topRight->t1 = bottomLeft;
    topRight->t2 = NULL:
    topRight->t3 = NULL:

    tin->triangle = upperLeft;

    // Priority queue for storing points and errors
    PriorityQueue* q = makeQueue();
    
    // Compute errors of all remaining grid points
    // For each triangle, add point with max error into priority queue
    int maxErrorBottomLeft = 0;
    Vertex v* vertexBottomLeft;
    LList* vListBottomLeft = LList_init();

    int maxErrorTopRight = 0;
    Vertex v* vertexTopRight;
    LList* vListTopRight = LList_init();

    for (row = 0; row < g->rows; row++) {
        for (col = 0; col < g->cols; col++) {
            if (get(g, row, col) != g->NODATA_value) {
                Vertex* v = (Vertex *) malloc(sizeof(Vertex));
                v->row = row;
                v->col = col;
                v->value = g->values[row][col];

                if (row > col) {
                    v->triangle = bottomLeft;
                    LList_insert_at_head(vListBottomLeft, (void *)v);

                    // TODO implement computeError
                    int error = computeError(bottomLeft, v);
                    if (error >= maxErrorBottomLeft) {
                        maxErrorBottomLeft = error;
                        vertexBottomLeft = v;
                    }
                } else {
                    v->triangle = topRight;
                    LList_insert_at_head(vListTopRight, (void *)v);

                    int error = computeError(topRight, v);
                    if (error >= maxErrorTopRight) {
                        maxErrorTopRight = error;
                        vertexTopRight = v;
                    }
                }
            }
        }
    }

    Node* nodeBottomLeft = makeNode(maxErrorBottomLeft, (void *)vertexBottomLeft);
    insert(q, nodeBottomLeft);

    Node* nodeTopRight = makeNode(maxErrorTopRight, (void *)vertexTopRight);
    insert(q, nodeTopRight);
    
    // Main algorithm
    Node* maxErrorNode = removeTop(q);
    while (maxErrorNode->priority > epsilon) {

        // Find point with largest error
        Vertex* maxErrorVertex = (Vertex *)maxErrorNode->item;

        // Add largest error point to TIN
        Triangle* containsLargestErrorVertex = maxErrorVertex->triangle;

        // TODO when will seg faults happen?
        // TODO double check this
        Triangle* newT1 = (Triangle *) malloc(sizeof(Triangle));
        Triangle* newT2 = (Triangle *) malloc(sizeof(Triangle));
        Triangle* newT3 = (Triangle *) malloc(sizeof(Triangle));

        // Initialize newT1
        newT1->v1 = maxErrorVertex;
        newT1->v2 = containsLargestErrorVertex->v1;
        newT1->v3 = containsLargestErrorVertex->v2;

        containsLargestErrorVertex->t1->t1 = newT1; // TODO check assumption that t->t1->t1 == t 
        newT1->t1 = containsLargestErrorVertex->t1;
        newT1->t2 = newT2;
        newT1->t3 = newT3;

        // Initialize newT2
        newT2->v1 = maxErrorVertex;
        newT2->v2 = containsLargestErrorVertex->v2;
        newT2->v3 = containsLargestErrorVertex->v3;

        containsLargestErrorVertex->t2->t2 = newT2;
        newT2->t1 = newT1;
        newT2->t2 = containsLargestErrorVertex->t2;
        newT2->t3 = newT3;

        // Initialize newT3
        newT3->v1 = maxErrorVertex;
        newT3->v2 = containsLargestErrorVertex->v1;
        newT3->v3 = containsLargestErrorVertex->v3;

        containsLargestErrorVertex->t3->t3 = newT3;
        newT3->t1 = newT1;
        newT3->t2 = newT2;
        newT3->t3 = containsLargestErrorVertex->t3;

        tin->triangle = newT1;

        // Compute errors of all points whose errors have changed
        // TODO implement triangleContains
        LList* vertices = containsLargestErrorVertex->vList;
        LNode* node = vertices->head; 

        int maxErrorT1 = 0;
        Vertex v* vertexT1;
        LList* vListT1 = LList_init();

        int maxErrorT2 = 0;
        Vertex v* vertexT2;
        LList* vListT2 = LList_init();

        int maxErrorT3 = 0;
        Vertex v* vertexT3;
        LList* vListT3 = LList_init();

        while (node != NULL) {
            Vertex* v = (Vertex *) node->value;

            if (triangleContains(newT1, v)) {
                v->triangle = newT1;
                LList_insert_at_head(vListT1, (void *)v);

                int error = computeError(newT1, v);
                if (error >= maxErrorT1) {
                    maxErrorT1 = error;
                    vertexT1 = v;
                }
            } else if (triangleContains(newT2, v)) {
                v->triangle = newT2;
                LList_insert_at_head(vListT1, (void *)v);

                int error = computeError(newT2, v);
                if (error >= maxErrorT1) {
                    maxErrorT1 = error;
                    vertexT1 = v;
                }
            } else if (triangleContains(newT3, v)) {
                v->triangle = newT3;
                LList_insert_at_head(vListT3, (void *)v);

                int error = computeError(newT3, v);
                if (error >= maxErrorT3) {
                    maxErrorT3 = error;
                    vertexT3 = v;
                }
            }
            
            node = node->next;
        }

        Node* nodeT1 = makeNode(maxErrorT1, (void *)vertexT1);
        insert(q, nodeT1);

        Node* nodeT2 = makeNode(maxErrorT2, (void *)vertexT2);
        insert(q, nodeT2);

        Node* nodeT3 = makeNode(maxErrorT3, (void *)vertexT3);
        insert(q, nodeT3);

        // After retriangulation, free old triangle
        free(containsLargestErrorVertex);

        // Set maxError to the error of the vertex with highest error in q
        maxErrorNode = removeTop(p);
    }
    
    return tin;
}

// The main method
// TODO
int main(int argc, char** args) 
{
    float epsilon = 0.0; //take from command line - TBU
    
    TIN tin;
    Grid imageGrid;
    
    //allocate blocks of memory for image data
    imageGrid.data = (int**) malloc(imageGrid.rows * sizeof(int*));
    if (imageGrid.data == NULL) {
        printf("Could not allocate array.");
        exit(1);
    }
    for (int i = 0; i < imageGrid.rows; i++) {
        imageGrid.data[i] = (int*) malloc(imageGrid.cols * sizeof(int));
        if (imageGrid.data[i] == NULL) {
            printf("Could not allocate array.");
            exit(1);
        }
    }
    
    //call simplification method
    TIN imageTIN = simplify(&tin, &imageGrid, epsilon);
    
    //free memory blocks allocated for image grid
    for (int i = 0; i < imageGrid.rows; i++)
        free(imageGrid.data[i]);
    free(imageGrid.data);
    
    //done
    return 0;
}
