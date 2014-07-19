#ifndef __RENDER_H__
#define __RENDER_H__

#include <assert.h>
#include "math/math_3d.h"
#include "math/camera.h"
#include "utils/macro_utils.h"

namespace render
{

class VertexDataCache
{
private:
  VertexDataCache();
public:
  ~VertexDataCache();
  DISALLOW_COPY_AND_ASSIGN(VertexDataCache);

  static VertexDataCache* GetInstance();

private:
  static VertexDataCache*      m_pvertex_data_cache_;
};

}

#endif