#ifndef __RANDOM_UTILS_H__
#define __RANDOM_UTILS_H__

#include <math.h>
#include <stdlib.h>

// [0, up)
static int RandInt(int up)
{
  return (rand() % up);
}

// [down, up)
static int RandIntSection(int down, int up)
{
  return RandInt(up - down) + down;
}

// [0, 1)
static float RandFloat()
{
  return ((rand() % 1234567) / 1234567.0f);
}

#endif