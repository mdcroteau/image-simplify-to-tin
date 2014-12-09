#include <stdlib.h>
#include <stdio.h>

#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#include "display.h"

LList* list;
double imageAspectRatio;
// Image height & width
int iHeight, iWidth;

// Maximum value that an x/y value can be mapped to, given
// current aspect ratio of window
double yScaledMax, xScaledMax;

void renderScene(void);
float mapY(int imageY);
float mapX(int imageX);

// Performs DFS on the network of triangles, turning them into a linked list
void turnIntoList(LList* list, Triangle* node)
{
  // Already part of list
  if(node == NULL || node->visited){
    return;
  }

  // Add to end of list, mark as visited
  node->visited = 1;
  LList_insert_at_tail(list, node);

  // Continue searching, from attached triangles
  turnIntoList(list, node->t1);
  turnIntoList(list, node->t2);
  turnIntoList(list, node->t3);


}

void displayTriangles(Triangle* head, int maxX, int maxY)
{
  list = LList_init();

  imageAspectRatio = ((double)maxX)/maxY;
  iHeight = maxY;
  iWidth = maxX;

  turnIntoList(list, head);
  printf("How many triangles? %d\n", list->size);

  int dummy_argc = 1;
  char** dummy_argv = malloc(2 * sizeof(char*));
  dummy_argv[0] = "Simplified Image";
  dummy_argv[1] = NULL;

  glutInit(&dummy_argc, dummy_argv);
  glutInitWindowPosition(-1, -1);
  glutInitWindowSize(500, 500);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutCreateWindow("Simplified Image");

  glutDisplayFunc(renderScene);

  glutMainLoop();

}

void buildMapping(int windowX, int windowY)
{
  double windowAspect = ((double)windowX)/windowY;

  // If the window's aspect ratio is less than the images it means
  // that the mapping is going to be limited in the x-dimension
  if(windowAspect < imageAspectRatio){
    xScaledMax = 1;
    yScaledMax = (((double)windowX)*iHeight/iWidth)/(windowY);
  }
  else{
    xScaledMax = (((double)windowY)*iWidth/iHeight)/(windowX);
    yScaledMax = 1;
  }
}

// Meant to be inline functions, but macs didn't like it
float mapY(int imageY)
{
  return -1 * (2 * yScaledMax * imageY / iHeight - yScaledMax);
}

float mapX(int imageX)
{
  return 2 * xScaledMax * imageX / iWidth - xScaledMax;
}

