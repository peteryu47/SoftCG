#include "app.h"

#include <assert.h>
#include <Windows.h>
#include <GLEW/glew.h>

#include "math/math_3d.h"
#include "render/render.h"
#include "render/frame_buffer.h"

App::App(int width, int height)
	:	m_iWindowWidht(width),
		m_iWindowHeight(height)
{
	m_pRender = new render::Render();
	m_pRender->SetViewPortSize(m_iWindowWidht, m_iWindowHeight);
}

App::~App()
{
	SAFE_DELETE(m_pRender);
}

void App::Run()
{

}

void App::Update(float delta)
{
  LARGE_INTEGER m_nFreq;
  LARGE_INTEGER m_nBeginTime;
  LARGE_INTEGER nEndTime;
  QueryPerformanceFrequency(&m_nFreq);
  QueryPerformanceCounter(&m_nBeginTime);

  m_pRender->DrawFrame();
	glDrawPixels(600, 600, GL_RGB, GL_UNSIGNED_BYTE, 
		m_pRender->GetCurSceneFrameBuffer()->GetFrameBufferData());

  QueryPerformanceCounter(&nEndTime);
  LONGLONG ms_time = (nEndTime.QuadPart - m_nBeginTime.QuadPart) * 1000 / m_nFreq.QuadPart;
}

float App::GetFPS()
{
  return 1000.0f / (1000.0f / m_pRender->GetFPS() + 9);
}
