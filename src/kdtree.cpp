/*
 * PUC-Rio - Pontificia Universidade Catolica
 *
 * Professor: Marcelo Gattass
 *
 * Arquivo: kdtree.cpp
 *
 * Nome: Luiz Felipe Machado
 */

#include <algorithm>

#ifdef WIN32
#include <windows.h>    /* includes only in MSWindows not in UNIX */
#include <gl/gl.h>     /* OpenGL functions*/
#include <gl/glu.h>    /* OpenGL utilitary functions*/
#include <gl/glut.h>    /* OpenGL utilitary functions*/
#else
#include <GL/gl.h>     /* OpenGL functions*/
#include <GL/glu.h>    /* OpenGL utilitary functions*/
#include <GL/glut.h>    /* OpenGL utilitary functions*/
#endif

#include "kdtree.h"
#include "bbox.h"

/**
  * @brief Seleciona o plano que vai cortar a cena
  *
  * @param[in] vet_tri     - vetor com a lista de triangulos
  * @param[in] vet_vet     - vetor com a lista de vertices
  * @param[in] vet_nor     - vetor com a lista de normais
  * @param[out] axis       - eixo selecionado (X, Y ou Z)
  * @param[out] axis_value - valor do eixo que vai compor o plano
  */
void selected_plane_kdtree(std::vector<cTriangle> &vet_tri,
                           std::vector<VVector> &vet_vertex,
                           std::vector<VVector> &vet_nor,
                           int &axis,
                           int &axis_value,
                           int depth);

