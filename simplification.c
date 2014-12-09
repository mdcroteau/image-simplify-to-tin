//File: simplification.c
//Authors: Croteau, Imhoff, Zeller
//Date: 11/09/14

#include "simplification.h"
#include "pqueue.h"
#include "grid.h"
#include "display.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

void initializeTriangle(Triangle *t, Grid* g, int row1, int col1, int row2, int col2, int row3, int col3) 
{
    // First vertex
    t->v1 = (Vertex *) malloc(sizeof(Vertex));
    t->v1->row = row1;
    t->v1->col = col1;
    t->v1->value = get(g, row1, col1);
    
    // Second vertex
    t->v2 = (Vertex *) malloc(sizeof(Vertex));
    t->v2->row = row2;
    t->v2->col = col2;
    t->v2->value = get(g, row2, col2);
    
    // Third vertex
    t->v3 = (Vertex *) malloc(sizeof(Vertex));
    t->v3->row = row3;
    t->v3->col = col3;
    t->v3->value = get(g, row3, col3);
}

double linearlyInterpolate(Vertex* a, Vertex* b, Vertex* c, int row, int col) 
{
    double abx = b->col - a->col;
    double aby = b->row - a->row;
    double abz = b->value - a->value;

    double acx = c->col - a->col;
    double acy = c->row - a->row;
    double acz = c->value - a->value;

    double crossx = aby*acz - abz*acy;
    double crossy = abz*acx - abx*acz;
    double crossz = abx*acy - aby*acx;
    double d = -(crossx*a->col + crossy*a->row + crossz*a->value);

    return (-crossx*col - crossy*row - d) / crossz;
}

double computeError(Grid* g, Triangle* t, Vertex* v)
{
    Vertex* v1 = t->v1;
    Vertex* v2 = t->v2;
    Vertex* v3 = t->v3;

    double fromTin = linearlyInterpolate(v1, v2, v3, v->row, v->col);
    printf("col, %d, row, %d, value, %d\n", v->col, v->row, get(g, v->row, v->col));
    printf("fromTin: %f\n", fromTin);
    double toReturn = abs(fromTin - (double)get(g, v->row, v->col));
    printf("toReturn: %f\n", toReturn);
    return toReturn;
}

float triangleArea(Vertex* v1, Vertex* v2, Vertex* v3)
{
    return abs(v1->col * (v2->row - v3->row) + v2->col * (v3->row - v1->row) +
               v3->col * (v1->row - v2->row)) / 2.0;
}

// TODO float equals
int triangleContains(Triangle* t, Vertex* v)
{
    /* float epsilon = 0.005; */
    float triArea = triangleArea(t->v1, t->v2, t->v3);
    float areaWithPoint = (triangleArea(t->v1, t->v2, v) +
                           triangleArea(t->v2, t->v3, v) +
                           triangleArea(t->v3, t->v1, v));
    /* if (fabs(triArea - areaWithPoint) < triArea * epsilon) */
    if (triArea == areaWithPoint)
        return 1;
    return 0;
}

