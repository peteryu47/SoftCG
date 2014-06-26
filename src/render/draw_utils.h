#ifndef __DRAW_UTILS_H__
#define __DRAW_UTILS_H__

#include <assert.h>

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

void  DrawTriangleOnFrameBuffer(FrameBuffer* rgb_buffer, int x0, int y0, int x1, int y1, int x2, int y2,
  int r0, int g0, int b0, int r1, int g1, int b1, int r2, int g2, int b2)
{

}

void  DrawTriangleOnFrameBuffer(FrameBuffer* rgb_buffer, int x0, int y0, int x1, int y1, int x2, int y2,
  int r, int g, int b)
{
  DrawTriangleOnFrameBuffer(rgb_buffer, x0, y0, x1, y1, x2, y2, r, g, b, r, g, b, r, g, b);
}

}

#endif