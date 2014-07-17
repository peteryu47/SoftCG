#ifndef __DRAW_UTILS_H__
#define __DRAW_UTILS_H__

#include <assert.h>

#include <stdio.h>
#include "utils/defines.h"
#include "utils/macro_utils.h"
#include "render/frame_buffer.h"

namespace render
{

void  DrawLineOnFrameBuffer(FrameBuffer* rgb_buffer, int x0, int y0, int x1, int y1, int r, int g, int b)
{
  int x, y, dx, dy, xstep, ystep;
  x = x0;
  y = y0;
  dx = x1 - x0;
  dy = y1 - y0;

  // left to right
  if(dx >= 0)
  {
    xstep = 1;
  }
  // right to left
  else
  {
    xstep = -1;
    dx = -dx;
  }

  // top to down
  if(dy >= 0)
  {
    ystep = 1;
  }
  // down to top
  else
  {
    ystep = -1;
    dy = -dy;
  }

  // near x axis
  if(dx > dy)
  {
    float m = (float)(dy) / (float)(dx);
    float acc_y = 0;

    for(int index = 0; index <= dx; index += 1)
    {
      rgb_buffer->SetBufferDataRGB(x, y, r, g, b);

      x += xstep;
      acc_y += m;
      if(acc_y > 1)
      {
        y += ystep;
        acc_y -= 1;
      }
    }
  }
  else
  {
    float m_inv = (float)(dx) / (float)(dy);
    float acc_x = 0;

    for(int index = 0; index <= dy; index += 1)
    {
      rgb_buffer->SetBufferDataRGB(x, y, r, g, b);

      y += ystep;
      acc_x += m_inv;
      if(acc_x > 1)
      {
        x += xstep;
        acc_x -= 1;
      }
    }
  }
}

#define C_C 0x00
#define C_W 0x01
#define C_E 0x02
#define C_S 0x04
#define C_N 0x08
#define C_NW 0x09
#define C_NE 0x0a
#define C_SW 0x05
#define C_SE 0x06

/*
            |             |
 NW 9 1001  |   N 8 1000  | NE 10 1100        ---------------->x
--------------------------------------        |
      W     |             |     E             |          right_top
      1     |   C 0 0000  |     2             |       -------
     0001   |             |    0010           |       |     |
--------------------------------------        |       |     |
 SW 5 0101  |   S 4 0100  | SE 6 0110         |       ------- 
            |             |                   y    left_bottom       
*/

bool ClipLine2DWithRect(int x0, int y0, int x1, int y1, int left, int bottom,
  int right, int top, int *result_x0, int *result_y0, int *result_x1, int *result_y1)
{
  if(x0 == x1 && y0 == y1)
    return false;

  int code0 = 0;
  int code1 = 0;

  if(x0 < left)
    code0 |= C_W;
  if(x0 > right)
    code0 |= C_E;
  if(y0 < bottom)
    code0 |= C_S;
  if(y0 > top)
    code0 |= C_N;

  if(x1 < left)
    code1 |= C_W;
  if(x1 > right)
    code1 |= C_E;
  if(y1 < bottom)
    code1 |= C_S;
  if(y1 > top)
    code1 |= C_N;

  // same side
  if(code0 & code1)
  {
    return false;
  }

  // all in center area, do not need clip
  if(code0 == C_C && code1 == C_C)
  {
    *result_x0 = x0; *result_y0 = y0; *result_x1 = x1; *result_y1 = y1;
    return true;
  }

  float k = float(y1 - y0) / float(x1 - x0);
  float k_inv = 1.0f / k;
  //y = k(x - x0) + y0
  //x= k_inv(y - y0) + x0

  switch(code0)
  {
  case C_C:
      break;

  case C_N:
    {
      *result_x0 = int(k_inv * (top - y0) + x0 + 0.5);
      *result_y0 = top;
      break;
    }

  case C_S:
    {
      *result_x0 = int(k_inv * (bottom - y0) + x0 + 0.5);
      *result_y0 = bottom;
      break;
    }

  case C_W:
    {
      *result_x0 = left;
      *result_y0 = int(k * (left - x0) + y0 + 0.5);
      break;
    }

  case C_E:
    {
      *result_x0 = right;
      *result_y0 = int(k * (right - x0) + y0 + 0.5);
      break;
    }

  case C_NW:
    {
      *result_x0 = int(k_inv * (top - y0) + x0 + 0.5);
      *result_y0 = top;
      if(*result_x0 < left)
      {
        *result_x0 = left;
        *result_y0 = int(k * (left - x0) + y0 + 0.5);
      }
      break;
    }

  case C_NE:
    {
      *result_x0 = int(k_inv * (top - y0) + x0 + 0.5);
      *result_y0 = top;
      if(*result_x0 > right)
      {
        *result_x0 = right;
        *result_y0 = int(k * (right - x0) + y0 + 0.5);
      }
      break;
    }

  case C_SW:
    {
      *result_x0 = int(k_inv * (bottom - y0) + x0 + 0.5);
      *result_y0 = top;
      if(*result_x0 < left)
      {
        *result_x0 = left;
        *result_y0 = int(k * (left - x0) + y0 + 0.5);
      }
      break;
    }

  case C_SE:
    {
      *result_x0 = int(k_inv * (bottom - y0) + x0 + 0.5);
      *result_y0 = top;
      if(*result_x0 > right)
      {
        *result_x0 = right;
        *result_y0 = int(k * (right - x0) + y0 + 0.5);
      }
      break;
    }  
  }

  if(x0 > right || x0 < left || y0 < top || y0 > bottom || x1 > right || x1 < left || y1 < top || y1 > bottom)
  {
    return false;
  }
  return true;
}

#define C_SWAP(x, y, t)   t = x; x = y; y = t

void  DrawTriangleOnFrameBuffer(FrameBuffer* rgb_buffer, 
  float x0, float y0, float x1, float y1, float x2, float y2,
  int r0, int g0, int b0, int r1, int g1, int b1, int r2, int g2, int b2)
{
  float t = 0;
  float t_x0, t_y0, t_x1, t_y1, t_x2, t_y2;
  int   t_r0, t_g0, t_b0, t_r1, t_g1, t_b1, t_r2, t_g2, t_b2;
  bool  need_raster_twice = false;
  // make v1 is the bottom vertex
  if(y1 > y0)
  {
    C_SWAP(x1, x0, t);  C_SWAP(y1, y0, t);
    C_SWAP(r1, r0, t);  C_SWAP(g1, g0, t);  C_SWAP(b1, b0, t);  
  }
  if(y1 > y2)
  {
    C_SWAP(x1, x2, t);  C_SWAP(y1, y2, t);
    C_SWAP(r1, r2, t);  C_SWAP(g1, g2, t);  C_SWAP(b1, b2, t);  
  }

  //  down
  //  v0  v2
  //    v1
  if(y0 == y2)
  {
    // v2   v0
    //    v1
    if(x2 < x0)
    {
      C_SWAP(x2, x0, t);  C_SWAP(y2, y0, t);
      C_SWAP(r2, r0, t);  C_SWAP(g2, g0, t);  C_SWAP(b2, b0, t);  
    }
    goto RasterizationDownTriangle;
  }
  else
  {
    //   up
    //   v0 
    // v1  v2
    if(y1 == y2)
    {
      //   v2 
      // v1  v0
      if(x1 > x2)
      {
        C_SWAP(x1, x2, t);  C_SWAP(y1, y2, t);
        C_SWAP(r1, r2, t);  C_SWAP(g1, g2, t);  C_SWAP(b1, b2, t);  
      }
      if(y2 > y0)
      {
        C_SWAP(x2, x0, t);  C_SWAP(y2, y0, t);
        C_SWAP(r2, r0, t);  C_SWAP(g2, g0, t);  C_SWAP(b2, b0, t);  
      }

      goto RasterizationUpTriangle;
    }
    else
    {
      if(y0 < y2)
      {
        C_SWAP(x0, x2, t);  C_SWAP(y0, y2, t);
        C_SWAP(r0, r2, t);  C_SWAP(g0, g2, t);  C_SWAP(b0, b2, t);  
      }

      //    up
      //    v0            v0
      //
      //        v2      v1    v2     
      //  v1            v0    v2
      //  
      //               v1
      if(x0 < x2)
      {
        float x = (float(x1 - x0) / float(y1 - y0) * float(y2 - y0) + x0);
        float len_factor = float(y2 - y0) / float(y1 - y0);

        t_x0 = x; t_y0 = y2; t_x1 = x1; t_y1 = y1; t_x2 = x2; t_y2 = y2;  // down
        x1 = x; y1 = y2;  //up

        t_r0 = (r1 - r0) * len_factor + r0;
        t_g0 = (g1 - g0) * len_factor + g0;
        t_b0 = (b1 - b0) * len_factor + b0;
        t_r1 = r1; t_g1 = g1; t_b1 = b1;
        t_r2 = r2; t_g2 = g2; t_b2 = b2;

        r1 = t_r0; g1 = t_g0; b1 = t_b0;
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
        float x = (float(x1 - x0) / float(y1 - y0) * float(y2 - y0) + x0);
        float len_factor = float(y2 - y0) / float(y1 - y0);
        t_x0 = x2; t_y0 = y2; t_x1 = x1; t_y1 = y1; t_x2 = x; t_y2 = y2;  //down
        x1 = x2; y1 = y2; x2 = x; y2 = y2;  //up

        t_r0 = r2; t_g0 = g2; t_b0 = b2;
        t_r1 = r1; t_g1 = g1; t_b1 = b1;
        t_r2 = (r1 - r0) * len_factor + r0;
        t_g2 = (g1 - g0) * len_factor + g0;
        t_b2 = (b1 - b0) * len_factor + b0;

        r1 = r2; g1 = g2; b1 = b2;
        r2 = t_r2; g2 = t_g2; b2 = t_b2;
      }
      
    }
  }

  need_raster_twice = true;
  goto RasterizationUpTriangle;

RasterizationTwice:
  x0 = t_x0; y0 = t_y0; x1 = t_x1; y1 = t_y1; x2 = t_x2; y2 = t_y2; 
  r0 = t_r0; g0 = t_g0; b0 = t_b0;
  r1 = t_r1; g1 = t_g1; b1 = t_b1;
  r2 = t_r2; g2 = t_g2; b2 = t_b2;
  goto RasterizationDownTriangle;

//   v0 
// v1  v2
RasterizationUpTriangle:
  {
    float k_10_inv = float(x0 - x1) / float(y0 - y1);
    float k_20_inv = float(x0 - x2) / float(y0 - y2);
    float xstart_10 = x1, xstart_20 = x2;
    int   xstart_20_int = int(xstart_20);

    float step_10_r = float(r0 - r1) / float(y0 - y1);
    float step_10_g = float(g0 - g1) / float(y0 - y1);
    float step_10_b = float(b0 - b1) / float(y0 - y1);
    float rstart_10 = r1, gstart_10 = g1, bstart_10 = b1;

    float step_20_r = float(r0 - r2) / float(y0 - y1);
    float step_20_g = float(g0 - g2) / float(y0 - y1);
    float step_20_b = float(b0 - b2) / float(y0 - y1);
    float rstart_20 = r2, gstart_20 = g2, bstart_20 = b2;

    float temp_rstart, temp_gstart, temp_bstart;
    float temp_step_r, temp_step_g, temp_step_b;
    float x_20_10_inv;

    for(int y = y1; y <= y0; ++y)
    {
      x_20_10_inv =  1.0f / (xstart_20 - xstart_10 + 1);
      temp_step_r = (rstart_20 - rstart_10) * x_20_10_inv;
      temp_step_g = (gstart_20 - gstart_10) * x_20_10_inv;
      temp_step_b = (bstart_20 - bstart_10) * x_20_10_inv;
      temp_rstart = rstart_10;
      temp_gstart = gstart_10;
      temp_bstart = bstart_10;
      
      for(int x = int(xstart_10); x <= xstart_20_int; ++x)
      {
        rgb_buffer->SetBufferDataRGB(int(x + 0.5), int(y + 0.5), int(temp_rstart), int(temp_gstart), int(temp_bstart));

        temp_rstart += temp_step_r;
        temp_gstart += temp_step_g;
        temp_bstart += temp_step_b;
      }
      xstart_10 += k_10_inv;
      xstart_20 += k_20_inv;
      xstart_20_int = int(xstart_20);

      rstart_10 += step_10_r;
      gstart_10 += step_10_g;
      bstart_10 += step_10_b;

      rstart_20 += step_20_r;
      gstart_20 += step_20_g;
      bstart_20 += step_20_b;
    }
    if(need_raster_twice)
      goto RasterizationTwice;
    else
      return;
  }

//  v0  v2
//    v1
RasterizationDownTriangle:
  {
    float k_10_inv = float(x0 - x1) / float(y0 - y1);
    float k_12_inv = float(x2 - x1) / float(y2 - y1);
    float xstart_10 = x1, xstart_12 = x1;
    int   xstart_12_int = int(xstart_12);

    float step_10_r = float(r0 - r1) / float(y0 - y1);
    float step_10_g = float(g0 - g1) / float(y0 - y1);
    float step_10_b = float(b0 - b1) / float(y0 - y1);
    float rstart_10 = r1, gstart_10 = g1, bstart_10 = b1;

    float step_12_r = float(r2 - r1) / float(y0 - y1);
    float step_12_g = float(g2 - g1) / float(y0 - y1);
    float step_12_b = float(b2 - b1) / float(y0 - y1);
    float rstart_12 = r1, gstart_12 = g1, bstart_12 = b1;

    float temp_rstart, temp_gstart, temp_bstart;
    float temp_step_r, temp_step_g, temp_step_b;
    float x_12_10_inv;

    for(int y = y1; y <= y0; ++y)
    {
      
      x_12_10_inv = 1.0f / (xstart_12 - xstart_10 + 1);
      temp_step_r = (rstart_12 - rstart_10) * x_12_10_inv;
      temp_step_g = (gstart_12 - gstart_10) * x_12_10_inv;
      temp_step_b = (bstart_12 - bstart_10) * x_12_10_inv;
      temp_rstart = rstart_10;
      temp_gstart = gstart_10;
      temp_bstart = bstart_10;
      for(int x = int(xstart_10); x <= xstart_12_int; ++x)
      {   
        rgb_buffer->SetBufferDataRGB(int(x + 0.5), int(y + 0.5), int(temp_rstart), int(temp_gstart), int(temp_bstart));

        temp_rstart += temp_step_r;
        temp_gstart += temp_step_g;
        temp_bstart += temp_step_b;
      }
      xstart_10 += k_10_inv;
      xstart_12 += k_12_inv;
      xstart_12_int = int(xstart_12);

      rstart_10 += step_10_r;
      gstart_10 += step_10_g;
      bstart_10 += step_10_b;

      rstart_12 += step_12_r;
      gstart_12 += step_12_g;
      bstart_12 += step_12_b;
    }
    return;
  }

}

void  DrawTriangleOnFrameBuffer(FrameBuffer* rgb_buffer, int x0, int y0, int x1, int y1, int x2, int y2,
  int r, int g, int b)
{
  DrawTriangleOnFrameBuffer(rgb_buffer, x0, y0, x1, y1, x2, y2, r, g, b, r, g, b, r, g, b);
}

void  DrawTriangleOnFrameBufferWithZBuffer(FrameBuffer* rgb_buffer,  FrameBuffer* z_buffer,
  float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2,
  int r0, int g0, int b0, int r1, int g1, int b1, int r2, int g2, int b2)
{
  float t = 0;
  float t_x0, t_y0, t_z0, t_x1, t_y1, t_z1, t_x2, t_y2, t_z2;
  int   t_r0, t_g0, t_b0, t_r1, t_g1, t_b1, t_r2, t_g2, t_b2;
  bool  need_raster_twice = false;
  // make v1 is the bottom vertex
  if(y1 > y0)
  {
    C_SWAP(x1, x0, t);  C_SWAP(y1, y0, t);  C_SWAP(z1, z0, t);
    C_SWAP(r1, r0, t);  C_SWAP(g1, g0, t);  C_SWAP(b1, b0, t);  
  }
  if(y1 > y2)
  {
    C_SWAP(x1, x2, t);  C_SWAP(y1, y2, t);  C_SWAP(z1, z2, t);
    C_SWAP(r1, r2, t);  C_SWAP(g1, g2, t);  C_SWAP(b1, b2, t);  
  }

  //  down
  //  v0  v2
  //    v1
  if(y0 == y2)
  {
    // v2   v0
    //    v1
    if(x2 < x0)
    {
      C_SWAP(x2, x0, t);  C_SWAP(y2, y0, t);  C_SWAP(z2, z0, t);
      C_SWAP(r2, r0, t);  C_SWAP(g2, g0, t);  C_SWAP(b2, b0, t);  
    }
    goto RasterizationDownTriangle;
  }
  else
  {
    //   up
    //   v0 
    // v1  v2
    if(y1 == y2)
    {
      //   v2 
      // v1  v0
      if(x1 > x2)
      {
        C_SWAP(x1, x2, t);  C_SWAP(y1, y2, t);  C_SWAP(z1, z2, t);
        C_SWAP(r1, r2, t);  C_SWAP(g1, g2, t);  C_SWAP(b1, b2, t);  
      }
      if(y2 > y0)
      {
        C_SWAP(x2, x0, t);  C_SWAP(y2, y0, t);  C_SWAP(z2, z0, t);
        C_SWAP(r2, r0, t);  C_SWAP(g2, g0, t);  C_SWAP(b2, b0, t);  
      }

      goto RasterizationUpTriangle;
    }
    else
    {
      if(y0 < y2)
      {
        C_SWAP(x0, x2, t);  C_SWAP(y0, y2, t);  C_SWAP(z0, z2, t);
        C_SWAP(r0, r2, t);  C_SWAP(g0, g2, t);  C_SWAP(b0, b2, t);  
      }

      //    up
      //    v0            v0
      //
      //        v2      v1    v2     
      //  v1            v0    v2
      //  
      //               v1
      if(x0 < x2)
      {
        float x = (float(x1 - x0) / float(y1 - y0) * float(y2 - y0) + x0);
        float len_factor = float(y2 - y0) / float(y1 - y0);

        t_x0 = x; t_y0 = y2; t_z0 = (z1 - z0) * len_factor + z0;
        t_x1 = x1; t_y1 = y1; t_z1 = z1;
        t_x2 = x2; t_y2 = y2; t_z2 = z2;  // down
        x1 = x; y1 = y2; z1 = t_z0;       //up

        t_r0 = (r1 - r0) * len_factor + r0;
        t_g0 = (g1 - g0) * len_factor + g0;
        t_b0 = (b1 - b0) * len_factor + b0;
        t_r1 = r1; t_g1 = g1; t_b1 = b1;
        t_r2 = r2; t_g2 = g2; t_b2 = b2;

        r1 = t_r0; g1 = t_g0; b1 = t_b0;
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
        float x = (float(x1 - x0) / float(y1 - y0) * float(y2 - y0) + x0);
        float len_factor = float(y2 - y0) / float(y1 - y0);
        t_x0 = x2; t_y0 = y2; t_z0 = (z1 - z0) * len_factor + z0;
        t_x1 = x1; t_y1 = y1; t_z1 = z1;
        t_x2 = x; t_y2 = y2; t_z2 = z2; //down
        x1 = x2; y1 = y2; z1 = z2; 
        x2 = x; y2 = y2; z2 = t_z0; //up

        t_r0 = r2; t_g0 = g2; t_b0 = b2;
        t_r1 = r1; t_g1 = g1; t_b1 = b1;
        t_r2 = (r1 - r0) * len_factor + r0;
        t_g2 = (g1 - g0) * len_factor + g0;
        t_b2 = (b1 - b0) * len_factor + b0;

        r1 = r2; g1 = g2; b1 = b2;
        r2 = t_r2; g2 = t_g2; b2 = t_b2;
      }

    }
  }

  need_raster_twice = true;
  goto RasterizationUpTriangle;

RasterizationTwice:
  x0 = t_x0; y0 = t_y0; z0 = t_z0; 
  x1 = t_x1; y1 = t_y1; z1 = t_z1;
  x2 = t_x2; y2 = t_y2; z2 = t_z2;
  r0 = t_r0; g0 = t_g0; b0 = t_b0;
  r1 = t_r1; g1 = t_g1; b1 = t_b1;
  r2 = t_r2; g2 = t_g2; b2 = t_b2;
  goto RasterizationDownTriangle;

  //   v0 
  // v1  v2
RasterizationUpTriangle:
  {
    float k_10_inv = float(x0 - x1) / float(int(y0) - int(y1));
    float k_20_inv = float(x0 - x2) / float(int(y0) - int(y2));
    float xstart_10 = x1, xstart_20 = x2;
    int   xstart_20_int = int(xstart_20);

    float zstart_10 = z1, zstart_20 = z2;
    float step_10_z = (z0 - z1) / (int(y0) - int(y1));
    float step_20_z = (z0 - z2) / (int(y0) - int(y1));

    float step_10_r = float(r0 - r1) / float(int(y0) - int(y1));
    float step_10_g = float(g0 - g1) / float(int(y0) - int(y1));
    float step_10_b = float(b0 - b1) / float(int(y0) - int(y1));
    float rstart_10 = r1, gstart_10 = g1, bstart_10 = b1;

    float step_20_r = float(r0 - r2) / float(int(y0) - int(y1));
    float step_20_g = float(g0 - g2) / float(int(y0) - int(y1));
    float step_20_b = float(b0 - b2) / float(int(y0) - int(y1));
    float rstart_20 = r2, gstart_20 = g2, bstart_20 = b2;

    float temp_rstart, temp_gstart, temp_bstart;
    float temp_step_r, temp_step_g, temp_step_b;
    float x_20_10_inv;

    float temp_zstart, temp_step_z;
    float depth_z;

    for(int y = y1; y <= y0; ++y)
    {
      x_20_10_inv =  1.0f / (int(xstart_20) - int(xstart_10) + 1);
      temp_step_r = (rstart_20 - rstart_10) * x_20_10_inv;
      temp_step_g = (gstart_20 - gstart_10) * x_20_10_inv;
      temp_step_b = (bstart_20 - bstart_10) * x_20_10_inv;
      temp_rstart = rstart_10;
      temp_gstart = gstart_10;
      temp_bstart = bstart_10;

      temp_zstart = zstart_10;
      temp_step_z = (zstart_20 - zstart_10) * x_20_10_inv;

      for(int x = int(xstart_10); x <= xstart_20_int; ++x)
      {
        z_buffer->GetBufferDataAF(int(x + 0.5), int(y + 0.5), &depth_z);
        if(depth_z < temp_zstart)
        {
          rgb_buffer->SetBufferDataRGB(int(x + 0.5), int(y + 0.5), int(temp_rstart), 
            int(temp_gstart), int(temp_bstart));
          z_buffer->SetBufferDataAF(int(x + 0.5), int(y + 0.5), temp_zstart);
        }

        temp_rstart += temp_step_r;
        temp_gstart += temp_step_g;
        temp_bstart += temp_step_b;
        temp_zstart += temp_step_z;
      }
      xstart_10 += k_10_inv;
      xstart_20 += k_20_inv;
      xstart_20_int = int(xstart_20);

      rstart_10 += step_10_r;
      gstart_10 += step_10_g;
      bstart_10 += step_10_b;

      rstart_20 += step_20_r;
      gstart_20 += step_20_g;
      bstart_20 += step_20_b;

      zstart_10 += step_10_z;
      zstart_20 += step_20_z;
    }
    if(need_raster_twice)
      goto RasterizationTwice;
    else
      return;
  }

  //  v0  v2
  //    v1
RasterizationDownTriangle:
  {
    float k_10_inv = float(x0 - x1) / float(int(y0) - int(y1));
    float k_12_inv = float(x2 - x1) / float(int(y2) - int(y1));
    float xstart_10 = x1, xstart_12 = x1;
    int   xstart_12_int = int(xstart_12);

    float zstart_10 = z1, zstart_12 = z1;
    float step_10_z = (z0 - z1) / (int(y0) - int(y1));
    float step_12_z = (z2 - z1) / (int(y0) - int(y1));

    float step_10_r = float(r0 - r1) / float(int(y0) - int(y1));
    float step_10_g = float(g0 - g1) / float(int(y0) - int(y1));
    float step_10_b = float(b0 - b1) / float(int(y0) - int(y1));
    float rstart_10 = r1, gstart_10 = g1, bstart_10 = b1;

    float step_12_r = float(r2 - r1) / float(int(y0) - int(y1));
    float step_12_g = float(g2 - g1) / float(int(y0) - int(y1));
    float step_12_b = float(b2 - b1) / float(int(y0) - int(y1));
    float rstart_12 = r1, gstart_12 = g1, bstart_12 = b1;

    float temp_rstart, temp_gstart, temp_bstart;
    float temp_step_r, temp_step_g, temp_step_b;
    float x_12_10_inv;

    float temp_zstart, temp_step_z;
    float depth_z;

    for(int y = y1; y <= y0; ++y)
    {

      x_12_10_inv = 1.0f / (int(xstart_12) - int(xstart_10) + 1);
      temp_step_r = (rstart_12 - rstart_10) * x_12_10_inv;
      temp_step_g = (gstart_12 - gstart_10) * x_12_10_inv;
      temp_step_b = (bstart_12 - bstart_10) * x_12_10_inv;
      temp_rstart = rstart_10;
      temp_gstart = gstart_10;
      temp_bstart = bstart_10;

      temp_zstart = zstart_10;
      temp_step_z = (zstart_12 - zstart_10) * x_12_10_inv;

      for(int x = int(xstart_10); x <= xstart_12_int; ++x)
      {   
        z_buffer->GetBufferDataAF(int(x + 0.5), int(y + 0.5), &depth_z);
        if(depth_z < temp_zstart)
        {
          rgb_buffer->SetBufferDataRGB(int(x + 0.5), int(y + 0.5), int(temp_rstart), 
            int(temp_gstart), int(temp_bstart));
          z_buffer->SetBufferDataAF(int(x + 0.5), int(y + 0.5), temp_zstart);
        }
        
        temp_rstart += temp_step_r;
        temp_gstart += temp_step_g;
        temp_bstart += temp_step_b;
        temp_zstart += temp_step_z;
      }
      xstart_10 += k_10_inv;
      xstart_12 += k_12_inv;
      xstart_12_int = int(xstart_12);

      rstart_10 += step_10_r;
      gstart_10 += step_10_g;
      bstart_10 += step_10_b;

      rstart_12 += step_12_r;
      gstart_12 += step_12_g;
      bstart_12 += step_12_b;

      zstart_10 += step_10_z;
      zstart_12 += step_12_z;
    }
    return;
  }
}

}//end of namespace render

#endif//end of define