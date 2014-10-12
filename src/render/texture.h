#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <assert.h>
#include <vector>
#include <map>
#include "utils/macro_utils.h"
#include "utils/defines.h"

using namespace std;

namespace render
{

class TextureCache;

class Texture
{
  friend class TextureCache;
public:
  Texture();
  ~Texture();

  int     GetWidth(){return m_iTextureWidth;}
  int     GetHeight(){return m_iTextureHeight;}
  void    GetRGBA(float w, float h, uchar *r, uchar *g, uchar *b, uchar *a);

private:
  int     m_iTextureWidth;
  int     m_iTextureHeight;
  int     m_iLineTotalLen;
  char*   m_pData;
  eRGBByteType  m_eRGBByteType;
};

}

#endif