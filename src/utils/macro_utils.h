#ifndef __MACRO_UTILS_H__
#define __MACRO_UTILS_H__

#include <cstdlib>

#define SAFE_DELETE(x)    do{if((x) != NULL) delete (x); x = NULL;}while(0)
#define SAFE_DELETE_N(x)  do{if((x) != NULL) delete[] (x); x = NULL;}while(0)
#define SAFE_FREE(x)      do{if((x) != NULL) free(x); x = NULL;}while(0)

#define ARRAY_INDEX(w, h, width) (int((h) * (width) + (w)))

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);\
  void operator=(const TypeName&)

#endif