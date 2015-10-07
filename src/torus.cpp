/*
 * PUC-Rio - Pontificia Universidade Catolica
 *
 * Professor: Marcelo Gattass
 *
 * Arquivo: torus.cpp
 *
 * Nome: Luiz Felipe Machado
 */

#include <math.h>
#include <algorithm>
using namespace std;

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

#include "torus.h"

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// https://kit3d.svn.codeplex.com/svn/CSharp/Kit3D/Kit3D/Objects/Torus.cs
void cTorus::m_create_mesh(void)
{
  //Loop rounds the circumference creating all of the individual segments of the torus
  for (int i = 0; i < m_slides; ++i)
  {
    //Calculate the starting angle of the torus segment
    double torus_angle = PI * 2 * ((double)i / m_slides);

    //Create a vector that is pointing in the current direction of the
    //segment we want to create, this is a unit vector
    VVector currentSegmentDirection(cos(torus_angle), sin(torus_angle), 0.0f);

    //For each segment we want to create a number of bands going around the segment
    for (int j = 0; j < m_rings; ++j)
    {
      double band_angle = PI * 2 * ((double)j / m_rings);

      VVector currentBandDirection = currentSegmentDirection * cos(band_angle);
      currentBandDirection.z += sin(band_angle);

      VVector p0 = currentSegmentDirection * m_inner_radius;

      VVector p1 = currentBandDirection * m_outer_radius;

      VVector p;
      p = p0 + p1;

      m_vet.push_back(p);

      int size = (int)m_vet.size();

      int a = size - 1;
      int b = size - 2;
      int c = size - 2 - m_rings;
      int d = size - 1 - m_rings;

      if(j == 0)
      {
        b += m_rings;
        c += m_rings;
      }

      if(i == 0)
      {
        c += m_rings * m_slides;
        d += m_rings * m_slides;
      }

      m_tri.push_back(cTriangle(a, c, b));
      m_tri.push_back(cTriangle(a, d, c));

    }
  }

  // Calcular normal
  for(int i = 0; i < (int)m_tri.size(); ++i)
  {
    VVector AB = m_vet[m_tri[i].m_v[1]] - m_vet[m_tri[i].m_v[0]];
    VVector AC = m_vet[m_tri[i].m_v[2]] - m_vet[m_tri[i].m_v[0]];

    VVector nor = Cross(AB, AC);

    nor.Normalize();

    m_nor.push_back(nor);
  }
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
void cTorus::draw(void)
{
  glBegin(GL_TRIANGLES);
  for(int i = 0; i < (int)m_tri.size(); i++)
  {
    glNormal3f(m_nor[i].x, m_nor[i].y, m_nor[i].z);

    glVertex3d(m_vet[m_tri[i].m_v[0]].x,
               m_vet[m_tri[i].m_v[0]].y,
               m_vet[m_tri[i].m_v[0]].z);

    glNormal3f(m_nor[i].x, m_nor[i].y, m_nor[i].z);

    glVertex3d(m_vet[m_tri[i].m_v[1]].x,
               m_vet[m_tri[i].m_v[1]].y,
               m_vet[m_tri[i].m_v[1]].z);

    glNormal3f(m_nor[i].x, m_nor[i].y, m_nor[i].z);

    glVertex3d(m_vet[m_tri[i].m_v[2]].x,
               m_vet[m_tri[i].m_v[2]].y,
               m_vet[m_tri[i].m_v[2]].z);
  }
  glEnd();
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
void cTorus::calculate_bbox(void)
{
  m_bbox.max = m_vet[0];
  m_bbox.min = m_vet[0];

  for(int i = 1; i < (int)m_vet.size(); ++i)
  {
    m_bbox.max.x = max(m_bbox.max.x, m_vet[i].x);
    m_bbox.max.y = max(m_bbox.max.y, m_vet[i].y);
    m_bbox.max.z = max(m_bbox.max.z, m_vet[i].z);

    m_bbox.min.x = min(m_bbox.min.x, m_vet[i].x);
    m_bbox.min.y = min(m_bbox.min.y, m_vet[i].y);
    m_bbox.min.z = min(m_bbox.min.z, m_vet[i].z);
  }
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
