#ifndef __RENDER_COMMON_H__
#define __RENDER_COMMON_H__

#include <assert.h>
#include "math/math_3d.h"
#include "utils/macro_utils.h"
#include "utils/defines.h"

using namespace std;

namespace render
{

struct Triangle
{
  int           state;
  int           attr;

  VECTOR3D      vertexs[3];
  VECTOR4D      colors[3];
  VECTOR2D      texcoords[3]; 
  VECTOR3D      normals[3];
  VECTOR3D      origin_vertexs[3];
};

struct OutPoint
{
  OutPoint& operator=(OutPoint &point)
  {
    VECTOR3D_COPY(&vertex, &(point.vertex));
    VECTOR4D_COPY(&color, &(point.color));
    VECTOR2D_COPY(&texcoord, &(point.texcoord));
    VECTOR3D_COPY(&normal, &(point.normal));
    VECTOR3D_COPY(&origin_vertex, &(point.origin_vertex));
    return *this;
  }
  VECTOR3D    vertex;
  VECTOR4D    color;
  VECTOR2D    texcoord;
  VECTOR3D    normal;
  VECTOR3D    origin_vertex;
};

class OutPointPackage
{
public:
  OutPointPackage(int my_count)
    : count(my_count),
      index(0)
  {
    if(my_count <= 0)
      my_count = my_count;
    points = new OutPoint[my_count];
  }
  ~OutPointPackage()
  {
    SAFE_DELETE_N(points);
  }

  void  AddPoint(OutPoint &point)   //copy only, not manage point memory
  {
    if(index > count)
      index = index;
    *(points + index) = point;
    ++index;
  }
  OutPoint*   GetPointByIndex(int idx) const
  {
    assert(idx < count);
    return (points + idx);
  }

  int   GetInvaildCount(){return index;}
private:
  int         count;
  int         index;
  OutPoint    *points;      //init memory firstly, copy point to this class.
};

}

#endif