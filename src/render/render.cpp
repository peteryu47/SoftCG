#include "render/render.h"

#include "render/frame_buffer.h"
#include "utils/defines.h"

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

	for(int w = 0; w < m_pSceneFrameBuffer->GetWidth(); ++w)
	{
		for(int h = 0; h < m_pSceneFrameBuffer->GetHeight(); ++h)
		{
			if(w > h)
				m_pSceneFrameBuffer->SetBufferDataRGB(w, h, 0, 0, 0);
			else
				m_pSceneFrameBuffer->SetBufferDataRGB(w, h, 255, 255, 255);
		}
	}
}

void Render::Clean()
{
	cleanFrameBuffers();
}

void Render::DrawFrame()
{

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