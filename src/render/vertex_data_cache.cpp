#include "render/vertex_data_cache.h"

#include "utils/defines.h"
#include "utils/random_utils.h"

namespace render
{

VertexDataCache* VertexDataCache::m_pvertex_data_cache_ = NULL;

VertexDataCache::VertexDataCache()
{

}

VertexDataCache::~VertexDataCache()
{

}

VertexDataCache* VertexDataCache::GetInstance()
{
  if(m_pvertex_data_cache_ == NULL)
  {
    m_pvertex_data_cache_ = new VertexDataCache();
  }
  return m_pvertex_data_cache_;
}

}