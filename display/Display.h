#pragma once

#include "llist.h"


//#include "../simplification.h" // using this definition of a Triangle
typedef struct _vertex {
    int row, col, value;    // Value will become of type Color
} Vertex;

typedef struct _triangle {
    Vertex* v1;
    Vertex* v2;
    Vertex* v3;    // Vertices of triangle
    struct _triangle* t1;
    struct _triangle* t2;
    struct _triangle* t3;  // Adjacent triangles
    int visited;           // For drawing
} Triangle;

void displayTriangles(Triangle* head, int maxX, int maxY);
