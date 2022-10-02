#include <stdio.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "vector.h"

#define GL_SCREEN_W       1280
#define GL_SCREEN_H       768
#define ORIGY             -1

const float GL_LIGHT_AMBIENT[] =  {0.2f, 0.2f, 0.2f, 1};
const float GL_LIGHT_COLOR[] = {1, 1, 1, 1};
const float GL_LIGHT_POS[] = {2, 4, 3, 0};
const float PLATFORM_COLOR[] = {1, 1, 1};

typedef struct
{
  int w, a, s, d;
} Keyboard; Keyboard keys;

typedef struct
{
  V3 pos, angle, dir, delta, origin;
  float speed, rotation;
} Camera; Camera camera;

void MoveCamera()
{
  if (keys.w == 1) { camera.pos.x += camera.dir.x * camera.speed; camera.pos.z += camera.dir.z * camera.speed; }
  if (keys.s == 1) { camera.pos.x -= camera.dir.x * camera.speed; camera.pos.z -= camera.dir.z * camera.speed; }
  if (keys.d == 1) { camera.pos.x -= camera.dir.z * camera.speed; camera.pos.z += camera.dir.x * camera.speed; }
  if (keys.a == 1) { camera.pos.x += camera.dir.z * camera.speed; camera.pos.z -= camera.dir.x * camera.speed; }
}

void DrawCube(V3 pos, V3 size)
{
  glPushMatrix();

  glTranslatef(pos.x, pos.y, pos.z);

  size = (V3){size.x * 0.5f, size.y * 0.5f, size.z * 0.5f};

  glBegin(GL_QUADS);

  // Front
  glNormal3f(-1,  0,  1); glVertex3f(-size.x, -size.y, -size.z);
  glNormal3f( 1,  0,  1); glVertex3f( size.x, -size.y, -size.z);
  glNormal3f( 1,  0,  1); glVertex3f( size.x,  size.y, -size.z);
  glNormal3f(-1,  0,  1); glVertex3f(-size.x,  size.y, -size.z);

  // Back
  glNormal3f(-1,  0, -1); glVertex3f(-size.x,  size.y,  size.z);
  glNormal3f( 1,  0, -1); glVertex3f( size.x,  size.y,  size.z);
  glNormal3f( 1,  0, -1); glVertex3f( size.x, -size.y,  size.z);
  glNormal3f(-1,  0, -1); glVertex3f(-size.x, -size.y,  size.z);

  // Right
  glNormal3f( 1,  0, -1); glVertex3f( size.x, -size.y, -size.z);
  glNormal3f( 1,  0,  1); glVertex3f( size.x, -size.y,  size.z);
  glNormal3f( 1,  0,  1); glVertex3f( size.x,  size.y,  size.z);
  glNormal3f( 1,  0, -1); glVertex3f( size.x,  size.y, -size.z);

  // Left
  glNormal3f(-1,  0, -1); glVertex3f(-size.x,  size.y, -size.z);
  glNormal3f(-1,  0,  1); glVertex3f(-size.x,  size.y,  size.z);
  glNormal3f(-1,  0,  1); glVertex3f(-size.x, -size.y,  size.z);
  glNormal3f(-1,  0, -1); glVertex3f(-size.x, -size.y, -size.z);

  // Top
  glNormal3f(-1,  1,  1); glVertex3f(-size.x,  size.y, -size.z);
  glNormal3f( 1,  1,  1); glVertex3f( size.x,  size.y, -size.z);
  glNormal3f( 1,  1, -1); glVertex3f( size.x,  size.y,  size.z);
  glNormal3f(-1,  1, -1); glVertex3f(-size.x,  size.y,  size.z);

  // Bottom
  glNormal3f(-1, -1, -1); glVertex3f(-size.x, -size.y,  size.z);
  glNormal3f( 1, -1, -1); glVertex3f( size.x, -size.y,  size.z);
  glNormal3f( 1, -1,  1); glVertex3f( size.x, -size.y, -size.z);
  glNormal3f(-1, -1,  1); glVertex3f(-size.x, -size.y, -size.z);
  
  glEnd();

  glPopMatrix();
}

void DrawScene()
{
  MoveCamera();
  
  gluLookAt(camera.pos.x, 1, camera.pos.z, camera.pos.x + camera.dir.x, 1,  camera.pos.z + camera.dir.z, 0, 1,  0);
  glTranslatef(0, -camera.pos.y, 0);

  glColor3f(1, 1, 1);
  DrawCube((V3){0, -1, 0}, (V3){10, 1, 8});
  glColor3f(0.9f, 0.9f, 0.3f);
  DrawCube((V3){-3, 0, -2}, (V3){1, 8, 1});

  glPushMatrix();
  glColor3f(1, 0.3f, 0.4f);
  glTranslatef(2, 1, -2);
  glutSolidTeapot(1);
  glPopMatrix();

  glColor3f(0, 0, 0);
  glRasterPos3f(2, 2, -2);
  glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char *)"~ GLUT_SolidTeapot");
}

void InitFunc()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  glFrontFace(GL_CW);

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_FRAMEBUFFER_SRGB);
  
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, GL_LIGHT_AMBIENT);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, GL_LIGHT_COLOR);
  glLightfv(GL_LIGHT0, GL_POSITION, GL_LIGHT_POS);
 
  glShadeModel(GL_SMOOTH);
  // glShadeModel(GL_FLAT);

  glClearColor(0, 0, 0, 1);
  
  glPointSize(8);

  gluPerspective(35, 1, 0.1f, 1000);
  glMatrixMode(GL_MODELVIEW);

  camera.angle.x = 30;
  camera.speed = 0.1f;
  camera.rotation = 0.03f;
  camera.dir.x =  sin(camera.angle.y + camera.delta.y);
  camera.dir.z = -cos(camera.angle.y + camera.delta.y);
}

void DisplayFunc()
{
  time.tick++; if (time.tick >= 20) { time.tick = 0; }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  DrawScene();

  glutSwapBuffers();
  glutPostRedisplay();
}

void KeyboardFunc(unsigned char key, int _, int __)
{
  if (key == 'w') keys.w = 1;
  if (key == 's') keys.s = 1;
  if (key == 'd') keys.d = 1;
  if (key == 'a') keys.a = 1;
  if (key == 27 ) exit(0);
}

void KeyboardUpFunc(unsigned char key, int _, int __)
{
  if (key == 'w') keys.w = 0;
  if (key == 's') keys.s = 0;
  if (key == 'd') keys.d = 0;
  if (key == 'a') keys.a = 0;
}

void MotionFunc(int x, int y)
{
  camera.dir.x =  sin(camera.angle.y + camera.delta.y);
  camera.dir.z = -cos(camera.angle.y + camera.delta.y);
  
  camera.delta.y = (x - camera.origin.x) * 0.001f;
}

void MouseFunc(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
  {
    camera.angle.y += camera.delta.y;
    camera.origin.x = -1;
  }
  else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    camera.origin.x = x;
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(GL_SCREEN_W, GL_SCREEN_H);
  glutInitWindowPosition((1920 - GL_SCREEN_W) * 0.5f, (1080 - GL_SCREEN_H) * 0.5f);
  
  glutCreateWindow("Nickel Engine ~ AT_v0.1");
  
  InitFunc();

  glutIgnoreKeyRepeat(1);

  glutDisplayFunc(DisplayFunc);
  glutKeyboardFunc(KeyboardFunc);
  glutKeyboardUpFunc(KeyboardUpFunc);
  glutMouseFunc(MouseFunc);
	glutMotionFunc(MotionFunc);
  
  glutMainLoop();
  
  return 0;
}
