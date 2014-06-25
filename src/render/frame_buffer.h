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
	kFrameFormatA
};


class FrameBuffer
{
public:
	FrameBuffer(int width, int height, eFrameFormat frame_format)
		:	m_iWidth(width),
			m_iHeight(height),
			m_eFrameFormat(frame_format)
	{
		int pixel_byte_count = 0;
		switch(frame_format)
		{
		case kFrameFormatRGB:
			pixel_byte_count = 3;
			break;
		case kFrameFormatRGBA:
			pixel_byte_count = 4;
			break;
		case kFrameFormatA:
			pixel_byte_count = 1;
			break;
		default:
			assert(false);
		}
		m_pData = new uchar[m_iHeight * m_iWidth * pixel_byte_count];
	}

	~FrameBuffer()
	{
		SAFE_DELETE_N(m_pData);
	}

	int		GetWidth(){return m_iWidth;}
	int		GetHeight(){return m_iHeight;}

	void	SetBufferDataRGB(int w, int h, uchar r, uchar g, uchar b)
	{
		int index = ARRAY_INDEX(w, h, m_iWidth) * 3;
		m_pData[index + 0] = r;
		m_pData[index + 1] = g;
		m_pData[index + 2] = b;
	}
	void GetBufferData(int w, int h, uchar *r, uchar *g, uchar *b)
	{
		int index = ARRAY_INDEX(w, h, m_iWidth) * 3;
		*r = m_pData[index + 0];
		*g = m_pData[index + 1];
		*b = m_pData[index + 2];
	}

	uchar* GetFrameBufferData()
	{
		return m_pData;
	}

protected:
	uchar			*m_pData;
	int				m_iWidth;
	int				m_iHeight;
	int				m_iPixelByteCount;
	eFrameFormat	m_eFrameFormat;
};

}

#endif