/*
 * PUC-Rio - Pontificia Universidade Catolica
 *
 * Professor: Marcelo Gattass
 *
 * Arquivo: main.cpp
 *
 * Nome: Luiz Felipe Machado
 */

#include "cstdlib"
#include "stdio.h"
#include <math.h>
#include <algorithm>
using namespace std;

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

#include "util.h"
#include "bsp.h"
#include "torus.h"
#include "manipulator.h"
#include "kdtree.h"
#include "mesh.h"

static cTorus  *torus;
static cBsp    *bsp;
static cKdtree *kdtree;
static cMesh   *mesh;
static cBbox   *bbox = NULL;

float white[4] = {1.0f, 1.0f, 1.0f, 1.0f};
float red[4]   = {1.0f, 0.1f, 0.1f, 1.0f};
float blue[4]  = {0.0f, 0.0f, 1.0f, 1.0f};
float green[4] = {0.0f, 1.0f, 0.0f, 1.0f};

bool b_fill       = true;
bool b_depth_test = true;
bool b_bbox       = false;
bool b_axis       = true;

bool b_render_torus  = false;
bool b_render_bsp    = true;
bool b_render_kdtree = false;
bool b_render_mesh   = false;

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
static void Init ()
{
  std::vector<VVector> vet_ver, vet_nor;
  std::vector<cTriangle> vet_tri;

  torus = new cTorus(20, 20, 240, 45);

  torus->get_att(vet_tri, vet_ver, vet_nor);

  printf("-----------------------------------------------\n");

  printf("Numero de triangulos no torus:\t\t%d\n", torus->get_num_triangles());

  bsp = new cBsp();

  bsp->create_bsp(vet_tri, vet_ver, vet_nor);

  //printf("Numero de triangulos na bsp:\t\t%d\n", bsp->get_num_triangles());
  //
  //vet_ver.push_back(VVector(0,0,0));
  //vet_ver.push_back(VVector(100,0,0));
  //vet_ver.push_back(VVector(100,100,0));
  //vet_ver.push_back(VVector(0,100,0));
  //
  //mesh = new cMesh();
  //
  //mesh->create_mesh(vet_ver, 1, 1);
  //
  //mesh->get_att(vet_tri, vet_ver, vet_nor);
  //
  //kdtree = new cKdtree();

  //kdtree->create_kdtree(vet_tri, vet_ver, vet_nor);

  //printf("Numero de triangulos na kdtree:\t\t%d\n", kdtree->get_num_triangles());

  printf("-----------------------------------------------\n");

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}

static void Redraw ()
{
  static bool first = true;
  if (first) {
    Init();
    first = false;
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  int vp[4];
  glGetIntegerv(GL_VIEWPORT,vp);

  if(b_render_torus)
    bbox = &torus->get_bbox();
  else if(b_render_bsp)
    bbox = &bsp->get_bbox();
  else if(b_render_kdtree)
    bbox = &kdtree->get_bbox();
  else if(b_render_mesh)
    bbox = &mesh->get_bbox();

  float xcenter = (bbox->min.x + bbox->max.x) / 2;
  float ycenter = (bbox->min.y + bbox->max.y) / 2;
  float zcenter = (bbox->min.z + bbox->max.z) / 2;
  float zdist = 10 * (bbox->max.z - bbox->min.z);

  if(zdist < TOL)
    zdist = 100.0f;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(50.0f, (float)vp[2]/vp[3], 0.1f, 5000.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  VManipulator* manip = VManipulator::Instance();
  manip->SetZCenter(zdist);
  manip->Load();

  VVector eye(0.0f, 0.0f, 300.0f);

  gluLookAt(xcenter, ycenter, zcenter-zdist,//eye.x, eye.y, eye.z,//
            xcenter, ycenter, zcenter,
            0.0f, 1.0f, 0.0f);

  float pos[4] = {1000.0f, 1000.0f, 1000.0f, 1.0f};
  glLightfv(GL_LIGHT0, GL_POSITION, pos);

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 30.0f);

  if(!b_fill)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  if(b_depth_test)
    glEnable(GL_DEPTH_TEST);
  else
    glDisable(GL_DEPTH_TEST);

  if(b_render_torus)
    torus->draw();
  else if(b_render_bsp)
    bsp->draw(eye);
  else if(b_render_kdtree)
    kdtree->draw(eye);
  else if(b_render_mesh)
    mesh->draw();

  glDisable(GL_LIGHTING);
  {
    if(b_bbox)
    {
      glColor4fv(white);
      bbox->draw();
    }

    if(b_axis)
    {
      // render axis
      glBegin(GL_LINES);
      {
        glColor4fv(red); // X red
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(100.0f, 0.0f, 0.0f);

        glColor4fv(green); // Y green
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 100.0f, 0.0f);

        glColor4fv(blue); // Z blue
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 100.0f);
      }
      glEnd();
    }
  }
  glEnable(GL_LIGHTING);

  glutSwapBuffers();
}

static void Keyboard (unsigned char key, int, int)
{
  switch (key)
  {
  case 'q':
    exit(0);
    break;
  case 'f':
    b_fill = !b_fill;
    b_fill ? printf("Desenha os objetos solidos.\n") : printf("Desenha apenas as primitivas.\n");
    break;
  case 'z':
    b_depth_test = !b_depth_test;
    b_depth_test ? printf("Depth teste ligado.\n") : printf("Depth teste desligado.\n");
    break;
  case 'b':
    b_bbox = !b_bbox;
    b_bbox ? printf("Desenha bouding box.\n") : printf("Nao desenha bouding box.\n");
    break;
  case 'e':
    b_axis = !b_axis;
    b_axis ? printf("Desenha o eixo.\n") : printf("Nao desenha o eixo.\n");
    break;
  case '1':
    b_render_torus = true;
    b_render_bsp = false;
    b_render_kdtree = false;
    b_render_mesh = false;
    printf("Desenha o torus normal.\n");
    break;
  case '2':
    b_render_torus = false;
    b_render_bsp = true;
    b_render_kdtree = false;
    b_render_mesh = false;
    printf("Desenha o torus usando bsp.\n");
    break;
  case '3':
    b_render_torus = false;
    b_render_bsp = false;
    b_render_kdtree = true;
    b_render_mesh = false;
    printf("Desenha o torus usando kdtree.\n");
    break;
  case '4':
    b_render_torus = false;
    b_render_bsp = false;
    b_render_kdtree = false;
    b_render_mesh = true;
    printf("Desenha a malha.\n");
    break;
  }

  Redraw();
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
  glutInitWindowPosition( (glutGet(GLUT_SCREEN_WIDTH)-800)/2,
                          (glutGet(GLUT_SCREEN_HEIGHT)-600)/2);
  glutInitWindowSize(800,600);
  glutCreateWindow("Torus");
  glutDisplayFunc(Redraw);
  glutKeyboardFunc(Keyboard);
  glutMainLoop();

  delete torus;

  return EXIT_SUCCESS;
}
