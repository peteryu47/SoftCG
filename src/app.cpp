#include "app.h"

#include <assert.h>
#include <GLEW\glew.h>
#include "math/math_3d.h"

#include "frame_buffer.h"

App::App(int width, int height)
	:	m_iWindowWidht(width),
		m_iWindowHeight(height)
{
	m_pFrameBuffer = new FrameBuffer(width, height, kFrameFormatRGB);

	for(int w = 0; w < m_iWindowWidht; ++w)
	{
		for(int h = 0; h < m_iWindowHeight; ++h)
		{
			if(w > h)
				m_pFrameBuffer->SetBufferDataRGB(w, h, 0, 0, 0);
			else
				m_pFrameBuffer->SetBufferDataRGB(w, h, 255, 255, 255);
		}
	}
}

App::~App()
{
	SAFE_DELETE(m_pFrameBuffer);
}

void App::Run()
{

}

void App::Update(float delta)
{
	glDrawPixels(600, 600, GL_RGB, GL_UNSIGNED_BYTE, m_pFrameBuffer->GetFrameBufferData());
}