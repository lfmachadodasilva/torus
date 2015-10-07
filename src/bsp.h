/*
 * PUC-Rio - Pontificia Universidade Catolica
 *
 * Professor: Marcelo Gattass
 *
 * Arquivo: bsp.h
 *
 * Nome: Luiz Felipe Machado
 */

#ifndef BSP_H
#define BSP_H

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
class cNodeBsp
{
private:
public:
  int tri; // indice do triangulo no vetor de triangulos m_data_tri

  cNodeBsp *front;
  cNodeBsp *back;

  cNodeBsp(void) : front(NULL), back(NULL) {}
};


/**
  * @brief Bsp tree class
  *
  * Links:
  * http://en.wikipedia.org/wiki/Binary_space_partitioning
  * http://www.devmaster.net/articles/bsp-trees/
  * http://www.blitzbasic.com/codearcs/codearcs.php?code=1133
  * http://web.cs.wpi.edu/~matt/courses/cs563/talks/bsp/document.html
  * http://maven.smith.edu/~mcharley/bsp/createbsptree.html
  */
class cBsp
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
  cNodeBsp *m_root;

  /**
    * @brief Bouding box
    */
  cBbox m_bbox;

  void m_draw_rec(cNodeBsp *node, VVector &eye)  ;
  void m_draw_node(cNodeBsp *node)  ;

  cNodeBsp* m_insert_rec(std::vector<cTriangle> &vet_tri,
                      std::vector<VVector> &vet_vertex,
                      std::vector<VVector> &vet_nor);

public:
  cBsp(void) : m_root(NULL) {}

  void draw(VVector &eye)  ;

  void create_bsp(std::vector<cTriangle> &vet_tri,
                  std::vector<VVector> &vet_vertex,
                  std::vector<VVector> &vet_nor);

  int get_num_triangles(void) { return (int)m_data_tri.size(); }

  void calculate_bbox_bsp(void) { m_bbox = calculate_bbox(m_data_vet); }

  cBbox get_bbox(void) { return m_bbox; }
};

#endif // BSP_H
