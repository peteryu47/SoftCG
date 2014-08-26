#include "render/raster.h"

#include "utils/macro_utils.h"

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

void Raster::Rasterise( const std::vector<Triangle*> &triangles, std::vector<OutPointPackage*> &out_point_packges )
{
  int   index0, index1, index2;
  float others0[10], others1[10], others2[10];
  int   other_count = 8;

  for(std::vector<Triangle*>::const_iterator itr = triangles.begin();
      itr != triangles.end(); ++itr)
  {
    others0[0] = (*itr)->vertexs[0].z;
    memcpy(others0 + 1, &(*itr)->colors[0], sizeof(float) * 4);
    memcpy(others0 + 5, &(*itr)->texcoords[0], sizeof(float) * 2);
    others1[0] = (*itr)->vertexs[1].z;
    memcpy(others1 + 1, &(*itr)->colors[1], sizeof(float) * 4);
    memcpy(others1 + 5, &(*itr)->texcoords[1], sizeof(float) * 2);
    others2[0] = (*itr)->vertexs[2].z;
    memcpy(others2 + 1, &(*itr)->colors[2], sizeof(float) * 4);
    memcpy(others2 + 5, &(*itr)->texcoords[2], sizeof(float) * 2);
    int point_count = CalTriangleFitAreaPixelByPoint(      
      (*itr)->vertexs[0].x, 
      (*itr)->vertexs[0].y, 
      (*itr)->vertexs[1].x, 
      (*itr)->vertexs[1].y, 
      (*itr)->vertexs[2].x, 
      (*itr)->vertexs[2].y);
    OutPointPackage *out_point_packege = new OutPointPackage(point_count);
    rasteriseTriangle(
      (*itr)->vertexs[0].x, 
      (*itr)->vertexs[0].y, 
      (*itr)->vertexs[1].x, 
      (*itr)->vertexs[1].y, 
      (*itr)->vertexs[2].x, 
      (*itr)->vertexs[2].y,
      other_count, others0, others1, others2, out_point_packege
    );
    out_point_packges.push_back(out_point_packege);
  }
}

#define C_SWAP(x, y, t)               t = x; x = y; y = t
#define C_SWAP_ARRAY(x, y, t, count)  memcpy(t, x, sizeof(float) * count); \
  memcpy(x, y, sizeof(float) * count); memcpy(y, t, sizeof(float) * count) 
#define C_COPY(x, y, count)           memcpy(x, y, sizeof(float) * count)

void Raster::rasteriseTriangle( float x0, float y0, float x1, float y1, 
                                float x2, float y2, int other_count, 
                                float *others0, float *others1, float *others2, 
                                OutPointPackage* out_point_package )
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
      other_count, others1, others0, others2, out_point_package);
  }
  else
  {
    //   up
    //   v0 
    // v1  v2
    if(int(y1) == int(y2))
    {
      rasteriseUpTriangle(x0, y0, x1, y2, x2, y2, 
        other_count, others0, others1, others2, out_point_package);
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
          t_array[i] = (others1[i] - others0[i]) * len_factor + others0[i];
        }

        //up
        rasteriseUpTriangle(x0, y0, x, y2, x2, y2, 
          other_count, others0, t_array, others2, out_point_package);

        //down       
        rasteriseDownTriangle(x1, y1, x, y2, x2, y2, 
          other_count, others1, t_array, others2, out_point_package);
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
          t_array[i] = (others1[i] - others0[i]) * len_factor + others0[i];
        }

        // up
        rasteriseUpTriangle(x0, y0, x2, y2, x, y2, 
          other_count, others0, others2, t_array, out_point_package);

        // down
        rasteriseDownTriangle(x1, y1, x2, y2, x, y2, 
          other_count, others1, others2, t_array, out_point_package);
      }
    }
  }
}

#define FOR_START for(int i = 0; i < other_count; ++i){
#define FOR_END   }

