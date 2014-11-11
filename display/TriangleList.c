#include <stdlib.h>

#include "TriangleList.h"

Color* buildColor(float red, float green, float blue)
{
  Color* color = malloc(sizeof(Color));
  color->r = red;
  color->g = green;
  color->b = blue;
  return color;
}

Point* buildPoint(int x, int y)
{
  Point* point = malloc(sizeof(Point));
  point->x = x;
  point->y = y;
  return point;
}

TriangleNode* initTriangle()
{
  TriangleNode* triangle = malloc(sizeof(TriangleNode));
  // Set the contents to a known state
  triangle->oneP   = NULL;
  triangle->oneC   = NULL;
  triangle->twoP   = NULL;
  triangle->twoC   = NULL;
  triangle->threeP = NULL;
  triangle->threeC = NULL;
  triangle->next   = NULL;

  return triangle;
}

void freeTriangle(TriangleNode* node, int freeContents)
{
  if(freeContents){
    free(node->oneP);
    free(node->oneC);
    free(node->twoP);
    free(node->twoC);
    free(node->threeP);
    free(node->threeC);
  }
  free(node);
}

void freeList(TriangleNode* head, int freeContents)
{
  TriangleNode* toFree = head;

  while(toFree->next != NULL){
    TriangleNode* temp = toFree->next;
    freeTriangle(toFree, freeContents);
    toFree = temp;
  }

  freeTriangle(toFree, freeContents);
}
