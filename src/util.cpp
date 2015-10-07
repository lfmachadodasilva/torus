/*
 * PUC-Rio - Pontificia Universidade Catolica
 *
 * Professor: Marcelo Gattass
 *
 * Arquivo: util.cpp
 *
 * Nome: Luiz Felipe Machado
 */

#include "util.h"

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// http://en.wikipedia.org/wiki/Line-plane_intersection
// http://en.wikipedia.org/wiki/Matrix_inversion#Methods_of_matrix_inversion
// http://softsurfer.com/Archive/algorithm_0104/algorithm_0104B.htm#Line-Plane Intersection
bool calculate_intersection(VVector a, VVector b, VVector p0, VVector nor, VVector &res)
{
  double da = Dot(a - p0, nor);
  double db = Dot(b - p0, nor);

  if(da > -TOL && da < TOL) da = 0.0f;
  if(db > -TOL && db < TOL) db = 0.0f;

  if(da * db < -TOL)
  {
    double f = fabs(da) / (fabs(da) + fabs(db));
    res = a + ((b - a) * f);
    return true;
  }

  return false;
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
void add_triangle(VVector a, VVector b, VVector c,
                  std::vector<cTriangle> &vet_tri,
                  std::vector<VVector> &vet_ver,
                  std::vector<VVector> &vet_nor)
{
  int idx_a, idx_b, idx_c;
  std::vector<VVector>:: const_iterator it;

  // verifica se o ponto jah existe
  it = find(vet_ver.begin(), vet_ver.end(), a);
  if(it == vet_ver.end())
  {
    vet_ver.push_back(a);
    idx_a = (int) vet_ver.size() - 1;
  }
  else
    idx_a = it - vet_ver.begin();

  it = find(vet_ver.begin(), vet_ver.end(), b);
  if(it == vet_ver.end())
  {
    vet_ver.push_back(b);
    idx_b = (int) vet_ver.size() - 1;
  }
  else
    idx_b= it - vet_ver.begin();

  it = find(vet_ver.begin(), vet_ver.end(), c);
  if(it == vet_ver.end())
  {
    vet_ver.push_back(c);
    idx_c = (int) vet_ver.size() - 1;
  }
  else
    idx_c = it - vet_ver.begin();

  vet_tri.push_back(cTriangle(idx_a, idx_b, idx_c));

  // calcula a normal
  VVector nor = Cross(b - a, c - a);
  nor.Normalize();
  vet_nor.push_back(nor);
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
int calculate_side(VVector &normal_plane,  VVector &p,  VVector &a,  VVector &b,  VVector &c)
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
int calculate_side(VVector &normal_plane,  VVector &p,  VVector &a)
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
void cut_triangle(VVector nor,
                  VVector p0,
                  int idx_tri,
                  std::vector<cTriangle> &vet_tri,
                  std::vector<VVector> &vet_vertex,
                  std::vector<VVector> &,
                  std::vector<cTriangle> &vet_tri_front,
                  std::vector<VVector> &vet_vertex_front,
                  std::vector<VVector> &vet_nor_front,
                  std::vector<cTriangle> &vet_tri_back,
                  std::vector<VVector> &vet_vertex_back,
                  std::vector<VVector> &vet_nor_back)
{
  // coordenadas do triangulo que vai ser cortado.
  VVector a = vet_vertex[vet_tri[idx_tri].m_v[0]];
  VVector b = vet_vertex[vet_tri[idx_tri].m_v[1]];
  VVector c = vet_vertex[vet_tri[idx_tri].m_v[2]];

  VVector ab, ac, bc;

  // verifica se corta o segmento AB
  bool b_ab = calculate_intersection(a, b, p0, nor, ab);

  // verifica se corta o segmento AC
  bool b_ac = calculate_intersection(a, c, p0, nor, ac);

  // verifica se corta o segmento BC
  bool b_bc = calculate_intersection(b, c, p0, nor, bc);

  int s;

  if(b_ab && !b_ac && !b_bc) // apenas o segmento ab
  {
    s =  calculate_side(nor, p0, ab, b, c);
    if(s == FRONT)
    {
      add_triangle(ab, b, c, vet_tri_front, vet_vertex_front, vet_nor_front);
      add_triangle(ab, c, a, vet_tri_back, vet_vertex_back, vet_nor_back);
    }
    else
    {
      add_triangle(ab, b, c, vet_tri_back, vet_vertex_back, vet_nor_back);
      add_triangle(ab, c, a, vet_tri_front, vet_vertex_front, vet_nor_front);
    }
  }


  else if(!b_ab && b_ac && !b_bc) // apenas o segmento ac
  {
    s =  calculate_side(nor, p0, ac, a , b);
    if(s == FRONT)
    {
      add_triangle(ac, a, b, vet_tri_front, vet_vertex_front, vet_nor_front);
      add_triangle(ac, b, c, vet_tri_back, vet_vertex_back, vet_nor_back);
    }
    else
    {
      add_triangle(ac, a, b, vet_tri_back, vet_vertex_back, vet_nor_back);
      add_triangle(ac, b, c, vet_tri_front, vet_vertex_front, vet_nor_front);
    }
  }


  else if(!b_ab && !b_ac && b_bc) // apenas o segmento bc
  {
    s =  calculate_side(nor, p0, bc, a , b);
    if(s == FRONT)
    {
      add_triangle(bc, a, b, vet_tri_front, vet_vertex_front, vet_nor_front);
      add_triangle(bc, c, a, vet_tri_back, vet_vertex_back, vet_nor_back);
    }
    else
    {
      add_triangle(bc, a, b, vet_tri_back, vet_vertex_back, vet_nor_back);
      add_triangle(bc, c, a, vet_tri_front, vet_vertex_front, vet_nor_front);
    }
  }

  // TODO (luizfelipe) verificar qual eh a melhor divisao, para isso, melhor seria
  // fazer a diferenca as areas e verificar qual eh a menor diferenca.

  else if(b_ab && b_ac && !b_bc) // cortou os segmentos ab e ac
  {
    s =  calculate_side(nor, p0, a, ab , ac);
    if(s == FRONT)
    {
      add_triangle(a, ab, ac, vet_tri_front, vet_vertex_front, vet_nor_front);

      add_triangle(b, ac,  ab, vet_tri_back, vet_vertex_back, vet_nor_back);
      add_triangle(b, c,  ac, vet_tri_back, vet_vertex_back, vet_nor_back);
    }
    else
    {
      add_triangle(a, ab, ac, vet_tri_back, vet_vertex_back, vet_nor_back);

      add_triangle(b, ac,  ab, vet_tri_front, vet_vertex_front, vet_nor_front);
      add_triangle(b, c,  ac, vet_tri_front, vet_vertex_front, vet_nor_front);
    }
  }


  else if(!b_ab && b_ac && b_bc ) // cortou os segmentos ac e bc
  {
    s =  calculate_side(nor, p0, c, ac, bc);
    if(s == FRONT)
    {
      add_triangle(c, ac, bc, vet_tri_front, vet_vertex_front, vet_nor_front);

      add_triangle(a, bc, ac, vet_tri_back, vet_vertex_back, vet_nor_back);
      add_triangle(a, b, bc, vet_tri_back, vet_vertex_back, vet_nor_back);
    }
    else
    {
      add_triangle(ac, bc, c, vet_tri_back, vet_vertex_back, vet_nor_back);

      add_triangle(a, bc, ac, vet_tri_front, vet_vertex_front, vet_nor_front);
      add_triangle(a, b, bc, vet_tri_front, vet_vertex_front, vet_nor_front);
    }
  }


  else if(b_ab && !b_ac && b_bc) // cortou os segmentos ab e bc
  {
    s =  calculate_side(nor, p0, b, bc, ab);
    if(s == FRONT)
    {
      add_triangle(b, bc, ab, vet_tri_front, vet_vertex_front, vet_nor_front);

      add_triangle(a, ab, bc, vet_tri_back, vet_vertex_back, vet_nor_back);
      add_triangle(a, bc, c, vet_tri_back, vet_vertex_back, vet_nor_back);
    }
    else
    {
      add_triangle(b, bc, ab, vet_tri_back, vet_vertex_back, vet_nor_back);

      add_triangle(a, ab, bc, vet_tri_front, vet_vertex_front, vet_nor_front);
      add_triangle(a, bc, c, vet_tri_front, vet_vertex_front, vet_nor_front);
    }
  }
  else
    return;
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
cBbox calculate_bbox(std::vector<VVector> &vet)
{
  cBbox bbox;

  bbox.max.x = vet[0].x;
  bbox.max.y = vet[0].y;
  bbox.max.z = vet[0].z;

  bbox.min.x = vet[0].x;
  bbox.min.y = vet[0].y;
  bbox.min.z = vet[0].z;

  for(int i = 1; i < (int)vet.size(); ++i)
  {
    bbox.max.x = max(bbox.max.x, vet[i].x);
    bbox.max.y = max(bbox.max.y, vet[i].y);
    bbox.max.z = max(bbox.max.z, vet[i].z);

    bbox.min.x = min(bbox.min.x, vet[i].x);
    bbox.min.y = min(bbox.min.y, vet[i].y);
    bbox.min.z = min(bbox.min.z, vet[i].z);
  }

  return bbox;
}