// TODO refactor and document
TIN* simplify(TIN* tin, Grid* g, double epsilon) 
{
    // Initialize TIN with 4 corner points
    Triangle* bottomLeft = (Triangle *) malloc(sizeof(Triangle));
    initializeTriangle(bottomLeft, g, 0, 0, g->rows - 1, 0, g->rows - 1, g->cols - 1);

    Triangle* topRight = (Triangle *) malloc(sizeof(Triangle));
    initializeTriangle(topRight, g, 0, 0, 0, g->cols - 1, g->rows - 1, g->cols - 1);

    bottomLeft->t1 = NULL;
    bottomLeft->t2 = NULL;
    bottomLeft->t3 = topRight;

    topRight->t1 = NULL;
    topRight->t2 = NULL;
    topRight->t3 = bottomLeft;

    tin->triangle = bottomLeft;

    // Priority queue for storing points and errors
    PriorityQueue* q = makeQueue();
    
    // Compute errors of all remaining grid points
    // For each triangle, add point with max error into priority queue
    double maxErrorBottomLeft = -1;
    Vertex* vertexBottomLeft = 0;
    LList* vListBottomLeft = LList_init();

    double maxErrorTopRight = -1;
    Vertex* vertexTopRight = 0;
    LList* vListTopRight = LList_init();

    for (int row = 0; row < g->rows; row++) {
        for (int col = 0; col < g->cols; col++) {
            if (!((row == 0 && col == 0) ||
                (row == 0 && col == g->cols-1) ||
                (row == g->rows-1 && col == 0) ||
                (row == g->rows-1 && col == g->cols-1))) {

                Vertex* v = (Vertex *) malloc(sizeof(Vertex));
                v->row = row;
                v->col = col;
                v->value = get(g, row, col);

                if (row > col) {
                    v->triangle = bottomLeft;
                    LList_insert_at_head(vListBottomLeft, (void *)v);

                    double error = computeError(g, bottomLeft, v);
                    if (error >= maxErrorBottomLeft) {
                        maxErrorBottomLeft = error;
                        vertexBottomLeft = v;
                    }
                } else {
                    v->triangle = topRight;
                    LList_insert_at_head(vListTopRight, (void *)v);

                    double error = computeError(g, topRight, v);
                    if (error >= maxErrorTopRight) {
                        maxErrorTopRight = error;
                        vertexTopRight = v;
                    }
                }
            }
        }
    }

    bottomLeft->vList = vListBottomLeft;
    topRight->vList = vListTopRight;

    Node* nodeBottomLeft = makeNode(maxErrorBottomLeft, (void *)vertexBottomLeft);
    insert(q, nodeBottomLeft);

    Node* nodeTopRight = makeNode(maxErrorTopRight, (void *)vertexTopRight);
    insert(q, nodeTopRight);
    
    // Main algorithm
    Node* maxErrorNode = removeTop(q);
    while (maxErrorNode->priority > epsilon) {
        // Find point with largest error
        Vertex* maxErrorVertex = (Vertex *) maxErrorNode->item;
        assert(maxErrorVertex);
        free(maxErrorNode);
        printf("1\n");

        // Add largest error point to TIN
        Triangle* containsLargestErrorVertex = maxErrorVertex->triangle;
        assert(containsLargestErrorVertex);
        printf("2\n");

        Triangle* newT1 = (Triangle *) malloc(sizeof(Triangle));
        Triangle* newT2 = (Triangle *) malloc(sizeof(Triangle));
        Triangle* newT3 = (Triangle *) malloc(sizeof(Triangle));

        // Initialize newT1
        newT1->v1 = containsLargestErrorVertex->v1;
        newT1->v2 = containsLargestErrorVertex->v2;
        newT1->v3 = maxErrorVertex;
        printf("3\n");

        if (containsLargestErrorVertex->t1 != NULL)
            containsLargestErrorVertex->t1->t1 = newT1;
        newT1->t1 = containsLargestErrorVertex->t1;
        newT1->t2 = newT2;
        newT1->t3 = newT3;
        printf("4\n");

        // Initialize newT2
        newT2->v1 = maxErrorVertex;
        newT2->v2 = containsLargestErrorVertex->v2;
        newT2->v3 = containsLargestErrorVertex->v3;
        printf("5\n");

        if (containsLargestErrorVertex->t2 != NULL)
            containsLargestErrorVertex->t2->t2 = newT2;
        newT2->t1 = newT1;
        newT2->t2 = containsLargestErrorVertex->t2;
        newT2->t3 = newT3;
        printf("6\n");

        // Initialize newT3
        newT3->v1 = containsLargestErrorVertex->v1;
        newT3->v2 = maxErrorVertex;
        newT3->v3 = containsLargestErrorVertex->v3;
        printf("7\n");

        if (containsLargestErrorVertex->t3 != NULL)
            containsLargestErrorVertex->t3->t3 = newT3;
        newT3->t1 = newT1;
        newT3->t2 = newT2;
        newT3->t3 = containsLargestErrorVertex->t3;
        printf("8\n");

        tin->triangle = newT1;

        // Compute errors of all points whose errors have changed
        LList* vertices = containsLargestErrorVertex->vList;
        assert(containsLargestErrorVertex->vList);
        LNode* node = vertices->head; 
        printf("9\n");

        double maxErrorT1 = -1;
        Vertex* vertexT1 = 0;
        LList* vListT1 = LList_init();

        double maxErrorT2 = -1;
        Vertex* vertexT2 = 0;
        LList* vListT2 = LList_init();

        double maxErrorT3 = -1;
        Vertex* vertexT3 = 0;
        LList* vListT3 = LList_init();

        printf("10\n");
        while (node != NULL) {
            Vertex* v = (Vertex *) node->item;

            if (triangleContains(newT1, v)) {
                v->triangle = newT1;
                LList_insert_at_head(vListT1, (void *)v);

                double error = computeError(g, newT1, v);
                if (error >= maxErrorT1) {
                    maxErrorT1 = error;
                    vertexT1 = v;
                }
            } else if (triangleContains(newT2, v)) {
                v->triangle = newT2;
                LList_insert_at_head(vListT2, (void *)v);

                double error = computeError(g, newT2, v);
                if (error >= maxErrorT2) {
                    maxErrorT2 = error;
                    vertexT2 = v;
                }
            } else if (triangleContains(newT3, v)) {
                v->triangle = newT3;
                LList_insert_at_head(vListT3, (void *)v);

                double error = computeError(g, newT3, v);
                if (error >= maxErrorT3) {
                    maxErrorT3 = error;
                    vertexT3 = v;
                }
            }
            
            node = node->next;
        }
        printf("11\n");

        newT1->vList = vListT1;
        newT2->vList = vListT2;
        newT3->vList = vListT3;

        Node* nodeT1 = makeNode(maxErrorT1, (void *)vertexT1);
        insert(q, nodeT1);

        Node* nodeT2 = makeNode(maxErrorT2, (void *)vertexT2);
        insert(q, nodeT2);

        Node* nodeT3 = makeNode(maxErrorT3, (void *)vertexT3);
        insert(q, nodeT3);

        // After retriangulation, free old triangle
        LList_free(containsLargestErrorVertex->vList);
        free(containsLargestErrorVertex);

        // Set maxError to the error of the vertex with highest error in q
        maxErrorNode = removeTop(q);
        printf("12\n");
    }
    
    return tin;
}