void get_att(const int axis, const int axis_value, VVector &normal_plane, VVector &p);

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
void cKdtree::create_kdtree(std::vector<cTriangle> &vet_tri,
                            std::vector<VVector> &vet_vertex,
                            std::vector<VVector> &vet_nor)
{
  if(!m_root)
    m_root = m_insert_rec(vet_tri, vet_vertex, vet_nor, 3);

  m_bbox = calculate_bbox(vet_vertex);
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
cNodeKdtree* cKdtree::m_insert_rec(std::vector<cTriangle> &vet_tri,
                                   std::vector<VVector> &vet_vertex,
                                   std::vector<VVector> &vet_nor,
                                   int depth)
{
  cNodeKdtree *node = NULL;

  if(vet_tri.empty())
  {
    // ja eh uma folha
    return node;
  }

  if((int)vet_tri.size() == 1)
  {
    // vai ser a folha
    // insere no noh o indice do triangulo
    node = new cNodeKdtree();

    add_triangle(vet_vertex[vet_tri[0].m_v[0]],
                 vet_vertex[vet_tri[0].m_v[1]],
                 vet_vertex[vet_tri[0].m_v[2]],
                 m_data_tri,
                 m_data_vet,
                 m_data_nor);
    node->is_leaf = true;
    node->tri = (int) m_data_tri.size() - 1;

    return node;
  }

  std::vector<cTriangle> vet_tri_front;
  std::vector<VVector> vet_vertex_front, vet_nor_front;

  std::vector<cTriangle> vet_tri_back;
  std::vector<VVector> vet_vertex_back, vet_nor_back;

  // escolher o eixo e o valor do eixo
  int axis, axis_value;
  selected_plane_kdtree(vet_tri, vet_vertex, vet_nor, axis, axis_value, depth);

  printf("Axis: %d\tvalue: %d\n", axis, axis_value);

  VVector normal_plane, point;
  get_att(axis, axis_value, normal_plane, point);

  // cria o noh
  node = new cNodeKdtree();
  node->is_leaf = false;
  node->axis = axis;
  node->axis_value = axis_value;

  vet_tri_front.clear(); vet_vertex_front.clear(); vet_nor_front.clear();
  vet_tri_back.clear(); vet_vertex_back.clear(); vet_nor_back.clear();

  // separar os triangulos
  for(int i = 0; i < (int)vet_tri.size(); i++)
  {
    int side = calculate_side(normal_plane,
                              point,
                              vet_vertex[vet_tri[i].m_v[0]],
                              vet_vertex[vet_tri[i].m_v[1]],
                              vet_vertex[vet_tri[i].m_v[2]]);
    if(side == CUT)
    {
      cut_triangle(normal_plane,
                   point,
                   i,
                   vet_tri,
                   vet_vertex,
                   vet_nor,
                   vet_tri_front,
                   vet_vertex_front,
                   vet_nor_front,
                   vet_tri_back,
                   vet_vertex_back,
                   vet_nor_back);
    }
    else if(side == FRONT)
    {
      add_triangle(vet_vertex[vet_tri[i].m_v[0]],
                   vet_vertex[vet_tri[i].m_v[1]],
                   vet_vertex[vet_tri[i].m_v[2]],
                   vet_tri_front,
                   vet_vertex_front,
                   vet_nor_front);
    }
    else// if(side == BACK)
    {
      add_triangle(vet_vertex[vet_tri[i].m_v[0]],
                   vet_vertex[vet_tri[i].m_v[1]],
                   vet_vertex[vet_tri[i].m_v[2]],
                   vet_tri_back,
                   vet_vertex_back,
                   vet_nor_back);
    }

  }

  //vet_tri.clear(); vet_vertex.clear(); vet_nor.clear();

  node->front = m_insert_rec(vet_tri_front, vet_vertex_front, vet_nor_front, depth + 1);
  node->back = m_insert_rec(vet_tri_back, vet_vertex_back, vet_nor_back, depth + 1);

  //vet_tri_front.clear(); vet_vertex_front.clear(); vet_nor_front.clear();
  //vet_tri_back.clear(); vet_vertex_back.clear(); vet_nor_back.clear();

  return node;
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
void cKdtree::draw(VVector &eye)
{
  if(m_root)
    m_draw_rec(m_root, eye);
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
void cKdtree::m_draw_node(cNodeKdtree *node)
{
  if(!node->is_leaf) // desenha apenas se for folha
    return;

  int i = node->tri;

  glBegin(GL_TRIANGLES);
  {
    glNormal3f(m_data_nor[i].x, m_data_nor[i].y, m_data_nor[i].z);

    glVertex3d(m_data_vet[m_data_tri[i].m_v[0]].x,
               m_data_vet[m_data_tri[i].m_v[0]].y,
               m_data_vet[m_data_tri[i].m_v[0]].z);

    glNormal3f(m_data_nor[i].x, m_data_nor[i].y, m_data_nor[i].z);

    glVertex3d(m_data_vet[m_data_tri[i].m_v[1]].x,
               m_data_vet[m_data_tri[i].m_v[1]].y,
               m_data_vet[m_data_tri[i].m_v[1]].z);

    glNormal3f(m_data_nor[i].x, m_data_nor[i].y, m_data_nor[i].z);

    glVertex3d(m_data_vet[m_data_tri[i].m_v[2]].x,
               m_data_vet[m_data_tri[i].m_v[2]].y,
               m_data_vet[m_data_tri[i].m_v[2]].z);
  }
  glEnd();
}


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
void cKdtree::m_draw_rec(cNodeKdtree *node, VVector &eye)
{
  if(!node)
    return;

  if(!node->back && !node->front)
  {
    // eh uma folha
    m_draw_node(node);
    return;
  }

  VVector nor, p;
  get_att(node->axis, node->axis_value, nor, p);

  //int t = node->tri;
  int s = calculate_side(nor, p, eye);

  if(s == FRONT)
  {
    // estou na frente do no
    m_draw_rec(node->back, eye);
    //m_draw_node(node);
    m_draw_rec(node->front, eye);
  }
  else if(s == BACK)
  {
    //estou atras do no
    m_draw_rec(node->front, eye);
    //m_draw_node(node);
    m_draw_rec(node->back, eye);
  }
  else
  {
    m_draw_rec(node->front, eye);
    m_draw_rec(node->back, eye);
  }
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
int calculate_side_kdtree(VVector &normal_plane,  VVector &p,  VVector &a,  VVector &b,  VVector &c)
{
  double d0 = Dot(a - p, normal_plane);
  double d1 = Dot(b - p, normal_plane);
  double d2 = Dot(c - p, normal_plane);

  // tolerancia
  if(d0 > -TOL && d0 < TOL) d0 = 0.0f;
  if(d1 > -TOL && d1 < TOL) d1 = 0.0f;
  if(d2 > -TOL && d2 < TOL) d2 = 0.0f;

  if((d0 * d1 < -TOL) || (d0 * d2 < -TOL) || (d1 * d2 < -TOL))
    return CUT;

  if(d0 > TOL || d1 > TOL || d2 > TOL)
    return FRONT;
  else if(d0 < TOL || d1 < TOL || d2 < TOL)
    return BACK;

  // paralelo
  return BACK;
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
int calculate_side_kdtree(VVector &normal_plane,  VVector &p,  VVector &a)
{
  double d = Dot(a - p, normal_plane);

  // tolerancia
  if(d > -TOL && d < TOL) d = 0.0f;

  if(d > TOL)
    return FRONT;
  else if(d < TOL)
    return BACK;

  return CUT;
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
void get_att(const int axis, const int axis_value, VVector &normal_plane, VVector &p)
{
  if(axis == X)
  {
    normal_plane.x = 1.0f;
    normal_plane.y = 0.0f;
    normal_plane.z = 0.0f;

    p.x = axis_value;
    p.y = 0.0f;
    p.z = 0.0f;
  }
  else if(axis == Y)
  {
    normal_plane.x = 0.0f;
    normal_plane.y = 1.0f;
    normal_plane.z = 0.0f;

    p.x = 0.0f;
    p.y = axis_value;
    p.z = 0.0f;
  }
  else if(axis == Z)
  {
    normal_plane.x = 0.0f;
    normal_plane.y = 0.0f;
    normal_plane.z = 1.0f;

    p.x = 0.0f;
    p.y = 0.0f;
    p.z = axis_value;
  }
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
void selected_plane_kdtree(std::vector<cTriangle> &,
                           std::vector<VVector> &vet_vertex,
                           std::vector<VVector> &,
                           int &axis,
                           int &axis_value,
                           int depth)
{
  //// calcula a bouding box
  //cBbox b = calculate_bbox(vet_vertex);

  //// verifica qual eixo eh maior
  //double delta_x = fabs(b.max.x - b.min.x);
  //double delta_y = fabs(b.max.y - b.min.y);
  //double delta_z = fabs(b.max.z - b.min.z);
  //if(delta_x > delta_y)
  //{
  //  if(delta_x > delta_z)
  //  {
  //    axis = X;
  //    axis_value = (b.max.x + b.min.x) / 2;
  //  }
  //  else
  //  {
  //    axis = Z;
  //    axis_value = (b.max.z + b.min.z) / 2;
  //  }
  //}
  //else
  //{
  //  if(delta_y > delta_z)
  //  {
  //    axis = Y;
  //    axis_value = (b.max.y + b.min.y) / 2;
  //  }
  //  else
  //  {
  //    axis = Z;
  //    axis_value = (b.max.z + b.min.z) / 2;
  //  }
  //}

  //double dx, dy, dz;
  //if(delta_x > 0) dx = (b.max.x + b.min.x) / 2;
  //else dx = 0.0f;

  //if(delta_y > 0) dy = (b.max.y + b.min.y) / 2;
  //else dy = 0.0f;

  //if(delta_z > 0) dz = (b.max.z + b.min.z) / 2;
  //else dz = 0.0f;

  //VVector v (dx, dy, dz);
  //int idx = 0;
  //double l = VVector(vet_vertex[0] - v).Length();

  //for(int i = 1; i < (int) vet_vertex.size(); ++i)
  //{
  //  VVector b = vet_vertex[i] - v;
  //  double l_aux = b.Length();
  //  if(l_aux < l)
  //  {
  //    l = l_aux;
  //    idx = i;
  //  }
  //}
  //VVector m = vet_vertex[idx];

  axis = depth % 3;

  vector<VVector> vet_aux = vet_vertex;

  int n = (int)vet_aux.size();
  bool change = false;
  for(int j = n - 1; j > 0; j--)
  {
    for(int i = 0; i < j; i++)
    {
      if(axis == 0 && vet_aux[i + 1].x < vet_aux[i].x)
        change = true;
      if(axis == 1 && vet_aux[i + 1].y < vet_aux[i].y)
        change = true;
      if(axis == 2 && vet_aux[i + 1].z < vet_aux[i].z)
        change = true;

      if(change)
      {
        VVector a = vet_aux[i];
        vet_aux[i] = vet_aux[i + 1];
        vet_aux[i + 1] = a;
        change = false;
      }
    }
  }

  VVector m = vet_aux[n / 2];

  if(axis == X)
  {
    axis_value = m.x;
  }
  else if(axis == Y)
  {
    axis_value = m.y;
  }
  else if(axis == Z)
  {
    axis_value = m.z;
  }
}
