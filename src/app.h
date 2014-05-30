#ifndef __APP_H__
#define __APP_H__

class FrameBuffer;

class App
{
public:
  App(int width, int height);
  ~App();

public:
  void Run();
  virtual void Update(float delta);

protected:
	FrameBuffer *m_pFrameBuffer;
	int			m_iWindowWidht;
	int			m_iWindowHeight;
};

#endif