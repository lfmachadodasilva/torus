/*
 * PUC-Rio - Pontificia Universidade Catolica
 *
 * Professor: Marcelo Gattass
 *
 * Arquivo: mesh.cpp
 *
 * Nome: Luiz Felipe Machado
 */

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

#include "mesh.h"

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
bool cMesh::create_mesh(std::vector<VVector> vet, int dx, int dy)
{
  // (TODO) verificar se os pontos são colineares


  // (TODO) verificar se os pontos pertencem a um plano.

  // Criando vertices
  VVector p0, p1, p2;
  int num_pts = (int) vet.size();
  p0 = vet[num_pts - 1];

  p1 = vet[0];

  p2 = vet[1];

  VVector p10 = p0 - p1;
  double sizex = p10.Length() / dx;
  p10.Normalize();

  VVector p12 = p2 - p1;
  double sizey = p12.Length() / dy;
  p12.Normalize();

  std::vector<VVector> vet_aux;

  for(int x = 0; x <= dx; ++x)
  {
    for(int y = 0; y <= dy; ++y)
    {
      VVector aux_x = (p10 * (sizex * x));
      VVector aux_y = (p12 * (sizey * y));
      vet_aux.push_back(aux_x + aux_y + p1);
    }
  }

  for(int x = 0, i = 0; x < dx; ++x)
  {
    for(int y = 0; y < dy; ++y)
    {
      add_triangle( vet_aux[i], vet_aux[i + 1], vet_aux[i + 1 + dy], 
                    m_data_tri, m_data_ver, m_data_nor);
      add_triangle( vet_aux[i + 1], vet_aux[i + 2 + dy], vet_aux[i + 1 + dy],
                    m_data_tri, m_data_ver, m_data_nor);
      ++i;
    }
    ++i;
  }

  calculate_bbox_mesh();

  return true;
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
void cMesh::draw(void)
{
  for(int i = 0; i < (int)m_data_tri.size(); ++i)
  {
    cTriangle tri = m_data_tri[i];
    VVector p0 = m_data_ver[tri.m_v[0]];
    VVector p1 = m_data_ver[tri.m_v[1]];
    VVector p2 = m_data_ver[tri.m_v[2]];

    glBegin(GL_TRIANGLES);
    {
      glNormal3f(m_data_nor[i].x, m_data_nor[i].y, m_data_nor[i].z);

      glVertex3f(p0.x, p0.y, p0.z);
      glVertex3f(p1.x, p1.y, p1.z);
      glVertex3f(p2.x, p2.y, p2.z);
    }
    glEnd();
  }

  //glPointSize(5.0f);
  //for(int i = 0; i < (int)m_data_ver.size(); ++i)
  //{
  //  glBegin(GL_POINTS);
  //  {
  //    glVertex3f(m_data_ver[i].x, m_data_ver[i].y, m_data_ver[i].z);
  //  }
  //  glEnd();
  //}
}
