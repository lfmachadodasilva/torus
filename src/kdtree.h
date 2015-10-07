/*
 * PUC-Rio - Pontificia Universidade Catolica
 *
 * Professor: Marcelo Gattass
 *
 * Arquivo: kdtree.h
 *
 * Nome: Luiz Felipe Machado
 */

#ifndef KDTREE_H
#define KDTREE_H

#include <vector>

#include "util.h"
#include "triangle.h"
#include "vector.h"
#include "bbox.h"

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// Links:
//

/**
  * @brief Node class
  */
class cNodeKdtree
{
private:
public:

  /**
    * @brief Variavel que controla se eh uma folha ou nao.
	*
	* No caso de ser uma folha, sera utilizado o indice do triangulo (tri).
	* No caso de ser um no qualquer, sera utilizado o vertice e o valor do vertice
    */
  bool is_leaf;
  
  /**
    * @brief indice do triangulo no vetor de triangulos m_data_tri
	*
	* Apenas utilizado quando for uma folha (is_ leaf == true)
    */
  int tri; 
  
  int axis;
  double axis_value;

  cNodeKdtree *front;
  cNodeKdtree *back;

  cNodeKdtree()
  {
    front = back = NULL;
    tri = axis = axis_value = -1;
    is_leaf = false;
  }
};

/**
  * @brief Kdtree tree class
  *
  * Links:
  */
class cKdtree
{
private:

  /**
    * @brief Vector with vertex coordinates
    */
  std::vector<VVector> m_data_vet;

  /**
    * @brief Vector with index of vertex coordinates.
    */
  std::vector<cTriangle> m_data_tri;

  /**
    * @brief Vector with normal vector of each triangle.
    */
  std::vector<VVector> m_data_nor;

  /**
    * @brief Root node of tree
    */
  cNodeKdtree *m_root;

  /**
    * @brief Bouding box
    */
  cBbox m_bbox;

  cNodeKdtree* m_insert_rec(std::vector<cTriangle> &vet_tri,
                            std::vector<VVector> &vet_vertex,
                            std::vector<VVector> &vet_nor,
                            int depth);

  void m_draw_rec(cNodeKdtree *node, VVector &eye);

  void m_draw_node(cNodeKdtree *node);

public:
  cKdtree(void) : m_root(NULL) {}

  void create_kdtree(std::vector<cTriangle> &vet_tri,
                  std::vector<VVector> &vet_vertex,
                  std::vector<VVector> &vet_nor);

  int get_num_triangles(void) { return (int)m_data_tri.size(); }

  void calculate_bbox_kdtree(void) { m_bbox = calculate_bbox(m_data_vet); }

  void draw(VVector &eye);

  cBbox get_bbox(void) { return m_bbox; }
};

#endif // KDTREE_H
