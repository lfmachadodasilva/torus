// vector.h
// Visualização 3D
// W. Celes - celes@inf.puc-rio.br
// PUC-Rio, Jan 2008

#ifndef V_VECTOR_H
#define V_VECTOR_H

#include <stdio.h>
#include <math.h>

#define X 0
#define Y 1
#define Z 2

struct VVector
{
  double x, y, z;
  
  VVector (void) : x(0.0f), y(0.0f), z(0.0f)
  {
  }
  
  VVector (double vx, double vy, double vz)
  : x(vx), y(vy), z(vz)
  {
  }

  double get_axis(int axis)
  {
    if(axis == 0)
      return x;
    else if(axis == 1)
      return y;
    else if(axis == 2)
      return z;

    return -1;
  }

  void Set (double vx, double vy, double vz)
  {
    x = vx; y = vy; z = vz;
  }
  
  double SqrLength () const
  {
    return (x*x+y*y+z*z);
  }
  
  double Length () const
  {
    return (double)sqrt(x*x+y*y+z*z);
  }

  double Normalize ()
  {
    double l = Length();
    if (l != 0.0f)
    {
      double d = 1.0f/l;
      x *= d; y *= d; z *= d;
    }
    return l;
  }

  bool Equal(const VVector &other)
  {
    if(x == other.x && y == other.y && z == other.z)
      return true;
    return false;
  }

  bool operator==(const VVector &other)
  {
    if(x == other.x && y == other.y && z == other.z)
      return true;
    return false;
  }
  
  VVector& operator+= (const VVector& other)
  {
    x += other.x; y += other.y; z += other.z;
    return *this;
  }

  VVector& operator+= (double scalar)
  {
    x += scalar; y += scalar; z += scalar;
    return *this;
  }

  VVector& operator-= (const VVector& other)
  {
    x -= other.x; y -= other.y; z -= other.z;
    return *this;
  }

  VVector& operator-= (double scalar)
  {
    x -= scalar; y -= scalar; z -= scalar;
    return *this;
  }

  VVector operator- ()
  {
    VVector v(-x,-y,-z);
    return v;
  }

  VVector operator-(const VVector& other)
  {
    VVector v;
    v.x = x - other.x;
    v.y = y - other.y;
    v.z = z - other.z;
    return v;
  }

  VVector operator+(const VVector& other)
  {
    VVector v;
    v.x = x + other.x;
    v.y = y + other.y;
    v.z = z + other.z;
    return v;
  }

  VVector operator*(const VVector& other)
  {
    VVector v;
    v.x = x * other.x;
    v.y = y * other.y;
    v.z = z * other.z;
    return v;
  }

  VVector operator*(const double &other)
  {
    VVector v;
    v.x = x * other;
    v.y = y * other;
    v.z = z * other;
    return v;
  }

  VVector& operator*= (const VVector& other)
  {
    x *= other.x; y *= other.y; z *= other.z;
    return *this;
  }

  VVector& operator*= (double scalar)
  {
    x *= scalar; y *= scalar; z *= scalar;
    return *this;
  }

  VVector& operator/= (double scalar)
  {
    x /= scalar; y /= scalar; z /= scalar;
    return *this;
  }
  
  friend double Dot (const VVector& a, const VVector& b)
  {
    return a.x*b.x + a.y*b.y + a.z*b.z;
  }
  
  friend VVector Cross (const VVector& a, const VVector& b)
  {
    return VVector(a.y*b.z-b.y*a.z,b.x*a.z-a.x*b.z,a.x*b.y-b.x*a.y);
  }

  void Print (const char* label=0) const
  {
    printf("%s: %f  %f  %f\n", label ? label : "", x, y, z);
  }
  
};

#endif
