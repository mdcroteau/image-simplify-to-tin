#include "grid.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int allocateGrid(Grid* grid, int rows, int cols)
{
    int i;

    if ((grid->color = malloc(rows*sizeof(Color *))) == NULL)
        return 0;
    for (i = 0; i < rows; i++) {
        if ((grid->color[i] = malloc(cols*sizeof(Color))) == NULL)
            return 0;
    }

    return 1;
}

void freeGrid(Grid* grid)
{
    int i;

    for (i = 0; i < grid->rows; i++) {
        free(grid->color[i]);
    }
    free(grid->color);
}

int readFileIntoGrid(Grid* grid, char* filename)
{
    int i, j;

    FILE* f;
    if ((f = fopen(filename, "r")) == NULL)
        return 0;

    fscanf(f, "ncols %d\n", &grid->cols);
    fscanf(f, "nrows %d\n", &grid->rows);

    if (!allocateGrid(grid, grid->rows, grid->cols))
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

    fprintf(f, "ncols %d\n", grid->cols);
    fprintf(f, "nrows %d\n", grid->rows);

    for (i = 0; i < grid->rows; i++) {
        for (j = 0; j < grid->cols; j++) {
            fprintf(f, "%d %d %d ", grid->color[i][j].red, grid->color[i][j].green, grid->color[i][j].blue);
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
}

void printGrid(Grid* grid)
{
    int i, j;

    printf("Printing grid with %d cols and %d rows.\n", grid->cols, grid->rows);
    for (i = 0; i < grid->rows; i++) {
        for (j = 0; j < grid->cols; j++) {
            printf("(%dR %dG %dB) ", grid->color[i][j].red, grid->color[i][j].green, grid->color[i][j].blue);
        }
        printf("\n");
    }
}

int inBoundsOnGrid(Grid* grid, int i, int j)
{
    return (i >= 0 && j >= 0 && i < grid->rows && j < grid->cols);
}


void set(Grid* g, int row, int col, Color c)
{ 
    g->color[row][col].red = c.red;
    g->color[row][col].green = c.green;
    g->color[row][col].blue = c.blue;
}

Color get(Grid* g, int row, int col)
{ 
    return g->color[row][col];
}
