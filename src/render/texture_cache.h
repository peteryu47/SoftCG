#ifndef __TEXTURE_CACHE_H__
#define __TEXTURE_CACHE_H__

#include <assert.h>
#include <vector>
#include <map>
#include "utils/macro_utils.h"
#include "utils/defines.h"

using namespace std;

namespace render
{

class Texture;

class TextureCache
{
private:
  TextureCache();
public:
  ~TextureCache();
  DISALLOW_COPY_AND_ASSIGN(TextureCache);

  static  TextureCache* GetInstance(){
    static TextureCache texture_cache; 
    return &texture_cache;
  }

  void    PurgeTextureCache();

  Texture* AddTexture(char* file_path);

protected:
  int     genTextureBuffer();
  void    loadTextureBmp(Texture* texture, char* file_path);

private:
  int     m_iTextureBufferStartId;
};

}

#endif