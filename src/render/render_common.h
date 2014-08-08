#ifndef __RENDER_COMMON_H__
#define __RENDER_COMMON_H__

#include "math/math_3d.h"
#include "utils/macro_utils.h"
#include "utils/defines.h"

using namespace std;

namespace render
{

struct Triangle
{
public:
  Triangle()
    : state(0),
      attr(0),
      vertexs(NULL),
      colors(NULL),
      normals(NULL),
      texcoords(NULL)
  {
    indexs[0] = indexs[1] = indexs[2] = 0;
  }

  int           state;
  int           attr;

  float         *vertexs;     //3
  float         *colors;      //4
  float         *normals;     //3
  float         *texcoords;   //2
  int           indexs[3];
};

struct OutPoint
{
  int         vertexs[2];
  float       colors[4];
  float       texcoords[2];
};

struct OutPointPackage
{
  int         count;
  OutPoint    *points;
};

}

#endif