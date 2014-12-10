//File: simplification.h
//Authors: Croteau, Imhoff, Zeller
//Date: 11/09/14

#pragma once

#include "llist.h"
#include "grid.h"

struct _triangle;

typedef struct _vertex {
    int row, col;
    Color color;
    struct _triangle *triangle;      // Triangle containing vertex
} Vertex;

typedef struct _triangle {
    Vertex *v1, *v2, *v3;            // Vertices of triangle
    struct _triangle *t1, *t2, *t3;  // Adjacent triangles
    LList* vList;                    // Linked list of vertices contained in triangle 
    int visited;                     // For drawing
} Triangle;

typedef struct _tin {
    Triangle* triangle;              // Any triangle in the TIN
} TIN;