//    up
//    v0
//
// v1      v2     
void Raster::rasteriseUpTriangle( float x0, float y0, float x1, float y1, float x2, float y2, 
                                  int other_count, float *others0, float *others1, float *others2, 
                                  OutPointPackage* out_point_package )
{
  float k_01_inv = (x1 - x0) / (y1 - y0);
  float k_02_inv = (x2 - x0) / (y2 - y0);
  float xstart_01 = x0, xstart_02 = x0;

  float other_01y_start[10];
  float other_02y_start[10];
  float other_01y_step[10];
  float other_02y_step[10];
  float other_x_start[10];
  float other_x_step[10];

  memcpy(other_01y_start, others0, sizeof(float) * other_count);
  memcpy(other_02y_start, others0, sizeof(float) * other_count);
  FOR_START
  other_01y_step[i] = (others1[i] - others0[i]) / (int(y0) - int(y1));
  other_02y_step[i] = (others2[i] - others0[i]) / (int(y0) - int(y1));
  FOR_END

  OutPoint point;
  for(int y = y0; y >= int(y1); --y)
  {
    memcpy(other_x_start, other_01y_start, sizeof(float) * other_count);
    FOR_START other_x_step[i] = (other_02y_start[i] - other_01y_start[i]) / (int(xstart_02) - int(xstart_01)); FOR_END
    for(int x = int(xstart_01); x <= int(xstart_02); ++x)
    {
      point.vertex.x= x; point.vertex.y = y; point.vertex.z = other_x_start[0];
      memcpy(&point.color, other_x_start + 1, sizeof(float) * 4);
      memcpy(&point.texcoord, other_x_start + 5, sizeof(float) * 2);
      out_point_package->AddPoint(point);
      FOR_START other_x_start[i] += other_x_step[i]; FOR_END
    }
    xstart_01 -= k_01_inv;
    xstart_02 -= k_02_inv;

    FOR_START other_01y_start[i] += other_01y_step[i]; other_02y_start[i] += other_02y_step[i]; FOR_END
  }

  //delete[] other_01y_start;
  //delete[] other_02y_start;
  //delete[] other_01y_step;
  //delete[] other_02y_step;
  //delete[] other_x_start;
  //delete[] other_x_step;
}

//    down
// v1      v2
//
//    v0
void Raster::rasteriseDownTriangle( float x0, float y0, float x1, float y1, float x2, float y2, 
                                    int other_count, float *others0, float *others1, float *others2, 
                                    OutPointPackage* out_point_package )
{
  float k_10_inv = (x0 - x1) / (y0 - y1);
  float k_20_inv = (x0 - x2) / (y0 - y2);
  float xstart_10 = x1, xstart_20 = x2;

  float other_10y_start[10];
  float other_20y_start[10];
  float other_10y_step[10];
  float other_20y_step[10];
  float other_x_start[10];
  float other_x_step[10];

  memcpy(other_10y_start, others1, sizeof(float) * other_count);
  memcpy(other_20y_start, others2, sizeof(float) * other_count);
  FOR_START
  other_10y_step[i] = (others0[i] - others1[i]) / (int(y1) - int(y0));
  other_20y_step[i] = (others0[i] - others2[i]) / (int(y1) - int(y0));
  FOR_END

  OutPoint point;
  for(int y = y1; y >= int(y0); --y)
  {
    memcpy(other_x_start, other_10y_start, sizeof(float) * other_count);
    FOR_START other_x_step[i] = (other_20y_start[i] - other_10y_start[i]) / (int(xstart_20) - int(xstart_10)); FOR_END
    for(int x = int(xstart_10); x <= int(xstart_20); ++x)
    {
      point.vertex.x= x; point.vertex.y = y; point.vertex.z = other_x_start[0];
      memcpy(&point.color, other_x_start + 1, sizeof(float) * 4);
      memcpy(&point.texcoord, other_x_start + 5, sizeof(float) * 2);
      out_point_package->AddPoint(point);
      FOR_START other_x_start[i] += other_x_step[i]; FOR_END
    }
    xstart_10 -= k_10_inv;
    xstart_20 -= k_20_inv;

    FOR_START other_10y_start[i] += other_10y_step[i]; other_20y_start[i] += other_20y_step[i]; FOR_END
  }

  //delete[] other_10y_start;
  //delete[] other_20y_start;
  //delete[] other_10y_step;
  //delete[] other_20y_step;
  //delete[] other_x_start;
  //delete[] other_x_step;
}

int Raster::CalTriangleFitAreaPixelByPoint( float x0, float y0, float x1, float y1, float x2, float y2 )
{
  POINT2D p0, p1, p2, pt;
  p0.x = x0; p0.y = y0;
  p1.x = x1; p1.y = y1;
  p2.x = x2; p2.y = y2;
  VECTOR2D_Sub(&p1, &p0, &pt);
  float a = VECTOR2D_Lenght_Fast(&pt);
  VECTOR2D_Sub(&p2, &p0, &pt);
  float b = VECTOR2D_Lenght_Fast(&pt);
  VECTOR2D_Sub(&p1, &p2, &pt);
  float c = VECTOR2D_Lenght_Fast(&pt);
  float p = (a + b + c) / 2.0f;
  float area = Fast_Sqrt(p * (p - a) * (p - b) * (p - c));
  return int(area + a + b + c + 4);
}

}



