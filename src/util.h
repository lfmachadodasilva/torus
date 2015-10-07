/*
 * PUC-Rio - Pontificia Universidade Catolica
 *
 * Professor: Marcelo Gattass
 *
 * Arquivo: util.h
 *
 * Nome: Luiz Felipe Machado
 */

#ifndef UTIL_H
#define UTIL_H

#include <math.h>
#include <algorithm>
#include <vector>
using namespace std;

#include "bbox.h"
#include "triangle.h"
#include "vector.h"

enum { FRONT = 0, BACK, CUT};

#define TOL 1.0e-5

/**
  * @brief Calcular intercessao
  *
  * Essa funcao verifica se o plano corta o segmento de reta. Caso corte ele retorna na
  * na variavel res a posicao que foi cortada.
  *
  * @param[in] a - ponto do segmento de reta
  * @param[in] b - ponto do segmento de reta
  * @param[in] p0 - ponto do plano
  * @param[in] nor - normal do plano
  * @param[out] pos do segmento que o plano cortou
  * @return true caso o plano tenha cortado o segmento ou
 *                      false caso o plano nao tenha cortado o segmento
  */
bool calculate_intersection(VVector a, VVector b, VVector p0, VVector nor, VVector &res);

/**
  * @brief Adicionar triangulo
  *
  * Essa funcao adiciona os vertices a, b e c a estrutura. os vertices nao sao duplicados, ou seja
  * antes de inserir eh verificado se o vertice ja existe na estrutura.
  *
  * @param[in] a - ponto do triangulo
  * @param[in] b - ponto do triangulo
  * @param[in] c - ponto do triangulo
  * @param[in] vet_tri - vetor com a lista de triangulos
  * @param[in] vet_vet - vetor com a lista de vertices
  * @param[in] vet_nor - vetor com a lista de normais
  */
void add_triangle(VVector a, VVector b, VVector c,
                  std::vector<cTriangle> &vet_tri,
                  std::vector<VVector> &vet_ver,
                  std::vector<VVector> &vet_nor);

/**
  * @brief Calcular lado
  *
  * Verifica se  o triangulo passado (3 pontos) estah na frente, atras ou devera ser cortado em
  * relacao ao plano (ponto e normal passado).
  *
  * @param[in] normal_plane - normal do plano
  * @param[in] p - ponto do plano
  * @param[in] a - ponto do triangulo
  * @param[in] b - ponto do triangulo
  * @param[in] c - ponto do triangulo
  * @return int FRONT - esta na frente, BACK - esta atras, CUT - devera cortar
  */
int calculate_side(VVector &normal_plane, VVector &p, VVector &a, VVector &b, VVector &c);

/**
  * @brief Calcular lado
  *
  * Verifica se  o ponto passado estah na frente ou atras relacao ao plano (ponto e normal passado).
  *
  * @param[in] normal_plane - normal do plano
  * @param[in] p - ponto do plano
  * @param[in] a - ponto
  * @return int FRONT - esta na frente, BACK - esta atras
  */
int calculate_side(VVector &normal_plane,  VVector &p,  VVector &a);

// TODO fazer comentario
void cut_triangle(VVector nor,
                  VVector p0,
                  int idx_tri,
                  std::vector<cTriangle> &vet_tri,
                  std::vector<VVector> &vet_vertex,
                  std::vector<VVector> &vet_nor,
                  std::vector<cTriangle> &vet_tri_front,
                  std::vector<VVector> &vet_vertex_front,
                  std::vector<VVector> &vet_nor_front,
                  std::vector<cTriangle> &vet_tri_back,
                  std::vector<VVector> &vet_vertex_back,
                  std::vector<VVector> &vet_nor_back);

// TODO fazer comentario
cBbox calculate_bbox(std::vector<VVector> &vet);


#endif // UTIL_H
