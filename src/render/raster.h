#ifndef __RASTER_H__
#define __RASTER_H__

#include <assert.h>
#include <vector>
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

  void  Rasterise(const std::list<Triangle*> &triangles, std::vector<OutPointPackage*> &out_point_packges);

private:
  void  rasteriseTriangle(float x0, float y0, float x1, float y1, 
    float x2, float y2, int other_count, float *others0, float *others1, 
    float *others2, OutPointPackage* out_point_package);
  void  rasteriseUpTriangle(float x0, float y0, float x1, float y1, 
    float x2, float y2, int other_count, float *others0, float *others1, 
    float *others2, OutPointPackage* out_point_package);
  void  rasteriseDownTriangle(float x0, float y0, float x1, float y1,  
    float x2, float y2, int other_count, float *others0, float *others1, 
    float *others2, OutPointPackage* out_point_package);

  int CalTriangleFitAreaPixelByPoint(float x0, float y0, float x1, float y1, float x2, float y2);
};

}

#endif