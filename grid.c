#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int allocateGrid(Grid* grid, int nrows, int ncols)
{
    int i;

    if ((grid->values = malloc(nrows*sizeof(int *))) == NULL)
        return 0;
    for (i = 0; i < nrows; i++) {
        if ((grid->values[i] = malloc(ncols*sizeof(int))) == NULL)
            return 0;
    }

    return 1;
}

void freeGrid(Grid* grid)
{
    int i;

    for (i = 0; i < grid->nrows; i++) {
        free(grid->values[i]);
    }
    free(grid->values);
}

int readFileIntoGrid(Grid* grid, char* filename)
{
    int i, j;
    char buffer[99600];

    FILE* f;
    if ((f = fopen(filename, "r")) == NULL)
        return 0;

    grid->ncols = 332; // TODO shouldn't be hard coded
    grid->nrows = 300;
    if (!allocateGrid(grid, grid->nrows, grid->ncols))
        return 0;

    for (i = 0; i < grid.nrows; i++) {
        for (j = 0; j < grid.ncols; j++) {
            grid->values[i][j] = fgetc(f);
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
    fprintf(f, "xllcorner %d\n", grid->xllcorner);
    fprintf(f, "yllcorner %d\n", grid->yllcorner);
    fprintf(f, "cellsize %d\n", grid->cellsize);
    fprintf(f, "NODATA_value %d\n", grid->NODATA_value);

    for (i = 0; i < grid->nrows; i++) {
        for (j = 0; j < grid->ncols; j++) {
            fprintf(f, "%d ", grid->values[i][j]);
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
    to->xllcorner = from->xllcorner;
    to->yllcorner = from->yllcorner;
    to->cellsize = from->cellsize;
    to->NODATA_value = from->NODATA_value;
}

void printGrid(Grid* grid)
{
    int i, j;

    printf("Printing grid with %d cols and %d rows.\n", grid->ncols, grid->nrows);
    for (i = 0; i < grid->nrows; i++) {
        for (j = 0; j < grid->ncols; j++) {
            printf("%d ", grid->values[i][j]);
        }
        printf("\n");
    }
}

int inBoundsOnGrid(Grid* grid, int i, int j)
{
    return (i >= 0 && j >= 0 && i < grid->nrows && j < grid->ncols);
}

int noDataAtPoint(Grid* grid, int i, int j)
{
    return (grid->NODATA_value == grid->values[i][j]);
}
