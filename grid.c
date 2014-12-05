#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "grid.h"

int allocateGrid(Grid* grid, int nrows, int ncols)
{
    int i;
    
    if ((grid->color = malloc(nrows*sizeof(Color*))) == NULL)
        return 0;
    for (i = 0; i < nrows; i++) {
        if ((grid->color[i] = malloc(ncols*sizeof(Color))) == NULL)
            return 0;
    }
    
    return 1;
}

void freeGrid(Grid* grid)
{
    int i;
    
    for (i = 0; i < grid->nrows; i++) {
        free(grid->color[i]);
    }
    free(grid->color);
}

int readFileIntoGrid(Grid* grid, char* filename)
{
    int i, j;
    char buffer[99600];
    
    FILE* f;
    if ((f = fopen(filename, "r")) == NULL)
        return 0;
    fgetc(f);   // Skip over "ncols" label
    grid->ncols = fgetc(f);
    fgetc(f);   // Skip over "nrows" label
    grid->nrows = fgetc(f);
    if (!allocateGrid(grid, grid->nrows, grid->ncols))
        return 0;
    
    for (i = 0; i < grid.nrows; i++) {
        for (j = 0; j < grid.ncols; j++) {
            grid->color[i][j].red = fgetc(f);
            grid->color[i][j].green = fgetc(f);
            grid->color[i][j].blue = fgetc(f);
        }
    }
    
    return 1;
}

int saveGridToFile(Grid* grid, char* filename)
{
    int i, j;
    
    FILE* f;
    if ((f = fopen(filename, "w")) == NULL)
        return 0;
    
    fprintf(f, "ncols %d\n", grid->ncols);
    fprintf(f, "nrows %d\n", grid->nrows);

    
    for (i = 0; i < grid->nrows; i++) {
        for (j = 0; j < grid->ncols; j++) {
            fprintf(f, "%d ", grid->color[i][j].red);
            fprintf(f, "%d ", grid->color[i][j].green);
            fprintf(f, "%d ", grid->color[i][j].blue);
        }
        fprintf(f, "\n");
    }
    
    freeGrid(grid);
    return 1;
}

void copyGridHeader(Grid* to, Grid* from)
{
    to->ncols = from->ncols;
    to->nrows = from->nrows;
}

void printGrid(Grid* grid)
{
    int i, j;
    
    printf("Printing grid with %d cols and %d rows.\n", grid->ncols, grid->nrows);
    for (i = 0; i < grid->nrows; i++) {
        for (j = 0; j < grid->ncols; j++) {
            printf("%d ", grid->color[i][j]);
        }
        printf("\n");
    }
}

int inBoundsOnGrid(Grid* grid, int i, int j)
{
    return (i >= 0 && j >= 0 && i < grid->nrows && j < grid->ncols);
}