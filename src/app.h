#ifndef __APP_H__
#define __APP_H__

namespace render
{
	class Render;
	class FrameBuffer;
}

class App
{
public:
  App(int width, int height);
  ~App();

public:
  void Run();
  virtual void  Update(float delta);
  float         GetFPS();

protected:
	render::Render		*m_pRender;
	int			m_iWindowWidht;
	int			m_iWindowHeight;
};

#endif