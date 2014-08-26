#include "render/render.h"

#include <Windows.h>

#include "render/frame_buffer.h"
#include "render/draw_utils.h"

#include "utils/defines.h"
#include "utils/random_utils.h"
#include "render/vertex_data_cache.h"
#include "render/raster.h"

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
  VertexDataCache::GetInstance()->PurgeVertexDataCache();
}

void Render::Init()
{
	initFrameBuffers();
}

void Render::Clean()
{
	cleanFrameBuffers();
}

void Render::DrawTriangle(int count)
{
  CleanBackground(0, 0, 0);
  CleanZBuffer();

  //prepare raster input triangle data 
  std::vector<Triangle*> traingles;
  float *vex_ptr = (float*)VertexDataCache::GetInstance()->GetVertexDataDataPtr(m_iVexVexDataBuffer);
  int   *index_ptr = (int*)VertexDataCache::GetInstance()->GetVertexDataDataPtr(m_iVexIndexDataBuffer);
  float *color_ptr = (float*)VertexDataCache::GetInstance()->GetVertexDataDataPtr(m_iVexColorDataBuffer);
  VECTOR4D p0, p1, p2, rp0, rp1, rp2;
  for (int i = 0; i < count; ++i)
  {
    Triangle *triangle = new Triangle;
    int index0 = *(index_ptr + i * 3);
    int index1 = *(index_ptr + i * 3 + 1);
    int index2 = *(index_ptr + i * 3 + 2);
    memcpy(&p0, vex_ptr + index0 * 3, sizeof(float) * 3);
    memcpy(&p1, vex_ptr + index1 * 3, sizeof(float) * 3);
    memcpy(&p2, vex_ptr + index2 * 3, sizeof(float) * 3);

    memcpy(&triangle->colors[0], color_ptr + index0 * 4, sizeof(float) * 4);
    memcpy(&triangle->colors[1], color_ptr + index1 * 4, sizeof(float) * 4);
    memcpy(&triangle->colors[2], color_ptr + index2 * 4, sizeof(float) * 4);

    p0.w = 1.0f; p1.w = 1.0f; p2.w = 1.0f;
    Mat_Mul_VECTOR4D_4X4(&p0, &m_matMVP, &rp0);
    Mat_Mul_VECTOR4D_4X4(&p1, &m_matMVP, &rp1);
    Mat_Mul_VECTOR4D_4X4(&p2, &m_matMVP, &rp2);

    rp0.x /= rp0.w; rp0.y /= rp0.w; rp0.z /= rp0.w;
    rp1.x /= rp1.w; rp1.y /= rp1.w; rp1.z /= rp1.w;
    rp2.x /= rp2.w; rp2.y /= rp2.w; rp2.z /= rp2.w;

    rp0.x = (rp0.x + 1) * m_iViewPortWidth / 2.0f; rp0.y = (rp0.y + 1) * m_iViewPortHeight / 2.0f;
    rp1.x = (rp1.x + 1) * m_iViewPortWidth / 2.0f; rp1.y = (rp1.y + 1) * m_iViewPortHeight / 2.0f;
    rp2.x = (rp2.x + 1) * m_iViewPortWidth / 2.0f; rp2.y = (rp2.y + 1) * m_iViewPortHeight / 2.0f;

    memcpy(&triangle->vertexs[0], &rp0, sizeof(float) * 3);
    memcpy(&triangle->vertexs[1], &rp1, sizeof(float) * 3);
    memcpy(&triangle->vertexs[2], &rp2, sizeof(float) * 3);
    traingles.push_back(triangle);
  }

  std::vector<OutPointPackage*> out_point_packages;
  Raster::GetInstance()->Rasterise(traingles, out_point_packages);

  OutPoint* out_point = NULL; 
  for(std::vector<OutPointPackage*>::iterator itr = out_point_packages.begin();
      itr != out_point_packages.end(); ++itr)
  {
    for (int i = 0; i < (*itr)->GetInvaildCount(); ++i)
    {
      out_point = (*itr)->GetPointByIndex(i);
      float z = 0;
      m_pZFrameBuffer->GetBufferDataAF(out_point->vertex.x, out_point->vertex.y, &z);
      if(out_point->vertex.z < z)
      {
        m_pSceneFrameBuffer->SetBufferDataRGB(out_point->vertex.x, out_point->vertex.y, 
          out_point->color.x, out_point->color.y, out_point->color.z);
        m_pZFrameBuffer->SetBufferDataAF(out_point->vertex.x, out_point->vertex.y, out_point->vertex.z);
      }
    }
  }

  //clean all temp data
  for(std::vector<Triangle*>::iterator itr = traingles.begin();
      itr != traingles.end(); ++itr)
  {
    delete (*itr);
  }
  traingles.clear();
  for(std::vector<OutPointPackage*>::iterator itr = out_point_packages.begin();
      itr != out_point_packages.end(); ++itr)
  {
    delete (*itr);
  }
  out_point_packages.clear();
}

void Render::CleanBackground(int r, int g, int b)
{
  memset(m_pSceneFrameBuffer->GetFrameBufferData(), 255, 
    sizeof(uchar) * m_pSceneFrameBuffer->GetTotalByteCount());
}

void Render::CleanZBuffer()
{
  //memset(m_pZFrameBuffer->GetFrameBufferDataFloat(), 128, 
  //  sizeof(float) * m_pZFrameBuffer->GetTotalByteCount());
  int total = m_pZFrameBuffer->GetWidth() * m_pZFrameBuffer->GetHeight();
  float *data_ptr = m_pZFrameBuffer->GetFrameBufferDataFloat();
  for(int i = 0; i < total ; ++i)
    *(data_ptr + i) = 100000000.0f;
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
	m_pZFrameBuffer = new FrameBuffer(m_iViewPortWidth, m_iViewPortHeight, kFrameFormatAF);
}

void Render::cleanFrameBuffers()
{
  SAFE_DELETE(m_pSceneFrameBuffer);
	SAFE_DELETE(m_pZFrameBuffer);
}

}