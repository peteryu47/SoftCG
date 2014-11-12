#include "app.h"

#include <assert.h>
#include <Windows.h>
#include <GLEW/glew.h>

#include "math/math_3d.h"
#include "math/camera.h"
#include "render/render.h"
#include "render/frame_buffer.h"
#include "render/vertex_data_cache.h"
#include "render/texture_cache.h"
#include "render/texture.h"

using namespace render;

const float vertexs[8][3] = {
  { 1.0f,  1.0f,  1.0f},   //0
  {-1.0f,  1.0f,  1.0f},   //1
  {-1.0f,  1.0f, -1.0f},   //2
  { 1.0f,  1.0f, -1.0f},   //3
  { 1.0f, -1.0f,  1.0f},   //4
  {-1.0f, -1.0f,  1.0f},   //5
  {-1.0f, -1.0f, -1.0f},   //6
  { 1.0f, -1.0f, -1.0f}    //7
};

const float colors[8][4] = {
  {255,   0,   0,   0},   //0
  {  0, 255,   0,   0},   //1
  {  0,   0, 255,   0},   //2
  {255, 255, 255,   0},   //3
  {  0,   0,   0,   0},   //4
  {255, 255,   0,   0},   //5
  {255,   0, 255,   0},   //6
  {  0, 255, 255,   0}    //7
};

const float texCod[8][2] = 
{
  {1.0f, 1.0f},
  {0.0f, 1.0f},
  {0.0f, 0.0f},
  {1.0f, 0.0f},
  {1.0f, 1.0f},
  {0.0f, 1.0f},
  {0.0f, 0.0f},
  {1.0f, 0.0f},
};

const int indexes[12][3] = 
{
  {3, 2, 6},
  {3, 6, 7},  //font
  {0, 4, 5},
  {0, 5, 1},  //back

  {0, 1, 2},
  {0, 2, 3},  //up
  {4, 7, 6},
  {4, 6, 5},  //down

  {0, 3, 7},
  {0, 7, 4},  //right
  {2, 1, 5},
  {2, 5, 6}   //left
};

const float cube_vertexs[24][3] = {
  { 1.0f,  1.0f, -1.0f},   //3 font   //0
  {-1.0f,  1.0f, -1.0f},   //2        //1
  {-1.0f, -1.0f, -1.0f},   //6        //2
  { 1.0f, -1.0f, -1.0f},   //7        //3

  { 1.0f,  1.0f,  1.0f},   //0 back   //4
  { 1.0f, -1.0f,  1.0f},   //4        //5
  {-1.0f, -1.0f,  1.0f},   //5        //6
  {-1.0f,  1.0f,  1.0f},   //1        //7

  { 1.0f,  1.0f,  1.0f},   //0 up     //8
  {-1.0f,  1.0f,  1.0f},   //1        //9
  {-1.0f,  1.0f, -1.0f},   //2        //10
  { 1.0f,  1.0f, -1.0f},   //3        //11

  { 1.0f, -1.0f,  1.0f},   //4 down   //12
  { 1.0f, -1.0f, -1.0f},   //7        //13
  {-1.0f, -1.0f, -1.0f},   //6        //14
  {-1.0f, -1.0f,  1.0f},   //5        //15

  { 1.0f,  1.0f,  1.0f},   //0 right  //16
  { 1.0f,  1.0f, -1.0f},   //3        //17
  { 1.0f, -1.0f, -1.0f},   //7        //18
  { 1.0f, -1.0f,  1.0f},   //4        //19

  {-1.0f,  1.0f,  1.0f},   //1 left   //20
  {-1.0f, -1.0f,  1.0f},   //5        //21
  {-1.0f, -1.0f, -1.0f},   //6        //22
  {-1.0f,  1.0f, -1.0f},   //2        //23
};

const float cube_texCod[24][2] = 
{
  {1.0f, 1.0f}, //font
  {0.0f, 1.0f},
  {0.0f, 0.0f},
  {1.0f, 0.0f},

  {0.0f, 1.0f}, //back 
  {0.0f, 0.0f},
  {1.0f, 0.0f},
  {1.0f, 1.0f},

  {1.0f, 1.0f}, //up
  {0.0f, 1.0f},
  {0.0f, 0.0f},
  {1.0f, 0.0f},

  {0.0f, 1.0f}, //down 
  {0.0f, 0.0f},
  {1.0f, 0.0f},
  {1.0f, 1.0f},

  {1.0f, 1.0f}, //right
  {1.0f, 0.0f},
  {0.0f, 0.0f},
  {0.0f, 1.0f},

  {0.0f, 1.0f}, //left
  {0.0f, 0.0f},
  {1.0f, 0.0f},
  {1.0f, 1.0f}
};

