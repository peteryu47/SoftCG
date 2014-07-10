#include "render/render.h"

#include <Windows.h>

#include "render/frame_buffer.h"
#include "render/draw_utils.h"

#include "utils/defines.h"
#include "utils/random_utils.h"

namespace render
{

Render::Render()
	:	m_iViewPortWidth(WINDOW_WIDTH),
		m_iViewPortHeight(WINDOW_HEIGHT)
{
	Init();
}

Render::~Render()
{
	Clean();
}

void Render::Init()
{
	initFrameBuffers();
}

void Render::Clean()
{
	cleanFrameBuffers();
}

const float vertexs[8][3] = {
  { 1,  1,  1},   //0
  {-1,  1,  1},   //1
  {-1,  1, -1},   //2
  { 1,  1, -1},   //3
  { 1, -1,  1},   //4
  {-1, -1,  1},   //5
  {-1, -1, -1},   //6
  { 1, -1, -1}    //7
};

const float colors[8][3] = {
  {255,   0,   0},   //0
  {  0, 255,   0},   //1
  {  0,   0, 255},   //2
  {255, 255, 255},   //3
  {  0,   0,   0},   //4
  {255, 255,   0},   //5
  {255,   0, 255},   //6
  {  0, 255, 255}    //7
};

const int indexes[12][3] = 
{
  {0, 1, 2},
  {0, 2, 3},  //up
  {4, 7, 6},
  {4, 6, 5},  //down
  {3, 2, 6},
  {3, 6, 7},  //font
  {0, 4, 5},
  {0, 5, 1},  //back
  {0, 3, 7},
  {0, 7, 4},  //right
  {2, 1, 5},
  {2, 5, 6}   //left
};

void Render::DrawFrame()
{

  LARGE_INTEGER m_nFreq;
  LARGE_INTEGER m_nBeginTime;
  LARGE_INTEGER nEndTime;
  QueryPerformanceFrequency(&m_nFreq);
  QueryPerformanceCounter(&m_nBeginTime);

  memset(m_pSceneFrameBuffer->GetFrameBufferData(), 255, sizeof(uchar) * m_pSceneFrameBuffer->GetTotalByteCount());
  memset(m_pZFrameBuffer->GetFrameBufferDataFloat(), 0, sizeof(float) * m_pZFrameBuffer->GetTotalByteCount());
  /*
  int left = 100, bottom = 100, right = 300, top = 300;
  int x0, y0, x1, y1;
  int new_x0, new_y0, new_x1, new_y1;
  for(int i = 0; i < 100; ++i)
  {
    x0 = RandInt(m_iViewPortWidth);
    y0 = RandInt(m_iViewPortWidth);
    x1 = RandInt(m_iViewPortWidth);
    y1 = RandInt(m_iViewPortWidth);
    
    //if(ClipLine2DWithRect(x0, y0, x1, y1, left, bottom, right, top, &new_x0, &new_y0, &new_x1, &new_y1))
    //{
    //  DrawLineOnFrameBuffer(m_pSceneFrameBuffer, 
    //    new_x0, new_y0, new_x1, new_y1, 
    //    RandInt(255), RandInt(255), RandInt(255));
    //}
    
    //DrawTriangleOnFrameBuffer(m_pSceneFrameBuffer, 100, 100, 200, 200, 250, 50, 255, 0, 0, 0, 255, 0, 0, 0, 255);
    //DrawLineOnFrameBuffer(m_pSceneFrameBuffer, 100, 100, 200, 150, 0, 0, 0);
    //m_pSceneFrameBuffer->SetBufferDataRGB(100, 200, 0, 0, 0);
  }
  */

  VECTOR4D p0, p1, p2;
  VECTOR4D rp0, rp1, rp2;
  
  for(int i = 0; i < 12; ++i)
  {
    p0.x = vertexs[indexes[i][0]][0]; p0.y = vertexs[indexes[i][0]][1]; p0.z = vertexs[indexes[i][0]][2]; p0.w = 1;
    p1.x = vertexs[indexes[i][1]][0]; p1.y = vertexs[indexes[i][1]][1]; p1.z = vertexs[indexes[i][1]][2]; p1.w = 1;
    p2.x = vertexs[indexes[i][2]][0]; p2.y = vertexs[indexes[i][2]][1]; p2.z = vertexs[indexes[i][2]][2]; p2.w = 1;
  
    Mat_Mul_VECTOR4D_4X4(&p0, m_camera.GetViewProjMat(), &rp0);
    Mat_Mul_VECTOR4D_4X4(&p1, m_camera.GetViewProjMat(), &rp1);
    Mat_Mul_VECTOR4D_4X4(&p2, m_camera.GetViewProjMat(), &rp2);

    rp0.x /= rp0.w; rp0.y /= rp0.w; rp0.z /= rp0.w;
    rp1.x /= rp1.w; rp1.y /= rp1.w; rp1.z /= rp1.w;
    rp2.x /= rp2.w; rp2.y /= rp2.w; rp2.z /= rp2.w;

    rp0.x = (rp0.x + 1) * m_iViewPortWidth / 2.0f; rp0.y = (rp0.y + 1) * m_iViewPortHeight / 2.0f;
    rp1.x = (rp1.x + 1) * m_iViewPortWidth / 2.0f; rp1.y = (rp1.y + 1) * m_iViewPortHeight / 2.0f;
    rp2.x = (rp2.x + 1) * m_iViewPortWidth / 2.0f; rp2.y = (rp2.y + 1) * m_iViewPortHeight / 2.0f;
    //DrawTriangleOnFrameBuffer(m_pSceneFrameBuffer, rp0.x, rp0.y, rp1.x, rp1.y, rp2.x, rp2.y, 
    //  colors[indexes[i][0]][0], colors[indexes[i][0]][1], colors[indexes[i][0]][2],
    //  colors[indexes[i][1]][0], colors[indexes[i][1]][1], colors[indexes[i][1]][2],
    //  colors[indexes[i][2]][0], colors[indexes[i][2]][1], colors[indexes[i][2]][2]);
    DrawTriangleOnFrameBufferWithZBuffer(m_pSceneFrameBuffer, m_pZFrameBuffer,
      rp0.x, rp0.y, rp0.z, rp1.x, rp1.y, rp1.z, rp2.x, rp2.y, rp2.z, 
      colors[indexes[i][0]][0], colors[indexes[i][0]][1], colors[indexes[i][0]][2],
      colors[indexes[i][1]][0], colors[indexes[i][1]][1], colors[indexes[i][1]][2],
      colors[indexes[i][2]][0], colors[indexes[i][2]][1], colors[indexes[i][2]][2]);
  }
  
  /*
  DrawTriangleOnFrameBuffer(m_pSceneFrameBuffer, 
    300, 500, 
    500, 300, 
    300, 100, 
    255, 0, 0,
    255, 255, 255,
    0, 0, 255);
  
  DrawTriangleOnFrameBuffer(m_pSceneFrameBuffer, 
    300, 500, 
    100, 300, 
    300, 100, 
    255, 0, 0,
    0, 255, 0,
    0, 0, 255);
   */
  QueryPerformanceCounter(&nEndTime);
  LONGLONG ms_time = (nEndTime.QuadPart - m_nBeginTime.QuadPart) * 1000 / m_nFreq.QuadPart;
  m_fFPS = 1000.0f / ms_time;
}

void Render::SetViewPortSize( int width, int height )
{
	m_iViewPortWidth = width;
	m_iViewPortHeight = height;

	if(width != m_pSceneFrameBuffer->GetWidth() || height != m_pSceneFrameBuffer->GetHeight())
	{
		cleanFrameBuffers();
		initFrameBuffers();
	}
  m_camera.ResetViewMat();
  m_camera.LookAt(2, 2, 2, 0, 1, 0, 0, 0, 0);
  m_camera.ResetProjMat();
  m_camera.PerspectiveProj(90, 1, 1.414, 10);
}

void Render::initFrameBuffers()
{
	m_pSceneFrameBuffer = new FrameBuffer(m_iViewPortWidth, m_iViewPortHeight, kFrameFormatRGB);
	m_pZFrameBuffer = new FrameBuffer(m_iViewPortWidth, m_iViewPortHeight, kFrameFormatAF);
}

void Render::cleanFrameBuffers()
{
  SAFE_DELETE(m_pSceneFrameBuffer);
	SAFE_DELETE(m_pZFrameBuffer);
}

}