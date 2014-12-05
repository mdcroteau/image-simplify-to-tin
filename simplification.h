//File: simplification.h
//Authors: Croteau, Imhoff, Zeller
//Date: 11/09/14

// TODO use Daniel's generic LList
#include "llist.h"

typedef struct _grid {
    int rows, cols;
    int xllcorner, yllcorner, cellsize, NODATA_value;
    int** data;
} Grid;

// typedef struct _color {
//     int r, g, b;
// } Color;

typedef struct _vertex {
    int row, col, value;    // Value will eventually be of type Color
    Triange* triangle;      // Triangle containing vertex
} Vertex;

// t1 shares edge v1-v2, t2 shares edge v2-v3, t3 shares edge v3-v1
typedef struct _triangle {
    Vertex* v1, v2, v3;    // Vertices of triangle
    Triangle* t1, t2, t3;  // Adjacent triangles
    LList* vList;          // Linked list of vertices contained in triangle 
    int visited;           // For drawing
} Triangle;

typedef struct _tin {
    Triangle* triangle;    // Any triangle in the TIN
} TIN;

// Sets grid element at (row, col) to value
void set(Grid* g, int row, int col, int value) { g->data[row][col] = value; }

// Gets rid element at (row, col)
int get(Grid g, int row, int col) { return g.data[row][col]; }
