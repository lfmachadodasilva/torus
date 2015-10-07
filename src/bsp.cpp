/*
 * PUC-Rio - Pontificia Universidade Catolica
 *
 * Professor: Marcelo Gattass
 *
 * Arquivo: bsp.cpp
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

#include "bsp.h"

/**
  * @brief Seleciona um triangulo
  *
  * Seleciona um triangulo que possa ser utilizado como no da arvore
  *
  * @param[in] vet_tri - vetor com a lista de triangulos
  * @param[in] vet_vet - vetor com a lista de vertices
  * @param[in] vet_nor - vetor com a lista de normais
  * @return int - indice do triangulo a ser utilizado como no
  */
int select_triangle(std::vector<cTriangle> &vet_tri, std::vector<VVector> &vet_vertex, std::vector<VVector> &vet_nor);

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
void cBsp::create_bsp(std::vector<cTriangle> &vet_tri,
                      std::vector<VVector> &vet_vertex,
                      std::vector<VVector> &vet_nor)
{
  if(!m_root)
    m_root = m_insert_rec(vet_tri, vet_vertex, vet_nor);

  calculate_bbox_bsp();
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
cNodeBsp* cBsp::m_insert_rec(std::vector<cTriangle> &vet_tri,
                          std::vector<VVector> &vet_vertex,
                          std::vector<VVector> &vet_nor)
{
  cNodeBsp *n = NULL;

  if(vet_tri.empty())
  {
    // o no pai eh uma folha
    return n;
  }

  std::vector<cTriangle> vet_tri_front;
  std::vector<VVector> vet_vertex_front, vet_nor_front;

  std::vector<cTriangle> vet_tri_back;
  std::vector<VVector> vet_vertex_back, vet_nor_back;

  // escolher o triangulo que vai dividir a cena
  int t = select_triangle(vet_tri, vet_vertex, vet_nor);

  // separar os triangulos da direta, esquerda e os cortados
  for(int i = 0; i < (int)vet_tri.size(); i++)
  {
    if(t == i)
    {
      // nao precisa separar o triangulo escolhido

      // cria no
      n = new cNodeBsp();

      // adiciona aos vetores "globais" / "finais" o novo triangulo

      add_triangle(vet_vertex[vet_tri[t].m_v[0]],
                   vet_vertex[vet_tri[t].m_v[1]],
                   vet_vertex[vet_tri[t].m_v[2]],
                   m_data_tri,
                   m_data_vet,
                   m_data_nor);

      n->tri = (int)m_data_tri.size() - 1;
    }
    else
    {
      int a = calculate_side(vet_nor[t],
                             vet_vertex[vet_tri[t].m_v[0]],
                             vet_vertex[vet_tri[i].m_v[0]],
                             vet_vertex[vet_tri[i].m_v[1]],
                             vet_vertex[vet_tri[i].m_v[2]]);

      if(a == CUT)
      {
        cut_triangle(vet_nor[t],
                     vet_vertex[vet_tri[t].m_v[0]],
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
      else if(a == FRONT)
      {
        add_triangle(vet_vertex[vet_tri[i].m_v[0]],
                     vet_vertex[vet_tri[i].m_v[1]],
                     vet_vertex[vet_tri[i].m_v[2]],
                     vet_tri_front,
                     vet_vertex_front,
                     vet_nor_front);
      }
      else if(a == BACK)
      {
        add_triangle(vet_vertex[vet_tri[i].m_v[0]],
                     vet_vertex[vet_tri[i].m_v[1]],
                     vet_vertex[vet_tri[i].m_v[2]],
                     vet_tri_back,
                     vet_vertex_back,
                     vet_nor_back);
      }
    }
  }

  //vet_tri.clear(); vet_vertex.clear(); vet_nor.clear();

  n->front = m_insert_rec(vet_tri_front, vet_vertex_front, vet_nor_front);
  n->back = m_insert_rec(vet_tri_back, vet_vertex_back, vet_nor_back);

  vet_tri_front.clear(); vet_vertex_front.clear(); vet_nor_front.clear();
  vet_tri_back.clear(); vet_vertex_back.clear(); vet_nor_back.clear();

  return n;
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
void cBsp::draw(VVector &eye)
{
  if(m_root)
    m_draw_rec(m_root, eye);
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
void cBsp::m_draw_node(cNodeBsp *node)
{
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
void cBsp::m_draw_rec(cNodeBsp *node, VVector &eye)
{
  if(!node)
    return;

  if(!node->back && !node->front)
  {
    // eh uma folha
    m_draw_node(node);
    return;
  }

  int t = node->tri;
  int s = calculate_side(m_data_nor[t], m_data_vet[m_data_tri[t].m_v[0]], eye);

  if(s == FRONT)
  {
    // estou na frente do no
    m_draw_rec(node->back, eye);
    m_draw_node(node);
    m_draw_rec(node->front, eye);
  }
  else if(s == BACK)
  {
    //estou atras do no
    m_draw_rec(node->front, eye);
    m_draw_node(node);
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
int select_triangle(std::vector<cTriangle> &vet_tri, std::vector<VVector> &vet_vertex, std::vector<VVector> &vet_nor)
{
  int selected_triangle = 0;
  int size_cut = 0;
  int best_cuts = 100000;

  // TODO
 //selected_triangle = (int) vet_tri.size() / 2;

  // busca um triangulo que não corte nenhum outro
  // caso não encontre, escolhe o triangulo que corte menos
  for(int i = 0; i < (int)vet_tri.size(); ++i)
  {
    size_cut = 0;
    for(int j = 0; j < (int) vet_tri.size(); ++j)
    {
      if(i != j)
      {
        int s = calculate_side(vet_nor[i],
                               vet_vertex[vet_tri[i].m_v[0]],
                               vet_vertex[vet_tri[j].m_v[0]],
                               vet_vertex[vet_tri[j].m_v[1]],
                               vet_vertex[vet_tri[j].m_v[2]]);
        if(s == CUT)
          ++size_cut;
      }
    }
    if(size_cut == 0)
    {
      // nao teve nenhum corte
      selected_triangle = i;
      break;
    }
    else
    {
      // quarda o triangulo que teve menor quantidade de cortes
      if(size_cut < best_cuts)
      {
        size_cut  = best_cuts;
        selected_triangle = i;
      }
    }
  }

  return selected_triangle;
}
