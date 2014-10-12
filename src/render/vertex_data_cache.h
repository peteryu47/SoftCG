#ifndef __VERTEX_DATA_CACHE_H__
#define __VERTEX_DATA_CACHE_H__

#include <assert.h>
#include <vector>
#include <map>
#include "utils/macro_utils.h"
#include "utils/defines.h"

using namespace std;

#define VERTEXE_DATA_CACHE_MAX_COUNT 100

namespace render
{

class VertexDataCache
{
private:
  VertexDataCache();
public:
  ~VertexDataCache();
  DISALLOW_COPY_AND_ASSIGN(VertexDataCache);

  static  VertexDataCache* GetInstance(){
    static VertexDataCache vertex_data_cache; 
    return &vertex_data_cache;
  }

  void    PurgeVertexDataCache();

  int     GenVertexDataBuffer();
  void    BindVertexDataBufferData( int buffer_id, eDataType data_type, int unit_size, 
                                    int unit_count, void *data_ptr);
  void    ReleaseVertexDataBufferData(int buffer_id);

  eDataType   GetVertexDataDataType(int buffer_id){return m_data_caches[buffer_id].data_type;}
  int         GetVertexDataUnitSize(int buffer_id){return m_data_caches[buffer_id].unit_size;}
  int         GetVertexDataUnitCount(int buffer_id){return m_data_caches[buffer_id].unit_count;}
  void*       GetVertexDataDataPtr(int buffer_id){return m_data_caches[buffer_id].data_ptr;}

private:
  struct DataCache
  {
    eDataType   data_type;
    int         unit_size;
    int         unit_count;
    void*       data_ptr;
    bool        is_use;
  };

  DataCache m_data_caches[VERTEXE_DATA_CACHE_MAX_COUNT];
};

}

#endif