// The main method
int main(int argc, char** argv)
{
    // TODO check arguments

    Grid* g = (Grid *) malloc(sizeof(Grid));
    readFileIntoGrid(g, argv[1]);

    printGrid(g);

    // Testing linearlyInterpolation
    /* Vertex v1; */
    /* Vertex v2; */
    /* Vertex v3; */

    /* v1.row = 5; */
    /* v1.col = 5; */
    /* v1.value = 0; */
    /* v2.row = 5; */
    /* v2.col = 25; */
    /* v2.value = 20; */
    /* v3.row = 25; */
    /* v3.col = 5; */
    /* v3.value = 20; */

    /* double interpolation = linearlyInterpolate(&v1, &v2, &v3, 15, 15); */
    /* printf("Testing linearlyInterpolation: %f\n", interpolation); */

    // Testing triangleContains
    /* v1.row = 5; */
    /* v1.col = 5; */
    /* v2.row = 5; */
    /* v2.col = 25; */
    /* v3.row = 25; */
    /* v3.col = 5; */

    /* Vertex v4; */
    /* v4.row = 15; */
    /* v4.col = 15; */

    /* Triangle t; */
    /* t.v1 = &v1; */
    /* t.v2 = &v2; */
    /* t.v3 = &v3; */

    /* int contains = triangleContains(&t, &v4); */
    /* printf("Testing triangleContains: %d\n", contains); */

    // Run grid-to-tin simplifier
    TIN* tin = (TIN *) malloc(sizeof(TIN));
    tin = simplify(tin, g, atof(argv[2]));

    // Display TIN
    displayTriangles(tin->triangle, g->cols, g->rows);

    return 0;
}
