#ifndef __DEFINES_H__
#define __DEFINES_H__

typedef unsigned char   uchar;
typedef unsigned short  ushort;
typedef unsigned int    uint;

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

enum eDataType
{
  kDataTypeByte,
  kDataTypeChar,
  kDataTypeUChar,
  kDataTypeShort,
  kDataTypeUShort,
  kDataTypeInt,
  kDataTypeUInt,
  kDataTypeFloat
};

enum eRGBByteType
{
  kRGBByteTypeRGB24,
  kRGBByteTypeRGBA32,
  kRGBByteTypeA32F
};

#endif