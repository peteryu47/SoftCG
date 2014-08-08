#include "app.h"

#include <assert.h>
#include <Windows.h>
#include <GLEW/glew.h>

#include "math/math_3d.h"
#include "math/camera.h"
#include "render/render.h"
#include "render/frame_buffer.h"
#include "render/vertex_data_cache.h"

using namespace render;

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

const float v_data[3][3] = 
{
  {300, 100},
  {200, 200},
  {400, 400}
};

const int i_data[3] = 
{
  0, 1, 2
};

App::App(int width, int height)
	:	m_iWindowWidht(width),
		m_iWindowHeight(height)
{
	m_pRender = new render::Render();
	m_pRender->SetViewPortSize(m_iWindowWidht, m_iWindowHeight);
  m_pCamera = new Camera;
  m_pCamera->ResetViewMat();
  m_pCamera->LookAt(2, 2, 2, 0, 1, 0, 0, 0, 0);
  m_pCamera->ResetProjMat();
  m_pCamera->PerspectiveProj(90, 1, 1.414, 10);
  m_pRender->SetViewMat(m_pCamera->GetViewMat());
  m_pRender->SetProjMat(m_pCamera->GetProjMat());
  int vex_buffer = VertexDataCache::GetInstance()->GenVertexDataBuffer();
  VertexDataCache::GetInstance()->BindVertexDataBufferData
    (vex_buffer, kDataTypeFloat, 3, 3, (void*)v_data);
  int index_buffer = VertexDataCache::GetInstance()->GenVertexDataBuffer();
  VertexDataCache::GetInstance()->BindVertexDataBufferData
    (index_buffer, kDataTypeInt, 1, 3, (void*)i_data);
  m_pRender->SetVexVexDataBuffer(vex_buffer);
  m_pRender->SetVexIndexDataBuffer(index_buffer);
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

  m_pRender->DrawTriangle(1);
	glDrawPixels(600, 600, GL_RGB, GL_UNSIGNED_BYTE, 
		m_pRender->GetCurSceneFrameBuffer()->GetFrameBufferData());

  QueryPerformanceCounter(&nEndTime);
  LONGLONG ms_time = (nEndTime.QuadPart - m_nBeginTime.QuadPart) * 1000.0f / m_nFreq.QuadPart;
  m_fFrameTime = ms_time / 1000.0f;
}

float App::GetFPS()
{
  return (1.0 / m_fFrameTime);
}
