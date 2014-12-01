//File: simplification.c
//Authors: Croteau, Imhoff, Zeller
//Date: 11/09/14

#include "simplification.h"
#include <stdlib.h>

//This method sets the element at (row, col) in a grid to value.
void set(Grid *g, int row, int col, int value) {g->data[row][col] = value;}


//This method gets the value at (row, col) in a grid.
int get(Grid g, int row, int col) {return g.data[row][col];}


void initializeTriangle(Triangle * t, Grid g, int row1, int col1, int row2, int col2, int row3, int col3) {
    
    //first vertex
    t.triangle.v1 = (Vertex *) malloc(sizeof(Vertex));
    t.triangle.v1.row = row1;
    t.triangle.v1.col = col1;
    t.triangle.v1.value = get(g, row1, col1);
    set(g, row1, col1, NULL);
    
    //second vertex
    t.triangle.v2 = (Vertex *) malloc(sizeof(Vertex));
    t.triangle.v2.row = row2;
    t.triangle.v2.col = col2;
    t.triangle.v2.value = get(g, row2, col2);
    set(g, row2, col2, NULL);
    
    //third vertex
    t.triangle.v3 = (Vertex *) malloc(sizeof(Vertex));
    t.triangle.v3.row = row3;
    t.triangle.v3.col = col3;
    t.triangle.v3.value = get(g, row3, col3);
    set(g, row3, col3, NULL);
}


//TBU
float getMaxError() {
    
    float maxError = 0.0;

    
    return maxError;
}


//This method simplifies a grid to a TIN.
TIN simplify(Grid g, float epsilon) {
    
    //initialize TIN of 4 corner points
    TIN tin;
    tin.triangle = (Triangle *) malloc(sizeof(Triangle));
    Triangle * newTriangle = (Triangle *) malloc(sizeof(Triangle));
    //upper left triangle
    initializeTriangle(tin.triangle, g, 0, 0, 0, g.cols - 1, g.rows - 1, 0);
    //bottom right triangle
    initializeTriangle(newTriangle, g, g.rows - 1, g.cols - 1, 0, g.cols - 1, g.rows - 1, 0);
    //have triangles point to each other
    tin.triangle.t1 = newTriangle;
    newTriangle.t1 = tin.triangle;
    
    
    //compute errors of all remaining grid points
    for (row = 0; row < g.rows; row++)
        for (col = 0; col < g.cols; col++)
            if (get(g, row, col) != NULL)
                //computeError(tin, g, row, col);
    
    while (getMaxError() > epsilon) {
        //find point with largest error
        
        //add largest error point to TIN and remove from remaining points
        
        //compute errors of all points whose errors have changed
    }
    
    return tin;
}


//The main method.
int main(int argc, char** args) {
    
    float epsilon = 0.0; //take from command line - TBU
    
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
    TIN imageTIN = simplify(imageGrid, epsilon);
    
    //free memory blocks allocated for image grid
    for (int i = 0; i < imageGrid.rows; i++)
        free(imageGrid.data[i]);
    free(imageGrid.data);
    
    //done
    return 0;
}
