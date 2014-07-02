#ifndef __RENDER_H__
#define __RENDER_H__

#include <assert.h>
#include "math/math_3d.h"
#include "math/camera.h"

namespace render
{

class FrameBuffer;

class Render
{
public:
	Render();
	~Render();

	void			    Init();
	void			    Clean();
	void			    DrawFrame();

  float         GetFPS(){return m_fFPS;}
	FrameBuffer*	GetCurSceneFrameBuffer(){return m_pSceneFrameBuffer;}

public:
	void			SetViewPortSize(int width, int height);

protected:
	void			initFrameBuffers();
	void			cleanFrameBuffers();

private:
	FrameBuffer		*m_pSceneFrameBuffer;
	FrameBuffer		*m_pZFrameBuffer;

	int				m_iViewPortWidth;
	int				m_iViewPortHeight;

  float     m_fFPS;

  Camera    m_camera;
};

}

#endif