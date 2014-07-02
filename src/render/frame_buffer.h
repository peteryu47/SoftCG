#ifndef __FRAME_BUFFER_H__
#define __FRAME_BUFFER_H__

#include <assert.h>

#include "utils/defines.h"
#include "utils/macro_utils.h"

namespace render
{

enum eFrameFormat
{
	kFrameFormatRGB,
	kFrameFormatRGBA,
	kFrameFormatA,
  kFrameFormatAF
};


class FrameBuffer
{
public:
	FrameBuffer(int width, int height, eFrameFormat frame_format)
		:	m_iWidth(width),
			m_iHeight(height),
      m_pData(NULL),
      m_pDataFloat(NULL),
			m_eFrameFormat(frame_format)
	{
		switch(frame_format)
		{
		case kFrameFormatRGB:
			m_iPixelByteCount = 3;
			break;
		case kFrameFormatRGBA:
			m_iPixelByteCount = 4;
			break;
		case kFrameFormatA:
			m_iPixelByteCount = 1;
			break;
    case kFrameFormatAF:
      m_iPixelByteCount = sizeof(float);
      break;
		default:
			assert(false);
		}

    if(frame_format == kFrameFormatAF)
      m_pDataFloat = new float[m_iHeight * m_iWidth * m_iPixelByteCount];
    else
		  m_pData = new uchar[m_iHeight * m_iWidth * m_iPixelByteCount];
	}

	~FrameBuffer()
	{
		SAFE_DELETE_N(m_pData);
    SAFE_DELETE_N(m_pDataFloat);
	}

	int		GetWidth(){return m_iWidth;}
	int		GetHeight(){return m_iHeight;}
  int   GetPixelByteCount(){return m_iPixelByteCount;}
  int   GetTotalByteCount(){return m_iWidth * m_iHeight * m_iPixelByteCount;}

	void	SetBufferDataRGB(int w, int h, uchar r, uchar g, uchar b)
	{
		int index = ARRAY_INDEX(w, h, m_iWidth) * 3;
		m_pData[index + 0] = r;
		m_pData[index + 1] = g;
		m_pData[index + 2] = b;
	}
	void  GetBufferDataRGB(int w, int h, uchar *r, uchar *g, uchar *b)
	{
		int index = ARRAY_INDEX(w, h, m_iWidth) * 3;
		*r = m_pData[index + 0];
		*g = m_pData[index + 1];
		*b = m_pData[index + 2];
	}
  void  SetBufferDataA(int w, int h, uchar a)
  {
    int index = ARRAY_INDEX(w, h, m_iWidth);
    m_pData[index] = a;
  }
  void  GetBufferDataA(int w, int h, uchar *a)
  {
    int index = ARRAY_INDEX(w, h, m_iWidth);
    *a = m_pData[index];
  }
  void  SetBufferDataAF(int w, int h, float a)
  {
    int index = ARRAY_INDEX(w, h, m_iWidth);
    m_pDataFloat[index] = a;
  }
  void  GetBufferDataAF(int w, int h, float *a)
  {
    int index = ARRAY_INDEX(w, h, m_iWidth);
    *a = m_pDataFloat[index];
  }

	uchar* GetFrameBufferData()
	{
		return m_pData;
	}



protected:
	uchar			*m_pData;
  float     *m_pDataFloat;
	int				m_iWidth;
	int				m_iHeight;
	int				m_iPixelByteCount;
	eFrameFormat	m_eFrameFormat;
};

}

#endif