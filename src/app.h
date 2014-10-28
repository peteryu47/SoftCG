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

  void          OnLButtonDown(int x, int y);
  void          OnLButtonUp(int x, int y);
  void          OnMouseMove(int x, int y);

protected:
	render::Render		*m_pRender;
  Camera            *m_pCamera;
	int			          m_iWindowWidht;
	int			          m_iWindowHeight;
  float             m_fFrameTime;
  render::Texture   *m_pTexture;

  bool              m_bMoveCamera;
  int               m_iLastX;
  int               m_iLastY;
};

#endif