// Main render function
void renderScene(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  LNode* node = LList_head(list);
  float max = 255.0;

  buildMapping(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

  // Paint white backdrop
  glColor3f(0.3, 1.0, 1.0);
  glBegin(GL_QUADS);
  glVertex3f(mapX(0), mapY(0), 0.0);
  glVertex3f(mapX(0), mapY(iHeight), 0.0);
  glVertex3f(mapX(iWidth), mapY(iHeight), 0.0);
  glVertex3f(mapX(iWidth), mapY(0), 0.0);
  glEnd();

  // Iterate through list of triangles and draw them
  while(node != NULL){
    //Color* c = node->oneC;
    //Point* p = node->oneP;
    Triangle* t = node->item;
    Vertex* v = t->v1;
    glBegin(GL_TRIANGLES);

    //glColor3f(c->r, c->g, c->b);
    //glVertex3f(mapX(p->x), mapY(p->y), 0.0);
    glColor3f(v->value/max, v->value/max, v->value/max);
    glVertex3f(mapX(v->col), mapY(v->row), 0.0);

    //c = node->twoC;
    //p = node->twoP;
    v = t->v2;

    //glColor3f(c->r, c->g, c->b);
    //glVertex3f(mapX(p->x), mapY(p->y), 0.0);
    glColor3f(v->value/max, v->value/max, v->value/max);
    glVertex3f(mapX(v->col), mapY(v->row), 0.0);

    //c = node->threeC;
    //p = node->threeP;
    v = t->v3;

    //glColor3f(c->r, c->g, c->b);
    //glVertex3f(mapX(p->x), mapY(p->y), 0.0);
    glColor3f(v->value/max, v->value/max, v->value/max);
    glVertex3f(mapX(v->col), mapY(v->row), 0.0);
    glEnd();
    
    glColor3f(1.0, 0, 0);
    glBegin(GL_LINES);
    glVertex3f(mapX(t->v1->col), mapY(t->v1->row), 0);
    glVertex3f(mapX(t->v2->col), mapY(t->v2->row), 0);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(mapX(t->v1->col), mapY(t->v1->row), 0);
    glVertex3f(mapX(t->v3->col), mapY(t->v3->row), 0);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(mapX(t->v2->col), mapY(t->v2->row), 0);
    glVertex3f(mapX(t->v3->col), mapY(t->v3->row), 0);
    glEnd();

    node = node->next;
  }
  glutSwapBuffers();
}

Vertex* makeVertex(int col, int row, int value)
{
  Vertex* v = malloc(sizeof(Vertex));
  v->row = row;
  v->col = col;
  v->value = value;
  return v;
}


// Cute test image
/* int main(int argc, char** argv) */
/* { */
/*   Triangle* A; */
/*   Triangle* B; */
/*   Triangle* C; */
/*   Triangle* D; */
/*   Triangle* E; */
/*   Triangle* F; */
/*   Triangle* G; */
/*   Triangle* H; */
/*   Triangle* I; */
/*   A = calloc(1, sizeof(Triangle)); */
/*   B = calloc(1, sizeof(Triangle)); */
/*   C = calloc(1, sizeof(Triangle)); */
/*   D = calloc(1, sizeof(Triangle)); */
/*   E = calloc(1, sizeof(Triangle)); */
/*   F = calloc(1, sizeof(Triangle)); */
/*   G = calloc(1, sizeof(Triangle)); */
/*   H = calloc(1, sizeof(Triangle)); */
/*   I = calloc(1, sizeof(Triangle)); */
/*  */
/*   Vertex* one = makeVertex(50, 20, 150); */
/*   Vertex* two = makeVertex(80, 20, 200); */
/*   Vertex* three = makeVertex(120, 20, 50); */
/*   Vertex* four = makeVertex(120, 60, 150); */
/*   Vertex* five = makeVertex(100, 40, 249); */
/*   Vertex* six = makeVertex(90, 100, 90); */
/*   Vertex* seven = makeVertex(65, 60, 0); */
/*   Vertex* eight = makeVertex(50, 80, 15); */
/*   Vertex* nine = makeVertex(35, 60, 80); */
/*  */
/*   A->t1 = B; */
/*   A->v1 = one; */
/*   A->v2 = eight; */
/*   A->v3 = nine; */
/*   A->visited = 0; */
/*  */
/*   B->t1 = A; */
/*   B->t2 = C; */
/*   B->t3 = I; */
/*   B->v1 = one; */
/*   B->v2 = seven; */
/*   B->v3 = eight; */
/*   B->visited = 0; */
/*  */
/*   C->t1 = B; */
/*   C->t2 = D; */
/*   C->v1 = one; */
/*   C->v2 = two; */
/*   C->v3 = seven; */
/*   C->visited = 0; */
/*  */
/*   D->t1 = C; */
/*   D->t2 = E; */
/*   D->t3 = G; */
/*   D->v1 = two; */
/*   D->v2 = five; */
/*   D->v3 = seven; */
/*   D->visited = 0; */
/*  */
/*   E->t1 = D; */
/*   E->t2 = F; */
/*   E->v1 = two; */
/*   E->v2 = three; */
/*   E->v3 = five; */
/*   E->visited = 0; */
/*  */
/*   F->t1 = E; */
/*   F->t2 = G; */
/*   F->v1 = three; */
/*   F->v2 = four; */
/*   F->v3 = five; */
/*   F->visited = 0; */
/*  */
/*   G->t1 = D; */
/*   G->t2 = F; */
/*   G->t3 = H; */
/*   G->v1 = five; */
/*   G->v2 = four; */
/*   G->v3 = seven; */
/*   G->visited = 0; */
/*  */
/*   H->t1 = G; */
/*   H->t2 = I; */
/*   H->v1 = seven; */
/*   H->v2 = four; */
/*   H->v3 = six; */
/*   H->visited = 0; */
/*  */
/*   I->t1 = B; */
/*   I->t2 = H; */
/*   I->v1 = seven; */
/*   I->v2 = six; */
/*   I->v3 = eight; */
/*   I->visited = 0; // Phew! */
/*  */
/*   displayTriangles(A, 140, 120); */

  /* Color* c1 = buildColor(1.0, 1.0, 1.0); */
  /* Color* c2 = buildColor(0, 0, 1.0); */
  /* Color* c3 = buildColor(0, 1.0, 0); */
  /* Color* c4 = buildColor(1.0, 0, 0); */
  /* Color* c5 = buildColor(.5, .5, .5); */
  /* Color* c6 = buildColor(.5, 0, .5); */

  /* Point* p1 = buildPoint(150, 150); */
  /* Point* p2 = buildPoint(10, 150); */
  /* Point* p3 = buildPoint(100, 40); */
  /* Point* p4 = buildPoint(200, 40); */
  /* Point* p5 = buildPoint(200, 150); */
  /* Point* p6 = buildPoint(175, 300); */

  /* TriangleNode* tri = initTriangle(); */
  /* tri->oneP = p1; */
  /* tri->oneC = c1; */
  /* tri->twoP = p2; */
  /* tri->twoC = c2; */
  /* tri->threeP = p6; */
  /* tri->threeC = c6; */
  /* tri->next = initTriangle(); */

  /* TriangleNode* temp = tri->next; */
  /* temp->oneP = p1; */
  /* temp->oneC = c1; */
  /* temp->twoP = p3; */
  /* temp->twoC = c3; */
  /* temp->threeP = p2; */
  /* temp->threeC = c2; */
  /* temp->next = initTriangle(); */

  /* temp = temp->next; */
  /* temp->oneP = p1; */
  /* temp->oneC = c1; */
  /* temp->twoP = p3; */
  /* temp->twoC = c3; */
  /* temp->threeP = p4; */
  /* temp->threeC = c4; */
  /* temp->next = initTriangle(); */

  /* temp = temp->next; */
  /* temp->oneP = p1; */
  /* temp->oneC = c1; */
  /* temp->twoP = p5; */
  /* temp->twoC = c5; */
  /* temp->threeP = p4; */
  /* temp->threeC = c4; */
  /* temp->next = initTriangle(); */

  /* temp = temp->next; */
  /* temp->oneP = p1; */
  /* temp->oneC = c1; */
  /* temp->twoP = p5; */
  /* temp->twoC = c5; */
  /* temp->threeP = p6; */
  /* temp->threeC = c6; */

  /* displayTriangles(tri, 300, 300); */
/*   return 0; */
/* } */
