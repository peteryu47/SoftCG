#include "render/vertex_data_cache.h"

#include "utils/defines.h"
#include "utils/macro_utils.h"
#include "utils/random_utils.h"

namespace render
{

VertexDataCache::VertexDataCache()
{
  for (int i = 0; i < VERTEXE_DATA_CACHE_MAX_COUNT; ++i)
  {
      m_data_caches[i].is_use = false;
  }
}

VertexDataCache::~VertexDataCache()
{
  PurgeVertexDataCache();
}

void VertexDataCache::PurgeVertexDataCache()
{
  for (int i = 0; i < VERTEXE_DATA_CACHE_MAX_COUNT; ++i)
  {
    SAFE_FREE(m_data_caches[i].data_ptr);
  }
}

int VertexDataCache::GenVertexDataBuffer()
{
  for (int i = 0; i < VERTEXE_DATA_CACHE_MAX_COUNT; ++i)
  {
    if(m_data_caches[i].is_use == false)
    {
      m_data_caches[i].is_use = true;
      return i;
    }
  }
  assert(false);
  return -1;
}

void VertexDataCache::BindVertexDataBufferData( int buffer_id, eDataType data_type, int unit_size, int unit_count, void *data_ptr )
{
  m_data_caches[buffer_id].data_type  = data_type;
  m_data_caches[buffer_id].unit_size  = unit_size;
  m_data_caches[buffer_id].unit_count = unit_count;
  switch (data_type)
  {
  case kDataTypeInt:
    m_data_caches[buffer_id].data_ptr = malloc(sizeof(int) * unit_size * unit_count);
    memcpy(m_data_caches[buffer_id].data_ptr, data_ptr, sizeof(int) * unit_size * unit_count);
    break;
  case kDataTypeFloat:
    m_data_caches[buffer_id].data_ptr = malloc(sizeof(float) * unit_size * unit_count);
    memcpy(m_data_caches[buffer_id].data_ptr, data_ptr, sizeof(float) * unit_size * unit_count);
    break;
  default:
    assert(false);
  }
}

void VertexDataCache::ReleaseVertexDataBufferData( int buffer_id )
{
  m_data_caches[buffer_id].is_use = false;
  SAFE_DELETE(m_data_caches[buffer_id].data_ptr);
}

}