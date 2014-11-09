//File: simplification.h
//Authors: Croteau, Imhoff, Zeller
//Date: 11/09/14

//define the struct Grid
typedef struct _grid {
  int rows, cols;   //dimensions of grid
  int** data;     //2D array of values in grid
} Grid;

//This method sets the element at (row, col) in a grid to value.
void set(Grid *g, int row, int col, int value);

//This method gets the value at (row, col) in a grid.
float get(Grid g, int row, int col);
