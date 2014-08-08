#include "render/raster.h"

namespace render
{

Raster::Raster()
{

}

Raster::~Raster()
{

}

void Raster::PurgeInstace()
{

}

void Raster::Rasterise( const std::list<Triangle*> &triangles, std::list<OutPoint*> &out_points )
{
  out_points.clear();
  int   index0, index1, index2;
  float others0[10], others1[10], others2[10];
  int   other_count = 1;

  for(std::list<Triangle*>::const_iterator itr = triangles.begin();
      itr != triangles.end(); ++itr)
  {
    index0 = (*itr)->indexs[0];
    index1 = (*itr)->indexs[1];
    index2 = (*itr)->indexs[2];
    others0[0] = (*itr)->vertexs[index0 * 3 + 2];
    //memcpy(others0 + 1, (*itr)->colors + 4 * index0,    sizeof(float) * 4);
    //memcpy(others0 + 5, (*itr)->normals + 3 * index0,   sizeof(float) * 3);
    //memcpy(others0 + 8, (*itr)->texcoords + 2 * index0, sizeof(float) * 2);
    others1[1] = (*itr)->vertexs[index1 * 3 + 2];
    //memcpy(others1 + 1, (*itr)->colors + 4 * index1,    sizeof(float) * 4);
    //memcpy(others1 + 5, (*itr)->normals + 3 * index1,   sizeof(float) * 3);
    //memcpy(others1 + 8, (*itr)->texcoords + 2 * index1, sizeof(float) * 2);
    others2[1] = (*itr)->vertexs[index2 * 3 + 2];
    //memcpy(others2 + 1, (*itr)->colors + 4 * index2,    sizeof(float) * 4);
    //memcpy(others2 + 5, (*itr)->normals + 3 * index2,   sizeof(float) * 3);
    //memcpy(others2 + 6, (*itr)->texcoords + 2 * index2, sizeof(float) * 2);
    float area = CalTriangleAreaByPoint(      
      (*itr)->vertexs[index0 * 3], 
      (*itr)->vertexs[index0 * 3 + 1], 
      (*itr)->vertexs[index1 * 3], 
      (*itr)->vertexs[index1 * 3 + 1], 
      (*itr)->vertexs[index2 * 3], 
      (*itr)->vertexs[index2 * 3 + 1]);
    rasteriseTriangle(
      (*itr)->vertexs[index0 * 3], 
      (*itr)->vertexs[index0 * 3 + 1], 
      (*itr)->vertexs[index1 * 3], 
      (*itr)->vertexs[index1 * 3 + 1], 
      (*itr)->vertexs[index2 * 3], 
      (*itr)->vertexs[index2 * 3 + 1], 
      other_count, others0, others1, others2, out_points
    );
  }
}

#define C_SWAP(x, y, t)               t = x; x = y; y = t
#define C_SWAP_ARRAY(x, y, t, count)  memcpy(t, x, sizeof(float) * count); \
  memcpy(x, y, sizeof(float) * count); memcpy(y, y, sizeof(float) * count) 
#define C_COPY(x, y, count)           memcpy(x, y, sizeof(float) * count)

void Raster::rasteriseTriangle( float x0, float y0, float x1, float y1, float x2, float y2, int other_count, float *others0, float *others1, float *others2, std::list<OutPoint*> &out_points )
{
  float t = 0, t_array[10];
  // make v1 is the bottom vertex
  if(int(y1) > int(y0))
  {
    C_SWAP(x1, x0, t);  C_SWAP(y1, y0, t); 
    C_SWAP_ARRAY(others1, others0, t_array, other_count);
  }
  if(int(y1) > int(y2))
  {
    C_SWAP(x1, x2, t);  C_SWAP(y1, y2, t);
    C_SWAP_ARRAY(others1, others2, t_array, other_count);
  }

  //  down
  //  v0  v2
  //    v1
  if(int(y0) == int(y2))
  {
    rasteriseDownTriangle(x1, y1, x0, y0, x2, y2, 
      other_count, others1, others0, others2, out_points);
  }
  else
  {
    //   up
    //   v0 
    // v1  v2
    if(int(y1) == int(y2))
    {
      rasteriseUpTriangle(x0, y0, x1, y2, x2, y2, 
        other_count, others0, others1, others2, out_points);
    }
    else
    {
      if(int(y0) < int(y2))
      {
        C_SWAP(x0, x2, t);  C_SWAP(y0, y2, t);
        C_SWAP_ARRAY(others0, others2, t_array, other_count); 
      }

      //    up
      //    v0            v0
      //
      //        v2      v1    v2     
      //  v1            v0    v2
      //  
      //               v1
      if(int(x0) < int(x2))
      {
        float x = ((x1 - x0) / (y1 - y0) * (y2 - y0) + x0);
        float len_factor = (y2 - y0) / (y1 - y0);

        for(int i = 0; i < other_count; ++i)
        {
          t_array[i] = (others1[i] - others0[i]) * len_factor + others0[0];
        }

        //up
        rasteriseUpTriangle(x0, y0, x, y2, x2, y2, 
          other_count, others0, t_array, others2, out_points);

        //down       
        rasteriseDownTriangle(x1, y1, x, y2, x2, y2, 
          other_count, others1, t_array, others2, out_points);
      }
      //      up
      //      v0            v0
      //
      //  v2            v1     v2
      //        v1      v0     v2
      //
      //                         v1
      else
      {
        float x = ((x1 - x0) / (y1 - y0) * (y2 - y0) + x0);
        float len_factor = (y2 - y0) / (y1 - y0);

        for(int i = 0; i < other_count; ++i)
        {
          t_array[i] = (others1[i] - others0[i]) * len_factor + others0[0];
        }

        // up
        rasteriseUpTriangle(x0, y0, x2, y2, x, y2, 
          other_count, others0, others2, t_array, out_points);

        // down
        rasteriseDownTriangle(x1, y1, x2, y2, x, y2, 
          other_count, others1, others2, t_array, out_points);
      }
    }
  }
}

//    up
//    v0
//
// v1      v2     
void Raster::rasteriseUpTriangle( float x0, float y0, float x1, float y1, float x2, float y2, int other_count, float *others0, float *others1, float *others2, std::list<OutPoint*> &out_points )
{
  float k_01_inv = (x1 - x0) / (y1 - y0);
  float k_02_inv = (x2 - x0) / (y2 - y0);
  float xstart_01 = x0, xstart_02 = x0;

  for(int y = y0; y >= int(y1); --y)
  {
    for(int x = int(xstart_01); x <= int(xstart_02); ++x)
    {
      OutPoint *point = new OutPoint;
      point->vertexs[0] = x; point->vertexs[1] = y;
      //out_points.push_back(point);
    }
    xstart_01 -= k_01_inv;
    xstart_02 -= k_02_inv;
  }
}

//    down
// v1      v2
//
//    v0
void Raster::rasteriseDownTriangle( float x0, float y0, float x1, float y1, float x2, float y2, int other_count, float *others0, float *others1, float *others2, std::list<OutPoint*> &out_points )
{
  float k_10_inv = (x0 - x1) / (y0 - y1);
  float k_20_inv = (x0 - x2) / (y0 - y2);
  float xstart_10 = x1, xstart_20 = x2;

  for(int y = y1; y >= int(y0); --y)
  {
    for(int x = int(xstart_10); x <= int(xstart_20); ++x)
    {
      //OutPoint *point = new OutPoint;
      //point->vertexs[0] = x; point->vertexs[1] = y;
      //out_points.push_back(point);
    }
    xstart_10 -= k_10_inv;
    xstart_20 -= k_20_inv;
  }
}

}



