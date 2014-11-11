#pragma once

typedef struct color_ {
    float r;
    float g;
    float b;
} Color;

typedef struct point_ {
    int x;
    int y;
} Point;

typedef struct triangle_ {
    Point* oneP;
    Color* oneC;
    Point* twoP;
    Color* twoC;
    Point* threeP;
    Color* threeC;
    struct triangle_* next;
} TriangleNode;


Color* buildColor(float red, float green, float blue);

Point* buildPoint(int x, int y);

TriangleNode* initTriangle();

void freeTriangle(TriangleNode* node, int freeContents);

void freeList(TriangleNode* head, int freeContents);

