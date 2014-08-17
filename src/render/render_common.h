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
  OutPoint& operator=(const OutPoint &point)
  {
    vertexs[0]    = point.vertexs[0];   vertexs[1]    = point.vertexs[1]; 
    colors[0]     = point.colors[0];    colors[1]     = point.colors[1]; 
    colors[2]     = point.colors[2];    colors[3]     = point.colors[3]; 
    texcoords[0]  = point.texcoords[0]; texcoords[1]  = point.texcoords[1]; 
  }
  int         vertexs[2];
  float       colors[4];
  float       texcoords[2];
};

struct OutPointPackage
{
  OutPointPackage(int my_count)
    : count(my_count),
      index(0)
  {
    points = (OutPoint*)malloc(sizeof(OutPoint) * count);
  }
  ~OutPointPackage()
  {
    free(points);
  }
  void  AddPoint(OutPoint* point)
  {
    *(points + index) = *point;
    ++index;
  }
  int         count;
  int         index;
  OutPoint    *points;
};

}

#endif