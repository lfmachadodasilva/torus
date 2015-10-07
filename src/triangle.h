/*
 * PUC-Rio - Pontificia Universidade Catolica
 *
 * Professor: Marcelo Gattass
 *
 * Arquivo: torus.h
 *
 * Nome: Luiz Felipe Machado
 */

#ifndef TRIANGLE_H
#define TRIANGLE_H

//#define X 0
//#define Y 1
//#define Z 2

//static int X = 0;
//static int Y = 1;
//static int Z = 2;

class cTriangle
{
  private:

  public:

  int m_v[3];

  cTriangle(void)
  {
    m_v[0] = -1;
    m_v[1] = -1;
    m_v[2] = -1;
  }

  cTriangle(int a, int b, int c)
  {
    this->m_v[0] = a;
    this->m_v[1] = b;
    this->m_v[2] = c;
  }

  void operator = (const cTriangle &other)
  {
    this->m_v[0] = other.m_v[0];
    this->m_v[1] = other.m_v[1];
    this->m_v[2] = other.m_v[2];
  }

  bool operator == (const cTriangle &other)
  {
    if(this->m_v[0] == other.m_v[0] &&
       this->m_v[1] == other.m_v[1] &&
       this->m_v[2] == other.m_v[2])
      return true;
    return false;
  }

};

#endif // TRIANGLE_H
