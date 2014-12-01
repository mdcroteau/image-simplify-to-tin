//File: simplification.h
//Authors: Croteau, Imhoff, Zeller
//Date: 11/09/14


typedef struct _grid {
    int rows, cols;     //dimensions of grid
    int** data;         //2D array of values in grid
} Grid;


typedef struct _color {
    int r, g, b;
} Color;


typedef struct _vertex {
    int row, col, value;    ///value will become Color///
} Vertex;


typedef struct _triangle {
    Vertex * v1, v2, v3;    //vertices of triangle
    Triangle * t1, t2, t3;  //adjacent triangles
} Triangle;


typedef struct _tin {
    Triangle * triangle;     //any triangle in the TIN
} TIN;


//This method sets the element at (row, col) in a grid to value.
void set(Grid *g, int row, int col, int value);     ///will set a Color///

//This method gets the value at (row, col) in a grid.
int get(Grid g, int row, int col);      ///will get a Color///
