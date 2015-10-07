/*
 * PUC-Rio - Pontificia Universidade Catolica
 *
 * Professor: Marcelo Gattass
 *
 * Arquivo: bbox.h
 *
 * Nome: Luiz Felipe Machado
 */

#ifndef BBOX_H
#define BBOX_H

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

#include "vector.h"

class cBbox
{
private:
public:
  VVector max;
  VVector min;

  void operator = (const cBbox &b)
  {
    this->max = b.max;
    this->min = b.min;
  }

  void draw(void)
  {
    glBegin(GL_LINES);
    {
      glVertex3f(min.x, min.y, min.z);
      glVertex3f(min.x, max.y, min.z);

      glVertex3f(min.x, min.y, min.z);
      glVertex3f(max.x, min.y, min.z);

      glVertex3f(min.x, min.y, min.z);
      glVertex3f(min.x, min.y, max.z);

      glVertex3f(min.x, min.y, max.z);
      glVertex3f(max.x, min.y, max.z);

      glVertex3f(min.x, min.y, max.z);
      glVertex3f(min.x, max.y, max.z);

      glVertex3f(max.x, max.y, max.z);
      glVertex3f(max.x, min.y, max.z);

      glVertex3f(max.x, max.y, max.z);
      glVertex3f(min.x, max.y, max.z);

      glVertex3f(max.x, max.y, max.z);
      glVertex3f(max.x, max.y, min.z);

      glVertex3f(max.x, max.y, min.z);
      glVertex3f(min.x, max.y, min.z);

      glVertex3f(max.x, max.y, min.z);
      glVertex3f(max.x, min.y, min.z);

      glVertex3f(max.x, min.y, max.z);
      glVertex3f(max.x, min.y, min.z);

      glVertex3f(min.x, max.y, max.z);
      glVertex3f(min.x, max.y, min.z);

    }
    glEnd();
  }
};

#endif // BBOX_H
