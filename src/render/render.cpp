#include "render/render.h"

#include <Windows.h>

#include "render/frame_buffer.h"
#include "render/draw_utils.h"

#include "utils/defines.h"
#include "utils/random_utils.h"

namespace render
{

render::Render::Render()
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

void Render::DrawFrame()
{

  LARGE_INTEGER m_nFreq;
  LARGE_INTEGER m_nBeginTime;
  LARGE_INTEGER nEndTime;
  QueryPerformanceFrequency(&m_nFreq);
  QueryPerformanceCounter(&m_nBeginTime);

  memset(m_pSceneFrameBuffer->GetFrameBufferData(), 255, sizeof(uchar) * m_pSceneFrameBuffer->GetTotalByteCount());
  int left = 100, top = 100, right = 500, bottom = 500;
  int x0, y0, x1, y1;
  int new_x0, new_y0, new_x1, new_y1;
  for(int i = 0; i < 10000; ++i)
  {
    x0 = RandInt(m_iViewPortWidth);
    y0 = RandInt(m_iViewPortWidth);
    x1 = RandInt(m_iViewPortWidth);
    y1 = RandInt(m_iViewPortWidth);
    if(ClipLine2DWithRect(x0, y0, x1, y1, left, top, right, bottom, &new_x0, &new_y0, &new_x1, &new_y1))
    {
      DrawLineOnFrameBuffer(m_pSceneFrameBuffer, 
        new_x0, new_y0, new_x1, new_y1, 
        RandInt(255), RandInt(255), RandInt(255));
    }
  }

  QueryPerformanceCounter(&nEndTime);
  LONGLONG ms_time = (nEndTime.QuadPart - m_nBeginTime.QuadPart) * 1000 / m_nFreq.QuadPart;
  m_fFPS = 1000.0f / ms_time;
}

void Render::SetModelViewMat( MATRIX4X4_PTR mvMat )
{
	MAT_COPY_4X4(&m_matModelView, mvMat);
}

void Render::SetProjectionMat( MATRIX4X4_PTR pMat )
{
	MAT_COPY_4X4(&m_matProjection, pMat);
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
}

void Render::initFrameBuffers()
{
	m_pSceneFrameBuffer = new FrameBuffer(m_iViewPortWidth, m_iViewPortHeight, kFrameFormatRGB);
	m_pZFrameBuffer = new FrameBuffer(m_iViewPortWidth, m_iViewPortHeight, kFrameFormatA);
}

void Render::cleanFrameBuffers()
{
  SAFE_DELETE(m_pSceneFrameBuffer);
	SAFE_DELETE(m_pZFrameBuffer);
}

}