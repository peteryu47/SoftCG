#ifndef __MATH_3D_H__
#define __MATH_3D_H__

typedef struct VECTOR2D_TYP
{
	union
	{
		float M[2];
		struct
		{
			float x, y;
		};
	};
}VECTOR2D, POINT2D, *VECTOR2D_PTR, *POINT2D_PTR;

typedef struct VECTOR3D_TYP
{
	union
	{
		float M[3];
		struct
		{
			float x, y, z;
		};
	};
}VECTOR3D, POINT3D, *VECTOR3D_PTR, *POINT3D_PTR;


typedef struct VERTEX2DI_TYP
{
	int x, y;
}VERTEX2DI, *VERTEX2DI_PTR;

typedef struct VERTEX2DF_TYP
{
	float x, y;
}VERTEX2DF, *VERTEX2DF_PTR;

typedef struct PARMLINE2D_TYP
{
	POINT2D p0;		//start point
	POINT2D p1;		//end point
	VECTOR2D v;		//direction vector
}PARMLINE2D, *PARMLINE2D_PTR;

typedef struct PARMLINE3D_TYP
{
	POINT3D p0;		//start point
	POINT3D p1;		//end point
	VECTOR3D v;		//direction vector
}PARMLINE3D, *PARMLINE3D_PTR;

typedef struct PLANE3D_TYP
{
	POINT3D p0;		//start point	POINT3D p1;		//end point
	VECTOR3D n;		//normal vector
}PLANE3D, *PLANE3D_PTR;

typedef struct MATRIX4X4_TYP
{
	union
	{
		float M[4][4];
		struct
		{
			float M00, M01, M02, M03;
			float M10, M11, M12, M13;
			float M20, M21, M22, M23;
			float M30, M31, M32, M33;
		};
	};
}MATRIX4X4, *MATRIX4X4_PTR;

#endif