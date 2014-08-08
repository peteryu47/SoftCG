#ifndef __RASTER_H__
#define __RASTER_H__

#include <assert.h>
#include <list>

#include "utils/defines.h"
#include "utils/macro_utils.h"
#include "render/render_common.h"

namespace render
{

class Raster
{
private:
  Raster();
public:
  ~Raster();

  static Raster* GetInstance()
  {
    static Raster raster;
    return &raster;
  }

  void  PurgeInstace();

  void  Rasterise(const std::list<Triangle*> &triangles, std::list<OutPoint*> &out_points);

private:
  void  rasteriseTriangle(float x0, float y0, float x1, float y1, 
    float x2, float y2, int other_count, float *others0, float *others1, 
    float *others2, std::list<OutPoint*> &out_points);
  void  rasteriseUpTriangle(float x0, float y0, float x1, float y1, 
    float x2, float y2, int other_count, float *others0, float *others1, 
    float *others2, std::list<OutPoint*> &out_points);
  void  rasteriseDownTriangle(float x0, float y0, float x1, float y1,  
    float x2, float y2, int other_count, float *others0, float *others1, 
    float *others2, std::list<OutPoint*> &out_points);
};

}

#endif