#include <stdlib.h>
#include <stdio.h>

#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#include "Display.h"

TriangleNode* list;
double imageAspectRatio;
// Image height & width
int iHeight, iWidth;

// Maximum value that an x/y value can be mapped to, given
// current aspect ratio of window
double yScaledMax, xScaledMax;

void renderScene(void);

void displayTriangles(TriangleNode* head, int maxX, int maxY)
{
  list = head;
  imageAspectRatio = ((double)maxX)/maxY;
  iHeight = maxY;
  iWidth = maxX;

  int dummy_argc = 1;
  char** dummy_argv = malloc(2 * sizeof(char*));
  dummy_argv[0] = "Simplified Image";
  dummy_argv[1] = NULL;

  glutInit(&dummy_argc, dummy_argv);
  glutInitWindowPosition(-1, -1);
  glutInitWindowSize(500, 500);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutCreateWindow("Window!");

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

  TriangleNode* node = list;

  buildMapping(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

  // Paint white backdrop
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_QUADS);
  glVertex3f(mapX(0), mapY(0), 0.0);
  glVertex3f(mapX(0), mapY(iHeight), 0.0);
  glVertex3f(mapX(iWidth), mapY(iHeight), 0.0);
  glVertex3f(mapX(iWidth), mapY(0), 0.0);
  glEnd();

  // Iterate through list of triangles and draw them
  while(node != NULL){
    Color* c = node->oneC;
    Point* p = node->oneP;
    glBegin(GL_TRIANGLES);

    glColor3f(c->r, c->g, c->b);
    glVertex3f(mapX(p->x), mapY(p->y), 0.0);

    c = node->twoC;
    p = node->twoP;

    glColor3f(c->r, c->g, c->b);
    glVertex3f(mapX(p->x), mapY(p->y), 0.0);

    c = node->threeC;
    p = node->threeP;

    glColor3f(c->r, c->g, c->b);
    glVertex3f(mapX(p->x), mapY(p->y), 0.0);

    glEnd();

    node = node->next;
  }
  glutSwapBuffers();
}


// Cute test image
int main(int argc, char** argv)
{
  Color* c1 = buildColor(1.0, 1.0, 1.0);
  Color* c2 = buildColor(0, 0, 1.0);
  Color* c3 = buildColor(0, 1.0, 0);
  Color* c4 = buildColor(1.0, 0, 0);
  Color* c5 = buildColor(.5, .5, .5);
  Color* c6 = buildColor(.5, 0, .5);

  Point* p1 = buildPoint(150, 150);
  Point* p2 = buildPoint(10, 150);
  Point* p3 = buildPoint(100, 40);
  Point* p4 = buildPoint(200, 40);
  Point* p5 = buildPoint(200, 150);
  Point* p6 = buildPoint(175, 300);

  TriangleNode* tri = initTriangle();
  tri->oneP = p1;
  tri->oneC = c1;
  tri->twoP = p2;
  tri->twoC = c2;
  tri->threeP = p6;
  tri->threeC = c6;
  tri->next = initTriangle();

  TriangleNode* temp = tri->next;
  temp->oneP = p1;
  temp->oneC = c1;
  temp->twoP = p3;
  temp->twoC = c3;
  temp->threeP = p2;
  temp->threeC = c2;
  temp->next = initTriangle();

  temp = temp->next;
  temp->oneP = p1;
  temp->oneC = c1;
  temp->twoP = p3;
  temp->twoC = c3;
  temp->threeP = p4;
  temp->threeC = c4;
  temp->next = initTriangle();

  temp = temp->next;
  temp->oneP = p1;
  temp->oneC = c1;
  temp->twoP = p5;
  temp->twoC = c5;
  temp->threeP = p4;
  temp->threeC = c4;
  temp->next = initTriangle();

  temp = temp->next;
  temp->oneP = p1;
  temp->oneC = c1;
  temp->twoP = p5;
  temp->twoC = c5;
  temp->threeP = p6;
  temp->threeC = c6;

  displayTriangles(tri, 300, 300);
  return 0;
}
