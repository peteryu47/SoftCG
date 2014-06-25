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
	int			m_iWindowWidht;
	int			m_iWindowHeight;
};

#endif