const int cube_indexes[12][3] = 
{
  {8, 10, 11},  //up
  {8, 9, 10},

  {12, 13, 14},
  {12, 14, 15},  //down

  {16, 17, 18},
  {16, 18, 19},  //right
  {20, 21, 22},
  {20, 22, 23},   //left
  
  {0, 1, 2},
  {0, 2, 3},  //font
  {4, 5, 6},
  {4, 6, 7},  //back
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
		m_iWindowHeight(height),
    m_pTexture(NULL),
    m_bMoveCamera(false)
{
	m_pRender = new render::Render();
	m_pRender->SetViewPortSize(m_iWindowWidht, m_iWindowHeight);
  m_pCamera = new Camera;
  m_pCamera->ResetViewMat();
  m_pCamera->LookAt(2, 2, 2, 0, 1, 0, 0, 0, 0);
  //m_pCamera->LookAt(1.85f * 1.5f, 3.32f * 1.5f, -1.93f * 1.5f, 0, 1, 0, 0, 0, 0);
  //m_pCamera->LookAt(0.014, 3.609, 0.632, 0.707, 0.143, -0.69, 0, 0, 0);
  //m_pCamera->LookAt(-3.4277, 0.5803, 0.8342, -0.0907, 0.6735, -0.7335, 0, 0, 0);
  //m_pCamera->LookAt(-3.4068, 0.5856, 0.9597, -0.0823, 0.6732, -0.7348, 0, 0, 0);
  m_pCamera->ResetProjMat();
  m_pCamera->PerspectiveProj(90, 1, 1, 10);
  m_pRender->SetModelViewMat(m_pCamera->GetViewMat());
  m_pRender->SetProjMat(m_pCamera->GetProjMat());
  int vex_buffer = VertexDataCache::GetInstance()->GenVertexDataBuffer();
  VertexDataCache::GetInstance()->BindVertexDataBufferData
    (vex_buffer, kDataTypeFloat, 3, 24, (void*)cube_vertexs);
  int index_buffer = VertexDataCache::GetInstance()->GenVertexDataBuffer();
  VertexDataCache::GetInstance()->BindVertexDataBufferData
    (index_buffer, kDataTypeInt, 1, 36, (void*)cube_indexes);
  int color_buffer = VertexDataCache::GetInstance()->GenVertexDataBuffer();
  //VertexDataCache::GetInstance()->BindVertexDataBufferData
  //  (color_buffer, kDataTypeFloat, 4, 8, (void*)colors);
  int texcod_buffer = VertexDataCache::GetInstance()->GenVertexDataBuffer();
  VertexDataCache::GetInstance()->BindVertexDataBufferData
    (texcod_buffer, kDataTypeFloat, 2, 24, (void*)cube_texCod);
  m_pRender->SetVexVexDataBuffer(vex_buffer);
  m_pRender->SetVexIndexDataBuffer(index_buffer);
  m_pRender->SetVexColorDataBuffer(color_buffer);
  m_pRender->SetVexTexCoordDataBuffer(texcod_buffer);

  m_pTexture = render::TextureCache::GetInstance()->AddTexture("F:\\Git\\SoftCG\\res\\test.bmp");
  m_pRender->SetTexture(m_pTexture);
}

App::~App()
{
	SAFE_DELETE(m_pRender);
  SAFE_DELETE(m_pCamera);
  SAFE_DELETE(m_pTexture);
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

  m_pRender->SetModelViewMat(m_pCamera->GetViewMat());

  m_pRender->DrawTriangle(12);
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

void App::OnLButtonDown(int x, int y)
{
  m_bMoveCamera = true;
  m_iLastX = x;
  m_iLastY = y;
}

void App::OnLButtonUp(int x, int y)
{
  m_bMoveCamera = false;
  m_pCamera->UpdateCameraDeltaXY((x - m_iLastX) / 600.0f, (y - m_iLastY) / 600.0f);
  m_iLastX = x;
  m_iLastY = y;
}

void App::OnMouseMove(int x, int y)
{
  //return;
  //m_bMoveCamera = false;
  if(m_bMoveCamera)
  {
    m_pCamera->UpdateCameraDeltaXY(x - m_iLastX, y - m_iLastY);
    m_iLastX = x;
    m_iLastY = y;
  }
}
