// manipulator.cpp
// Visualização 3D
// W. Celes - celes@inf.puc-rio.br
// PUC-Rio, Jan 2008

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <Windows.h>    /* includes only in MSWindows not in UNIX */
#include <gl/gl.h>     /* OpenGL functions*/
#include <gl/glu.h>    /* OpenGL utilitary functions*/
#include <gl/glut.h>    /* OpenGL utilitary functions*/
#else
#include <GL/gl.h>     /* OpenGL functions*/
#include <GL/glu.h>    /* OpenGL utilitary functions*/
#include <GL/glut.h>    /* OpenGL utilitary functions*/
#endif

#include "manipulator.h"
#include "vector.h"

#define PI 3.1459

VManipulator* VManipulator::s_instance = 0;

static char s_state = 'r';  // 'r': rotating, or 'z': zomming
static int s_x0, s_y0;

static VVector map (int x, int y)
{
  VVector v;
  int vp[4];
  glGetIntegerv(GL_VIEWPORT,vp);
  int radius = vp[2] < vp[3] ? vp[2]/2 : vp[3]/2;
  v.x = (float)((x-vp[0]) - vp[2]/2) / radius;
  v.y = (float)((vp[3]-y-vp[0]) - vp[3]/2) / radius;
  float len = sqrt(v.x*v.x + v.y*v.y);
  if (len > 1.0) {
    v.x /= len;
    v.y /= len;
    v.z = 0.0f;
  }
  else {
    v.z = sqrt(1 - v.x*v.x - v.y*v.y);
  }
  return v;
}

static void mouse (int button, int state, int x, int y)
{
  if (state == GLUT_DOWN) {
    s_x0 = x;
    s_y0 = y;
    if (button == GLUT_LEFT_BUTTON)
      s_state = 'r';
    else if (button == GLUT_RIGHT_BUTTON)
      s_state = 'z';
    else
      s_state = ' ';
  }
}

static void motion (int x, int y)
{
  int dx = abs(x-s_x0);
  int dy = abs(y-s_y0);
  if ( dx > 1 || dy > 1) {
    if (s_state == 'r') {
      VVector v0 = map(s_x0, s_y0);
      VVector v1 = map(x, y);
      VVector r = Cross(v0, v1);
      VManipulator::Instance()->Rotate(2*asin(r.Length())*180.0f/PI, r.x, r.y, r.z);

    }
    else if (s_state == 'z') {
      int vp[4];
      glGetIntegerv(GL_VIEWPORT,vp);
      float f = dx > dy ? (float)(x-s_x0) / vp[2] : (float) (-y+s_y0) / vp[3];
      VManipulator::Instance()->Scale(1+f, 1+f, 1+f);
    }
    s_x0 = x;
    s_y0 = y;
    glutPostRedisplay();
  }
}

VManipulator* VManipulator::Instance ()
{
  if (s_instance==0)
    s_instance = new VManipulator();
  return s_instance;
}

VManipulator::VManipulator ()
{
  Identity();
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
}

void VManipulator::SetZCenter (float zcenter)
{
  m_zcenter = zcenter;
}

void VManipulator::Identity ()
{
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glGetFloatv(GL_MODELVIEW_MATRIX, m_matrix);
  glPopMatrix();
  glPopAttrib();
}

void VManipulator::Rotate (float angle, float rx, float ry, float rz)
{
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glRotatef(angle, rx, ry, rz);
  glMultMatrixf(m_matrix);
  glGetFloatv(GL_MODELVIEW_MATRIX, m_matrix);
  glPopMatrix();
  glPopAttrib();
}

void VManipulator::Scale (float sx, float sy, float sz)
{
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glScalef(sx, sy, sz);
  glMultMatrixf(m_matrix);
  glGetFloatv(GL_MODELVIEW_MATRIX, m_matrix);
  glPopMatrix();
  glPopAttrib();
}

void VManipulator::Load ()
{
  glTranslatef(0.0f, 0.0f, -m_zcenter);
  glMultMatrixf(m_matrix);
  glTranslatef(0.0f, 0.0f, m_zcenter);
}

