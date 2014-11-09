#include "render/texture.h"

#include "utils/defines.h"
#include "utils/macro_utils.h"
#include "utils/random_utils.h"

namespace render
{

Texture::Texture()
{
  m_iTextureWidth = 0;
  m_iTextureHeight = 0;
  m_iLineTotalLen = 0;
  m_pData = NULL;
  m_eRGBByteType = kRGBByteTypeRGBA32;
}

Texture::~Texture()
{
  SAFE_FREE(m_pData);
}

void Texture::GetRGBA(float w, float h, uchar *r, uchar *g, uchar *b, uchar *a)
{
  if(w < 0.0f)
    w = 0.0f;
  if(w > 1.0f)
    w = 1.0f;
  if(h < 0.0f)
    h = 0.0f;
  if(h > 1.0f)
    h = 1.0f;
  float realf_w = w * (m_iTextureWidth - 1);
  float realf_h = h * (m_iTextureHeight - 1);
  int reali_w = (realf_w > (int(realf_w) + 0.5f)) ? (int(realf_w) + 1) : int(realf_w);
  int reali_h = (realf_h > (int(realf_h) + 0.5f)) ? (int(realf_h) + 1) : int(realf_h);
  //int pos = ARRAY_INDEX(int(w * (m_iTextureWidth - 1)) * 3, int(h * (m_iTextureHeight - 1)), m_iLineTotalLen);
  int pos = ARRAY_INDEX(reali_w * 3, reali_h, m_iLineTotalLen);
  if (m_eRGBByteType == kRGBByteTypeRGB24)
  {
    *r = (uchar)*(m_pData + pos);
    *g = (uchar)*(m_pData + pos + 1);
    *b = (uchar)*(m_pData + pos + 2);
  }
  else if(m_eRGBByteType == kRGBByteTypeRGBA32)
  {
    *r = (uchar)*(m_pData + pos);
    *g = (uchar)*(m_pData + pos + 1);
    *b = (uchar)*(m_pData + pos + 2);
    *a = (uchar)*(m_pData + pos + 3);
  }
  else
    assert(false);
}

}