/*
 * PUC-Rio - Pontificia Universidade Catolica
 *
 * Professor: Marcelo Gattass
 *
 * Arquivo: mesh.h
 *
 * Nome: Luiz Felipe Machado
 */

#ifndef MESH_H
#define MESH_H

#include "util.h"

class cMesh
{
private:

  std::vector<cTriangle> m_data_tri;

  std::vector<VVector> m_data_ver;

  std::vector<VVector> m_data_nor;

  cBbox m_bbox;

public:

    cMesh(void) {}

    bool create_mesh(std::vector<VVector> vet, int dx, int dy);

    void get_att(std::vector<cTriangle> &vet_tri,
                 std::vector<VVector> &vet_ver,
                 std::vector<VVector> &vet_nor)
    {
      vet_tri = m_data_tri;
      vet_ver = m_data_ver;
      vet_nor = m_data_nor;
    }

    int get_num_ver(void) { return (int) m_data_ver.size(); }

    void draw(void);

    void calculate_bbox_mesh(void) { m_bbox = calculate_bbox(m_data_ver); }
    cBbox get_bbox(void) { return m_bbox; }

};

#endif // MESH_H
