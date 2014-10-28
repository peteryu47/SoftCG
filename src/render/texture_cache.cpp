#include "render/texture_cache.h"

#include "utils/defines.h"
#include "utils/macro_utils.h"
#include "utils/random_utils.h"
#include "render/texture.h"
#include "utils/file_manager.h"
#include <Windows.h>

namespace render
{

TextureCache::TextureCache()
{
  m_iTextureBufferStartId = 0;
}

TextureCache::~TextureCache()
{
  PurgeTextureCache();
}

void TextureCache::PurgeTextureCache()
{

}

Texture* TextureCache::AddTexture(char* file_path)
{
  Texture *texture = new Texture();
  loadTextureBmp(texture, file_path);
  return texture;
}

int TextureCache::genTextureBuffer()
{
  return (++m_iTextureBufferStartId);
}

void TextureCache::loadTextureBmp(Texture* texture, char* file_path)
{
  assert(texture != NULL);
  int data_len = 0;
  char* data = FileManager::GetInstance()->GetFileData(file_path, &data_len);
  if(data == NULL)
  {
    assert(false);
    return;
  }
  int width = 0;
  int height = 0;
  int lineLength = 0;
  int totalLength = 0;
  memcpy(&width, data + 0x12, 4);
  memcpy(&height, data + 0x16, 4);		
  lineLength = width * 3;
  while(lineLength % 4 != 0)
    lineLength++;
  totalLength = lineLength * height;

  texture->m_pData = new char[totalLength];
  memcpy(texture->m_pData, data + 54, totalLength * sizeof(char));
  texture->m_eRGBByteType = kRGBByteTypeRGB24;
  texture->m_iTextureWidth = width;
  texture->m_iTextureHeight = height;
  texture->m_iLineTotalLen = lineLength;
  SAFE_DELETE_N(data);
}

}