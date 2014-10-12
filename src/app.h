#ifndef __APP_H__
#define __APP_H__

class Camera;
namespace render
{
	class Render;
	class FrameBuffer;
  class Texture;
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
  Camera            *m_pCamera;
	int			          m_iWindowWidht;
	int			          m_iWindowHeight;
  float             m_fFrameTime;
  render::Texture   *m_pTexture;
};

#endif