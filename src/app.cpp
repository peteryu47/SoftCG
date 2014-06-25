#include "app.h"

#include <assert.h>
#include <GLEW/glew.h>

#include "math/math_3d.h"
#include "render/render.h"
#include "render/frame_buffer.h"

App::App(int width, int height)
	:	m_iWindowWidht(width),
		m_iWindowHeight(height)
{
	m_pRender = new render::Render();
	m_pRender->Init();
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
	glDrawPixels(600, 600, GL_RGB, GL_UNSIGNED_BYTE, 
		m_pRender->GetCurSceneFrameBuffer()->GetFrameBufferData());
}