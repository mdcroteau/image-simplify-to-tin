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

void initialTriangulation(TIN* tin, Grid* g)
{
    // Top left vertex
    Vertex* v1 = (Vertex *) malloc(sizeof(Vertex));
    v1 = (Vertex *) malloc(sizeof(Vertex));
    v1->row = 0;
    v1->col = 0;
    v1->value = get(g, v1->row, v1->col);

    // Top right vertex
    Vertex* v2 = (Vertex *) malloc(sizeof(Vertex));
    v2 = (Vertex *) malloc(sizeof(Vertex));
    v2->row = 0;
    v2->col = g->cols - 1;
    v2->value = get(g, v2->row, v2->col);

    // Bottom left vertex
    Vertex* v3 = (Vertex *) malloc(sizeof(Vertex));
    v3 = (Vertex *) malloc(sizeof(Vertex));
    v3->row = g->rows - 1;
    v3->col = 0;
    v3->value = get(g, v3->row, v3->col);

    // Bottom right vertex
    Vertex* v4 = (Vertex *) malloc(sizeof(Vertex));
    v4 = (Vertex *) malloc(sizeof(Vertex));
    v4->row = g->rows - 1;
    v4->col = g->cols - 1;
    v4->value = get(g, v4->row, v4->col);

    // Bottom left triangle
    Triangle* bottomLeft = (Triangle *) malloc(sizeof(Triangle));
    bottomLeft->v1 = v1;
    bottomLeft->v2 = v3;
    bottomLeft->v3 = v4;

    // Top right triangle
    Triangle* topRight = (Triangle *) malloc(sizeof(Triangle));
    topRight->v1 = v1;
    topRight->v2 = v2;
    topRight->v3 = v4;

    // Link triangle together
    bottomLeft->t3 = topRight;
    topRight->t3 = bottomLeft;

    // Initialize tin
    tin->triangle = bottomLeft;
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
    /* printf("col, %d, row, %d, value, %d\n", v->col, v->row, get(g, v->row, v->col)); */
    /* printf("fromTin: %f\n", fromTin); */
    double toReturn = abs(fromTin - (double)get(g, v->row, v->col));
    /* printf("toReturn: %f\n", toReturn); */
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

void connectThirdNeighborToInsideTriangle(Triangle* containing, 
                                          Triangle* neighbor, 
                                          Triangle* inside)
{
    if (neighbor->t1 == containing)
        neighbor->t1 = inside;
    else if (neighbor->t2 == containing)
        neighbor->t2 = inside;
    else // if (neighbor->t3 == containing)
        neighbor->t3 = inside;
}

int triangleHasTwoVertices(Triangle* t, Vertex* v1, Vertex* v2)
{
    return (t && ((v1 == t->v1 && v2 == t->v2) || (v1 == t->v1 && v2 == t->v3) ||
            (v1 == t->v2 && v2 == t->v1) || (v1 == t->v2 && v2 == t->v3) ||
            (v1 == t->v3 && v2 == t->v1) || (v1 == t->v3 && v2 == t->v2)));
}

void connectTriangleToThirdNeighbor(Triangle* containing, Triangle* inside)
{
    if (triangleHasTwoVertices(containing->t1, inside->v1, inside->v2)) {
        inside->t3 = containing->t1;
        connectThirdNeighborToInsideTriangle(containing, containing->t1, inside);
    }
    else if (triangleHasTwoVertices(containing->t2, inside->v1, inside->v2)) {
        inside->t3 = containing->t2;
        connectThirdNeighborToInsideTriangle(containing, containing->t2, inside);
    }
    else if (triangleHasTwoVertices(containing->t3, inside->v1, inside->v2)) {
        inside->t3 = containing->t3;
        connectThirdNeighborToInsideTriangle(containing, containing->t3, inside);
    }
    else {
        inside->t3 = NULL;
    }
}

void splitTriangle(TIN* tin, Triangle* t, Vertex* v)
{
    // Allocate memory for new triangles
    Triangle* t1 = (Triangle *) malloc(sizeof(Triangle));
    Triangle* t2 = (Triangle *) malloc(sizeof(Triangle));
    Triangle* t3 = (Triangle *) malloc(sizeof(Triangle));

    // Initialize t1
    t1->v1 = t->v1;
    t1->v2 = t->v2;
    t1->v3 = v;

    t1->t1 = t2;
    t1->t2 = t3;

    // Initialize t2
    t2->v1 = t->v2;
    t2->v2 = t->v3;
    t2->v3 = v;

    t2->t1 = t1;
    t2->t2 = t3;

    // Initialize t3
    t3->v1 = t->v1;
    t3->v2 = t->v3;
    t3->v3 = v;

    t3->t1 = t1;
    t3->t2 = t2;

    // Connect t1 to its third neighboring triangle
    connectTriangleToThirdNeighbor(t, t1);
    connectTriangleToThirdNeighbor(t, t2);
    connectTriangleToThirdNeighbor(t, t3);

    // Make sure that TIN points to a triangle still part of the TIN
    tin->triangle = t1;
}

// TODO refactor and document
TIN* simplify(TIN* tin, Grid* g, double epsilon) 
{
    // Initialize TIN with 4 corner points
    initialTriangulation(tin, g);

    Triangle* bottomLeft = tin->triangle;
    Triangle* topRight = bottomLeft->t3;

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
    int i = 0;
    while (maxErrorNode->priority > epsilon) {
        // Find point with largest error
        Vertex* maxErrorVertex = (Vertex *) maxErrorNode->item;
        assert(maxErrorVertex);
        free(maxErrorNode);

        // Add largest error point to TIN
        Triangle* containsLargestErrorVertex = maxErrorVertex->triangle;
        assert(containsLargestErrorVertex);

        // Retriangulate
        splitTriangle(tin, containsLargestErrorVertex, maxErrorVertex);

        Triangle* newT1 = tin->triangle;
        Triangle* newT2 = newT1->t1;
        Triangle* newT3 = newT1->t2;

        // Compute errors of all points whose errors have changed
        LList* vertices = containsLargestErrorVertex->vList;
        assert(containsLargestErrorVertex->vList);
        LNode* node = vertices->head; 

        double maxErrorT1 = -1;
        Vertex* vertexT1 = 0;
        LList* vListT1 = LList_init();

        double maxErrorT2 = -1;
        Vertex* vertexT2 = 0;
        LList* vListT2 = LList_init();

        double maxErrorT3 = -1;
        Vertex* vertexT3 = 0;
        LList* vListT3 = LList_init();

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

        // TODO for debug
        if (i == 15)
            displayTriangles(tin->triangle, g->cols, g->rows);
        i++;
    }
    
    return tin;
}

// The main method
int main(int argc, char** argv)
{
    if (argc != 3) {
        printf("USAGE simplify [grid filename] [epsilon (double)]\n");
        return -1;
    }

    Grid* g = (Grid *) malloc(sizeof(Grid));
    readFileIntoGrid(g, argv[1]);

    /* printGrid(g); */

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
