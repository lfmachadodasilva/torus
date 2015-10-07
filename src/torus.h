/*
 * PUC-Rio - Pontificia Universidade Catolica
 *
 * Professor: Marcelo Gattass
 *
 * Arquivo: torus.h
 *
 * Nome: Luiz Felipe Machado
 */

#ifndef TORUS_H
#define TORUS_H

#include <vector>

#include "bbox.h"
#include "triangle.h"
#include "vector.h"

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
class cTorus
{

private:

    /**
    * @brief Vector with vertex coordinates
    */
    std::vector<VVector> m_vet;

    /**
    * @brief Vector with index of coordinates of triangles.
    */
    std::vector<cTriangle> m_tri;

    /**
    * @brief Vector with normal vector of each triangle.
    */
    std::vector<VVector> m_nor;

    /**
    * @brief Inner radius of the torus.
    */
    double m_inner_radius;

    /**
    * @brief Outer radius of the torus.
    */
    double m_outer_radius;

    /**
    * @brief Number of sides in a cross section.
    */
    int m_slides;

    /**
    * @brief Number of rings in the torus.
    */
    int m_rings;

    void m_create_mesh(void);

    cBbox m_bbox;

public:

    cTorus(int slides, int rings, double inner_radius, double outer_radius)
    {
      m_slides = slides;
      m_rings = rings;
      m_inner_radius = inner_radius;
      m_outer_radius = outer_radius;

      m_create_mesh();

      calculate_bbox();
    }

    void draw(void);

    void calculate_bbox(void);
    cBbox get_bbox(void) { return m_bbox; }

    void get_att(std::vector<cTriangle> &vet_tri ,std::vector<VVector> &vet_vertex, std::vector<VVector> &vet_nor)
    {
      vet_tri = m_tri;
      vet_vertex = m_vet;
      vet_nor = m_nor;
    }

    int get_num_triangles(void) { return (int)m_tri.size(); }

};

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------

#endif // TORUS_H
