#ifndef __RENDER_H__
#define __RENDER_H__

#include <assert.h>
#include <list>
#include "math/math_3d.h"
#include "math/camera.h"

namespace render
{

class FrameBuffer;
struct Triangle;
class Texture;

class Render
{
public:
	Render();
	~Render();

	void			    Init();
	void			    Clean();
	void			    DrawTriangle(int count);
  void          CleanBackground(int r, int g, int b);
  void          CleanZBuffer();

  float         GetFPS(){return m_fFPS;}
	FrameBuffer*	GetCurSceneFrameBuffer(){return m_pSceneFrameBuffer;}
  void          SetVexVexDataBuffer(int buffer_id){m_iVexVexDataBuffer = buffer_id;}
  void          SetVexColorDataBuffer(int buffer_id){m_iVexColorDataBuffer = buffer_id;}
  void          SetVexNormalDataBuffer(int buffer_id){m_iVexNormalDataButtfer = buffer_id;}
  void          SetVexTexCoordDataBuffer(int buffer_id){m_iVexTexCoordDataBuffer = buffer_id;}
  void          SetVexIndexDataBuffer(int buffer_id){m_iVexIndexDataBuffer = buffer_id;}
  void          SetModelViewMat(MATRIX4X4_PTR mat_view){MAT_COPY_4X4(mat_view, &m_matMView); Mat_Mul_4X4(&m_matMView, &m_matProj, &m_matMVP);}
  void          SetProjMat(MATRIX4X4_PTR mat_proj){MAT_COPY_4X4(mat_proj, &m_matProj); Mat_Mul_4X4(&m_matMView, &m_matProj, &m_matMVP);}
  void          SetTexture(Texture *texture){m_pTexture = texture;}

public:
	void			SetViewPortSize(int width, int height);

protected:
	void			initFrameBuffers();
	void			cleanFrameBuffers();
  void      filterBackTriangle(std::list<Triangle*> &triangles);

private:
	FrameBuffer		*m_pSceneFrameBuffer;
	FrameBuffer		*m_pZFrameBuffer;

	int				m_iViewPortWidth;
	int				m_iViewPortHeight;

  float     m_fFPS;

  MATRIX4X4 m_matMView;
  MATRIX4X4 m_matProj;
  MATRIX4X4 m_matMVP;
  MATRIX4X4 m_matPort;

  int       m_iVexVexDataBuffer;
  int       m_iVexColorDataBuffer;
  int       m_iVexNormalDataButtfer;
  int       m_iVexTexCoordDataBuffer;
  int       m_iVexIndexDataBuffer;

  Texture*  m_pTexture;
};

}

#endif