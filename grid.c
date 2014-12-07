#include "grid.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int allocateGrid(Grid* grid, int rows, int cols)
{
    int i;

    if ((grid->values = malloc(rows*sizeof(int *))) == NULL)
        return 0;
    for (i = 0; i < rows; i++) {
        if ((grid->values[i] = malloc(cols*sizeof(int))) == NULL)
            return 0;
    }

    return 1;
}

void freeGrid(Grid* grid)
{
    int i;

    for (i = 0; i < grid->rows; i++) {
        free(grid->values[i]);
    }
    free(grid->values);
}

int readFileIntoGrid(Grid* grid, char* filename)
{
    int i, j;

    FILE* f;
    if ((f = fopen(filename, "r")) == NULL)
        return 0;

    fscanf(f, "cols %d\n", &grid->cols);
    fscanf(f, "rows %d\n", &grid->rows);

    if (!allocateGrid(grid, grid->rows, grid->cols))
        return 0;

    fscanf(f, "xllcorner %d\n", &grid->xllcorner);
    fscanf(f, "yllcorner %d\n", &grid->yllcorner);
    fscanf(f, "cellsize %d\n", &grid->cellsize);
    fscanf(f, "NODATA_value %d\n", &grid->NODATA_value);

    for (i = 0; i < grid->rows; i++) {
        for (j = 0; j < grid->cols; j++) {
            fscanf(f, "%d", &(grid->values[i][j]));
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

    fprintf(f, "cols %d\n", grid->cols);
    fprintf(f, "rows %d\n", grid->rows);
    fprintf(f, "xllcorner %d\n", grid->xllcorner);
    fprintf(f, "yllcorner %d\n", grid->yllcorner);
    fprintf(f, "cellsize %d\n", grid->cellsize);
    fprintf(f, "NODATA_value %d\n", grid->NODATA_value);

    for (i = 0; i < grid->rows; i++) {
        for (j = 0; j < grid->cols; j++) {
            fprintf(f, "%d ", grid->values[i][j]);
        }
        fprintf(f, "\n");
    }

    freeGrid(grid);
    return 1;
}

void copyGridHeader(Grid* to, Grid* from)
{
    to->cols = from->cols;
    to->rows = from->rows;
    to->xllcorner = from->xllcorner;
    to->yllcorner = from->yllcorner;
    to->cellsize = from->cellsize;
    to->NODATA_value = from->NODATA_value;
}

void printGrid(Grid* grid)
{
    int i, j;

    printf("Printing grid with %d cols and %d rows.\n", grid->cols, grid->rows);
    for (i = 0; i < grid->rows; i++) {
        for (j = 0; j < grid->cols; j++) {
            printf("%d ", grid->values[i][j]);
        }
        printf("\n");
    }
}

int inBoundsOnGrid(Grid* grid, int i, int j)
{
    return (i >= 0 && j >= 0 && i < grid->rows && j < grid->cols);
}

int noDataAtPoint(Grid* grid, int i, int j)
{
    return (grid->NODATA_value == grid->values[i][j]);
}

void set(Grid* g, int row, int col, int value) 
{ 
    g->values[row][col] = value; 
}

int get(Grid* g, int row, int col)
{ 
    return g->values[row][col]; 
}
