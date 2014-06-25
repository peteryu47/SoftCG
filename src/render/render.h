#ifndef __RENDER_H__
#define __RENDER_H__

#include <assert.h>
#include "math/math_3d.h"

namespace render
{

class FrameBuffer;

class Render
{
public:
	Render();
	~Render();

	void			Init();
	void			Clean();
	void			DrawFrame();

	FrameBuffer*	GetCurSceneFrameBuffer(){return m_pSceneFrameBuffer;}

public:
	void			SetModelViewMat(MATRIX4X4_PTR mvMat);
	void			SetProjectionMat(MATRIX4X4_PTR pMat);
	void			SetViewPortSize(int width, int height);

protected:
	void			initFrameBuffers();
	void			cleanFrameBuffers();

private:
	FrameBuffer		*m_pSceneFrameBuffer;
	FrameBuffer		*m_pZFrameBuffer;

	MATRIX4X4		m_matModelView;
	MATRIX4X4		m_matProjection;

	int				m_iViewPortWidth;
	int				m_iViewPortHeight;
};

}

#endif