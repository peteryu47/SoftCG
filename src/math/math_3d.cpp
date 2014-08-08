#include "math_3d.h"

#include <math.h>

float cos_look[361];
float sin_look[361];

float Fast_Sin( float theta )
{
	theta = fmodf(theta, 360.0f);

	if(theta < 0) 
		theta += 360.0;

	int theta_int = (int)theta;
	float theta_frac = theta - theta_int;

	return (sin_look[theta_int] + theta_frac * (sin_look[theta_int + 1] - sin_look[theta_int]));	
}

float Fast_Cos( float theta )
{
	theta = fmodf(theta, 360.0f);

	if(theta < 0) 
		theta += 360.0;

	int theta_int = (int)theta;
	float theta_frac = theta - theta_int;

	return (cos_look[theta_int] + theta_frac * (cos_look[theta_int + 1] - cos_look[theta_int]));	
}

float Fast_Sqrt( float x )
{
	return (1.0f / Fast_Inv_Sqrt(x));
}

float Fast_Inv_Sqrt( float x )
{
	float xhalf = 0.5f * x;
	int i = *(int*)&x;
	i = 0x5f3759df - (i >> 1);
	x = *(float*)&i;
	x = x * (1.5f - xhalf * x * x);
	return x;
}

int Fast_Distance_2D( int x, int y )
{
	return Fast_Sqrt(x * x + y * y);
}

float Faste_Distance_3D( float x, float y, float z )
{
	return Fast_Sqrt(x * x + y * y + z * z);
}

void POLAR2D_TO_POINT2D( POLAR2D_PTR polar, POINT2D_PTR rect )
{
	rect->x = polar->r * cosf(polar->theta);
	rect->y = polar->r * sinf(polar->theta);
}

void POLAR2D_TO_RectXY( POLAR2D_PTR polar, float *x, float *y )
{
	*x = polar->r * cosf(polar->theta);
	*y = polar->r * sinf(polar->theta);
}

void POINT2D_TO_POLAR2D( POINT2D_PTR rect, POLAR2D_PTR polar )
{
	polar->r = sqrtf(rect->x * rect->x + rect->y * rect->y);
	polar->theta = atanf(rect->y / rect->x);
}

void POINT2D_TO_PolarRTh( POINT2D_PTR rect, float *r, float *theta )
{
	*r = sqrtf(rect->x * rect->x + rect->y * rect->y);
	*theta = atanf(rect->y / rect->x);
}

void CYLINDRICAL3D_TO_POINT3D( CYLINDRICAL3D_PTR cyl, POINT3D_PTR rect )
{
	rect->x = cyl->r * cosf(cyl->theta);
	rect->y = cyl->r * sinf(cyl->theta);
	rect->z = cyl->z;
}

void CYLINDRICAL3D_TO_RectXYZ( CYLINDRICAL3D_PTR cyl, float *x, float *y, float *z )
{
	*x = cyl->r * cosf(cyl->theta);
	*y = cyl->r * sinf(cyl->theta);
	*z = cyl->z;
}

void POINT3D_TO_CYLINDRICAL3D( POINT3D_PTR rect, CYLINDRICAL3D_PTR cyl )
{
	cyl->r = sqrtf(rect->x * rect->x + rect->y * rect->y + rect->z * rect->z);
	cyl->theta = atanf(rect->y / rect->x);
	cyl->z = rect->z;
}

void POINT3D_TO_CylindricalRThZ( POINT3D_PTR rect, float *r, float *theta, float *z )
{
	*r = sqrtf(rect->x * rect->x + rect->y * rect->y + rect->z * rect->z);
	*theta = atanf(rect->y / rect->x);
	*z = rect->z;
}

void SPHERICAL3D_TO_POINT3D( SPHERICAL3D_PTR sph, POINT3D_PTR rect )
{
	float sinPhi = sinf(sph->phi);
	rect->x = sph->p * sinPhi * cosf(sph->theta);
	rect->y = sph->p * sinPhi * sinf(sph->theta);
	rect->z = sph->p * cosf(sph->phi);
}

void SPHERICAL3D_TO_RectXYZ( SPHERICAL3D_PTR sph, float *x, float *y, float *z )
{
	float sinPhi = sinf(sph->phi);
	*x = sph->p * sinPhi * cosf(sph->theta);
	*y = sph->p * sinPhi * sinf(sph->theta);
	*z = sph->p * cosf(sph->phi);
}

void POINT3D_TO_SPHERICAL3D( POINT3D_PTR rect, SPHERICAL3D_PTR sph )
{
	sph->p = sqrtf(rect->x * rect->x + rect->y * rect->y + rect->z * rect->z);
	sph->phi = acosf(rect->z / sph->p);
	sph->theta = atanf(rect->y / rect->x);
}

void POINT3D_TO_SphericalPThPh( POINT3D_PTR rect, float *p, float *theta, float *phi )
{
	*p = sqrtf(rect->x * rect->x + rect->y * rect->y + rect->z * rect->z);
	*phi = acosf(rect->z / *p);
	*theta = atanf(rect->y / rect->x);
}

void VECTOR2D_Add( VECTOR2D_PTR va, VECTOR2D_PTR vb, VECTOR2D_PTR vsum )
{
	vsum->x = va->x + vb->x;
	vsum->y = va->y + vb->y;
}

VECTOR2D VECTOR2D_Add( VECTOR2D_PTR va, VECTOR2D_PTR vb )
{
	VECTOR2D vresult;
	vresult.x = va->x + vb->x;
	vresult.y = va->y + vb->y;
	return vresult;
}

void VECTOR2D_Sub( VECTOR2D_PTR va, VECTOR2D_PTR vb, VECTOR2D_PTR vdiff )
{
	vdiff->x = va->x - vb->x;
	vdiff->y = va->y - vb->y;
}

VECTOR2D VECTOR2D_Sub( VECTOR2D_PTR va, VECTOR2D_PTR vb )
{
	VECTOR2D vresult;
	vresult.x = va->x - vb->x;
	vresult.y = va->y - vb->y;
	return vresult;
}

void VECTOR2D_Scale( float k, VECTOR2D_PTR va )
{
	va->x *= k;
	va->y *= k;
}

void VECTOR2D_Scale( float k, VECTOR2D_PTR va, VECTOR2D_PTR vscaled )
{
	vscaled->x = va->x * k;
	vscaled->y = va->y * k;
}

float VECTOR2D_Dot( VECTOR2D_PTR va, VECTOR2D_PTR vb )
{
	return (va->x * vb->x + va->y * vb->y);
}

float VECTOR2D_Length( VECTOR2D_PTR va )
{
	return sqrtf(va->x * va->x + va->y * va->y);
}

float VECTOR2D_Lenght_Fast( VECTOR2D_PTR va )
{
	return Fast_Sqrt(va->x * va->x + va->y * va->y);
}

void VECTOR2D_Normalize( VECTOR2D_PTR va )
{
	float len = sqrtf(va->x * va->x + va->y * va->y);
	va->x /= len;
	va->y /= len;
}

void VECTOR2D_Normalize( VECTOR2D_PTR va, VECTOR2D_PTR vn )
{
	float len = sqrtf(va->x * va->x + va->y * va->y);
	vn->x = va->x / len;
	vn->y = va->y / len;
}

void VECTOR2D_Build( VECTOR2D_PTR init, VECTOR2D_PTR term, VECTOR2D_PTR result )
{
	result->x = term->x - init->x;
	result->y = term->y - init->y;
}

float VECTOR2D_CosTh( VECTOR2D_PTR va, VECTOR2D_PTR vb )
{
	float dot = va->x * vb->x + va->y * vb->y;
	float len_a = sqrtf(va->x * va->x + va->y * va->y);
	float len_b = sqrtf(vb->x * vb->x + vb->y * vb->y);
	return (dot / (len_a * len_b));
}

///////////////////
void VECTOR3D_Add( VECTOR3D_PTR va, VECTOR3D_PTR vb, VECTOR3D_PTR vsum )
{
	vsum->x = va->x + vb->x;
	vsum->y = va->y + vb->y;
	vsum->z = va->z + vb->z;
}

VECTOR3D VECTOR3D_Add( VECTOR3D_PTR va, VECTOR3D_PTR vb )
{
	VECTOR3D vresult;
	vresult.x = va->x + vb->x;
	vresult.y = va->y + vb->y;
	vresult.z = va->z + vb->z;
	return vresult;
}

void VECTOR3D_Sub( VECTOR3D_PTR va, VECTOR3D_PTR vb, VECTOR3D_PTR vdiff )
{
	vdiff->x = va->x - vb->x;
	vdiff->y = va->y - vb->y;
	vdiff->z = va->z - vb->z;
}

VECTOR3D VECTOR3D_Sub( VECTOR3D_PTR va, VECTOR3D_PTR vb )
{
	VECTOR3D vresult;
	vresult.x = va->x - vb->x;
	vresult.y = va->y - vb->y;
	vresult.z = va->z - vb->z;
	return vresult;
}

void VECTOR3D_Scale( float k, VECTOR3D_PTR va )
{
	va->x *= k;
	va->y *= k;
	va->z *= k;
}

void VECTOR3D_Scale( float k, VECTOR3D_PTR va, VECTOR3D_PTR vscaled )
{
	vscaled->x = va->x * k;
	vscaled->y = va->y * k;
	vscaled->z = va->z * k;
}

float VECTOR3D_Dot( VECTOR3D_PTR va, VECTOR3D_PTR vb )
{
	return (va->x * vb->x + va->y * vb->y + va->z * vb->z);
}

void VECTOR3D_Cross( VECTOR3D_PTR va, VECTOR3D_PTR vb, VECTOR3D_PTR vn )
{
	vn->x = va->y * vb->z - va->z *vb->y;
	vn->y = va->z * vb->x - va->x *vb->z;
	vn->z = va->x * vb->y - va->y *vb->x;
}

VECTOR3D VECTOR3D_Cross( VECTOR3D_PTR va, VECTOR3D_PTR vb )
{
	VECTOR3D vresult;
	vresult.x = va->y * vb->z - va->z *vb->y;
	vresult.y = va->z * vb->x - va->x *vb->z;
	vresult.z = va->x * vb->y - va->y *vb->x;
	return vresult;
}

float VECTOR3D_Length( VECTOR3D_PTR va )
{
	return sqrtf(va->x * va->x + va->y * va->y + va->z * va->z);
}

float VECTOR3D_Lenght_Fast( VECTOR3D_PTR va )
{
	return Fast_Sqrt(va->x * va->x + va->y * va->y + va->z * va->z);
}

void VECTOR3D_Normalize( VECTOR3D_PTR va )
{
	float len = sqrtf(va->x * va->x + va->y * va->y + va->z * va->z);
	va->x /= len;
	va->y /= len;
	va->z /= len;
}

void VECTOR3D_Normalize( VECTOR3D_PTR va, VECTOR3D_PTR vn )
{
	float len = sqrtf(va->x * va->x + va->y * va->y + va->z * va->z);
	vn->x = va->x / len;
	vn->y = va->y / len;
	vn->z = va->z / len;
}

void VECTOR3D_Build( VECTOR3D_PTR init, VECTOR3D_PTR term, VECTOR3D_PTR result )
{
	result->x = term->x - init->x;
	result->y = term->y - init->y;
	result->z = term->z - init->z;
}

float VECTOR3D_CosTh( VECTOR3D_PTR va, VECTOR3D_PTR vb )
{
	float dot = va->x * vb->x + va->y * vb->y + va->z * vb->z;
	float len_a = sqrtf(va->x * va->x + va->y * va->y + va->z * va->z);
	float len_b = sqrtf(vb->x * vb->x + vb->y * vb->y + vb->z * vb->z);
	return (dot / (len_a * len_b));
}

///////////////////
void VECTOR4D_Add( VECTOR4D_PTR va, VECTOR4D_PTR vb, VECTOR4D_PTR vsum )
{
	vsum->x = va->x + vb->x;
	vsum->y = va->y + vb->y;
	vsum->z = va->z + vb->z;
	vsum->w = 1.0;
}

VECTOR4D VECTOR4D_Add( VECTOR4D_PTR va, VECTOR4D_PTR vb )
{
	VECTOR4D vresult;
	vresult.x = va->x + vb->x;
	vresult.y = va->y + vb->y;
	vresult.z = va->z + vb->z;
	vresult.w = 1.0;
	return vresult;
}

void VECTOR4D_Sub( VECTOR4D_PTR va, VECTOR4D_PTR vb, VECTOR4D_PTR vdiff )
{
	vdiff->x = va->x - vb->x;
	vdiff->y = va->y - vb->y;
	vdiff->z = va->z - vb->z;
	vdiff->w = 1.0;
}

VECTOR4D VECTOR4D_Sub( VECTOR4D_PTR va, VECTOR4D_PTR vb )
{
	VECTOR4D vresult;
	vresult.x = va->x - vb->x;
	vresult.y = va->y - vb->y;
	vresult.z = va->z - vb->z;
	vresult.w = 1.0;
	return vresult;
}

void VECTOR4D_Scale( float k, VECTOR4D_PTR va )
{
	va->x *= k;
	va->y *= k;
	va->z *= k;
	va->w = 1.0;
}

void VECTOR4D_Scale( float k, VECTOR4D_PTR va, VECTOR4D_PTR vscaled )
{
	vscaled->x = va->x * k;
	vscaled->y = va->y * k;
	vscaled->z = va->z * k;
	vscaled->w = 1.0;
}

float VECTOR4D_Dot( VECTOR4D_PTR va, VECTOR4D_PTR vb )
{
	return (va->x * vb->x + va->y * vb->y + va->z * vb->z);
}

void VECTOR4D_Cross( VECTOR4D_PTR va, VECTOR4D_PTR vb, VECTOR4D_PTR vn )
{
	vn->x = va->y * vb->z - va->z *vb->y;
	vn->y = va->z * vb->x - va->x *vb->z;
	vn->z = va->x * vb->y - va->y *vb->x;
	vn->w = 1.0;
}

VECTOR4D VECTOR4D_Cross( VECTOR4D_PTR va, VECTOR4D_PTR vb )
{
	VECTOR4D vresult;
	vresult.x = va->y * vb->z - va->z *vb->y;
	vresult.y = va->z * vb->x - va->x *vb->z;
	vresult.z = va->x * vb->y - va->y *vb->x;
	vresult.w = 1.0;
	return vresult;
}

float VECTOR4D_Length( VECTOR4D_PTR va )
{
	return sqrtf(va->x * va->x + va->y * va->y + va->z * va->z);
}

float VECTOR4D_Lenght_Fast( VECTOR4D_PTR va )
{
	return Fast_Sqrt(va->x * va->x + va->y * va->y + va->z * va->z);
}

void VECTOR4D_Normalize( VECTOR4D_PTR va )
{
	float len = sqrtf(va->x * va->x + va->y * va->y + va->z * va->z);
	va->x /= len;
	va->y /= len;
	va->z /= len;
	va->w  = 1.0;
}

void VECTOR4D_Normalize( VECTOR4D_PTR va, VECTOR4D_PTR vn )
{
	float len = sqrtf(va->x * va->x + va->y * va->y + va->z * va->z + va->w * va->w);
	vn->x = va->x / len;
	vn->y = va->y / len;
	vn->z = va->z / len;
	vn->w = va->w / len;
}

void VECTOR4D_Build( VECTOR4D_PTR init, VECTOR4D_PTR term, VECTOR4D_PTR result )
{
	result->x = term->x - init->x;
	result->y = term->y - init->y;
	result->z = term->z - init->z;
	result->w = 1.0f;
}

float VECTOR4D_CosTh( VECTOR4D_PTR va, VECTOR4D_PTR vb )
{
	float dot = va->x * vb->x + va->y * vb->y + va->z * vb->z + va->w * vb->w;
	float len_a = sqrtf(va->x * va->x + va->y * va->y + va->z * va->z + va->w * va->w);
	float len_b = sqrtf(vb->x * vb->x + vb->y * vb->y + vb->z * vb->z + vb->w * vb->w);
	return (dot / (len_a * len_b));
}

void Mat_Init_2X2( MATRIX2X2_PTR ma, float m00, float m01, float m10, float m11 )
{
	ma->M00 = m00;
	ma->M01 = m01;
	ma->M10 = m10;
	ma->M11 = m11;
}

float Mat_Det_2X2( MATRIX2X2_PTR ma )
{
	return (ma->M00 * ma->M11 - ma->M01 * ma->M10);
}

void Mat_Add_2X2( MATRIX2X2_PTR ma, MATRIX2X2_PTR mb, MATRIX2X2_PTR msum )
{
	msum->M00 = ma->M00 + mb->M00;
	msum->M01 = ma->M01 + mb->M01;
	msum->M10 = ma->M10 + mb->M10;
	msum->M11 = ma->M11 + mb->M11;
}

void Mat_Mul_2X2( MATRIX2X2_PTR ma, MATRIX2X2_PTR mb, MATRIX2X2_PTR mprod )
{
	//00 01 | 00 01
	//10 11 | 10 11
	mprod->M00 = ma->M00 * mb->M00 + ma->M01 * mb->M10;
	mprod->M01 = ma->M00 * mb->M01 + ma->M01 * mb->M11;
	mprod->M10 = ma->M10 * mb->M00 + ma->M11 * mb->M10;
	mprod->M11 = ma->M10 * mb->M01 + ma->M11 * mb->M11;
}

int Mat_Inverse_2X2( MATRIX2X2_PTR m, MATRIX2X2_PTR mi )
{
	float det = m->M00 * m->M11 - m->M01 * m->M10;

	if(fabs(det) < EPSILON_E5)
		return 0;

	float inv_det = 1.0f / det;
	mi->M00 =  m->M11 * inv_det;
	mi->M01 = -m->M01 * inv_det;
	mi->M10 = -m->M10 * inv_det;
	mi->M11 =  m->M00 * inv_det;

	return 1;
}

int Solve_2X2_System( MATRIX2X2_PTR A, MATRIX1X2_PTR X, MATRIX1X2_PTR B )
{
	float det = Mat_Det_2X2(A);

	if(fabs(det) < EPSILON_E5)
		return 0;

	float inv_det = 1.0f / det;
	MATRIX2X2 work;

	MAT_COPY_2X2(A, &work);
	MAT_COLUMN_SWAP_2X2(&work, 0, B);
	X->M00 = Mat_Det_2X2(&work) * inv_det; 

	MAT_COPY_2X2(A, &work);
	MAT_COLUMN_SWAP_2X2(&work, 1, B);
	X->M01 = Mat_Det_2X2(&work) * inv_det; 

	return 1;
}

void Mat_Mul_1X2_3X2( MATRIX1X2_PTR ma, MATRIX3X2_PTR mb, MATRIX1X2_PTR mprod )
{
	//[M00, M01, 1]	[b00, b01]
	//				[b10, b11]
	//				[b20, b21]
	float sum = 0.0f;
	for(int col = 0; col < 2; ++col)
	{
		sum = 0.0f;
		for(int index = 0; index < 2; ++index)
		{
			sum += ma->M[index] * mb->M[index][col];
		}
		sum += mb->M[2][col];
		mprod->M[col] = sum;
	}
}

void Mat_Mul_1X3_3X3( MATRIX1X3_PTR ma, MATRIX3X3_PTR mb, MATRIX1X3_PTR mprod )
{
	float sum = 0.0f;
	for(int col = 0; col < 3; ++col)
	{
		sum = 0.0f;
		for(int index = 0; index < 3; ++index)
		{
			sum += ma->M[index] * mb->M[index][col];
		}
		mprod->M[col] = sum;
	}
}

void Mat_Mul_3X3( MATRIX3X3_PTR ma, MATRIX3X3_PTR mb, MATRIX3X3_PTR mprod )
{
	float sum = 0.0f;
	for(int col = 0; col < 3; ++col)
	{
		for(int row = 0; row < 3; ++row)
		{
			sum = 0.0f;
			for(int index = 0; index < 3; ++index)
			{
				sum += ma->M[row][index] * mb->M[index][col];
			}
			mprod->M[row][col] = sum;
		}
	}
}

void Mat_Init_3X2( MATRIX3X2_PTR ma, float m00, float m01, float m10, float m11, float m20, float m21 )
{
	ma->M00 = m00; ma->M01 = m01;
	ma->M10 = m10; ma->M11 = m11;
	ma->M20 = m20; ma->M21 = m21;
}

void Mat_Add_3X3( MATRIX3X3_PTR ma, MATRIX3X3_PTR mb, MATRIX3X3_PTR msum )
{
	for(int col = 0; col < 3; ++col)
	{
		for(int row = 0; row < 3; ++row)
			msum->M[row][col] = ma->M[row][col] + mb->M[row][col];
	}
}

void Mat_Mul_VECTOR3D_3X3( VECTOR3D_PTR ma, MATRIX3X3_PTR mb, VECTOR3D_PTR vprod )
{
	//[M00, M01, 1]	[b00, b01, b02]
	//				[b10, b11, b12]
	//				[b20, b21, b22]
	float sum = 0.0f;
	for(int col = 0; col < 3; ++col)
	{
		sum = 0.0f;
		for(int index = 0; index < 3; ++index)
		{
			sum += ma->M[index] * mb->M[index][col];
		}
		vprod->M[col] = sum;
	}
}

int Mat_Inverse_3X3( MATRIX3X3_PTR ma, MATRIX3X3_PTR mi )
{
	float det = Mat_Det_3X3(ma);

	if(fabs(det) < EPSILON_E5)
		return 0;

	float inv_det = 1.0f / det;

	// 00 01 02
	// 10 11 12
	// 20 21 22

	mi->M00 =  inv_det * (ma->M11 * ma->M22 - ma->M21 * ma->M12);
	mi->M10 = -inv_det * (ma->M10 * ma->M22 - ma->M20 * ma->M12);
	mi->M20 =  inv_det * (ma->M10 * ma->M21 - ma->M20 * ma->M11);

	mi->M01 = -inv_det * (ma->M01 * ma->M22 - ma->M21 * ma->M02);
	mi->M11 =  inv_det * (ma->M00 * ma->M22 - ma->M20 * ma->M02);
	mi->M21 = -inv_det * (ma->M00 * ma->M21 - ma->M20 * ma->M01);

	mi->M02 =  inv_det * (ma->M01 * ma->M12 - ma->M11 * ma->M02);
	mi->M12 = -inv_det * (ma->M00 * ma->M12 - ma->M10 * ma->M02);
	mi->M22 =  inv_det * (ma->M00 * ma->M11 - ma->M10 * ma->M01);

	return 1;
}

void Mat_Init_3X3( MATRIX3X3_PTR ma, float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22 )
{
	ma->M00 = m00; ma->M01 = m01; ma->M02 = m02;
	ma->M10 = m10; ma->M11 = m11; ma->M12 = m12;
	ma->M20 = m20; ma->M21 = m21; ma->M22 = m22;
}

float Mat_Det_3X3( MATRIX3X3_PTR m )
{
	return (m->M00 * (m->M11 * m->M22 - m->M21 * m->M12) - 
			m->M01 * (m->M10 * m->M22 - m->M20 * m->M12) + 
			m->M02 * (m->M10 * m->M21 - m->M20 * m->M11));
}

int Solve_3X3_System( MATRIX3X3_PTR A, MATRIX1X3_PTR X, MATRIX1X3_PTR B )
{
	float det = Mat_Det_3X3(A);

	if(fabs(det) < EPSILON_E5)
		return 0;

	float inv_det = 1.0f / det;
	MATRIX3X3 work_mat;

	MAT_COPY_3X3(A, &work_mat);
	MAT_COLUMN_SWAP_3X3(&work_mat, 0, B);
	X->M00 = Mat_Det_3X3(&work_mat) * inv_det;
	
	MAT_COPY_3X3(A, &work_mat);
	MAT_COLUMN_SWAP_3X3(&work_mat, 1, B);
	X->M01 = Mat_Det_3X3(&work_mat) * inv_det;

	MAT_COPY_3X3(A, &work_mat);
	MAT_COLUMN_SWAP_3X3(&work_mat, 2, B);
	X->M02 = Mat_Det_3X3(&work_mat) * inv_det;
	return 1;
}

void Mat_Add_4X4( MATRIX4X4_PTR ma, MATRIX4X4_PTR mb, MATRIX4X4_PTR msum )
{
	for(int row = 0; row < 4; ++row)
	{
		for(int col = 0; col < 4; ++col)
		{
			msum->M[row][col] = ma->M[row][col] + mb->M[row][col];
		}
	}
}

void Mat_Mul_4X4( MATRIX4X4_PTR ma, MATRIX4X4_PTR mb, MATRIX4X4_PTR mprod )
{
	float sum = 0.0f;
	for(int col = 0; col < 4; ++col)
	{
		for(int row = 0; row < 4; ++row)
		{
			sum = 0.0f;
			for(int index = 0; index < 4; ++index)
			{
				sum += ma->M[row][index] * mb->M[index][col];
			}
			mprod->M[row][col] = sum;
		}
	}
}

void Mat_Mul_1X4_4X4( MATRIX1X4_PTR ma, MATRIX4X4_PTR mb, MATRIX1X4_PTR mprod )
{
	float sum = 0.0f;
	for(int col = 0; col < 4; ++col)
	{
		sum = 0.0f;
		for(int row = 0; row < 4; ++row)
		{
			sum += ma->M[row] * mb->M[row][col];
			
		}
		mprod->M[col] = sum;
	}
}

void Mat_Mul_VECTOR3D_4X4( VECTOR3D_PTR va, MATRIX4X4_PTR mb, VECTOR3D_PTR vprod )
{
	// [a00, a01, a02, 1][]
	float sum = 0.0f;
	for(int col = 0; col < 3; ++col)
	{
		sum = 0.0f;
		for(int row = 0; row < 3; ++row)
		{
			sum += va->M[row] * mb->M[row][col];

		}
		sum += mb->M[3][col];
		vprod->M[col] = sum;
	}
}

void Mat_Mul_VECTOR3D_4X3( VECTOR3D_PTR va, MATRIX4X3_PTR mb, VECTOR3D_PTR vprod )
{
	// [a00, a01, a02, 1][]
	float sum = 0.0f;
	for(int col = 0; col < 3; ++col)
	{
		sum = 0.0f;
		for(int row = 0; row < 3; ++row)
		{
			sum += va->M[row] * mb->M[row][col];

		}
		sum += mb->M[3][col];
		vprod->M[col] = sum;
	}
}

void Mat_Mul_VECTOR4D_4X4( VECTOR4D_PTR va, MATRIX4X4_PTR mb, VECTOR4D_PTR vprod )
{
	float sum = 0.0f;
	for(int col = 0; col < 4; ++col)
	{
		sum = 0.0f;
		for(int row = 0; row < 4; ++row)
		{
			sum += va->M[row] * mb->M[row][col];

		}
		vprod->M[col] = sum;
	}
}

void Mat_Mul_VECTOR4D_4X3( VECTOR4D_PTR va, MATRIX4X3_PTR mb, VECTOR4D_PTR vprod )
{
	//[a00, a01, a02, a03]	[b00, b01, b02, 0]
	//						[b00, b01, b02, 0]
	//						[b00, b01, b02, 0]
	//						[b00, b01, b02, 1]
	float sum = 0.0f;
	for(int col = 0; col < 3; ++col)
	{
		sum = 0.0f;
		for(int row = 0; row < 4; ++row)
		{
			sum += va->M[row] * mb->M[row][col];

		}
		vprod->M[col] = sum;
	}
	vprod->M[3] = va->M[3];
}

int Mat_Inverse_4X4( MATRIX4X4_PTR m, MATRIX4X4_PTR mi )
{
	float det =  ( m->M00 * ( m->M11 * m->M22 - m->M12 * m->M21 ) -
		m->M01 * ( m->M10 * m->M22 - m->M12 * m->M20 ) +
		m->M02 * ( m->M10 * m->M21 - m->M11 * m->M20 ) );

	if (fabs(det) < EPSILON_E5)
		return(0);

	float det_inv  = 1.0f / det;

	mi->M00 =  det_inv * ( m->M11 * m->M22 - m->M12 * m->M21 );
	mi->M01 = -det_inv * ( m->M01 * m->M22 - m->M02 * m->M21 );
	mi->M02 =  det_inv * ( m->M01 * m->M12 - m->M02 * m->M11 );
	mi->M03 = 0.0f; 

	mi->M10 = -det_inv * ( m->M10 * m->M22 - m->M12 * m->M20 );
	mi->M11 =  det_inv * ( m->M00 * m->M22 - m->M02 * m->M20 );
	mi->M12 = -det_inv * ( m->M00 * m->M12 - m->M02 * m->M10 );
	mi->M13 = 0.0f; 

	mi->M20 =  det_inv * ( m->M10 * m->M21 - m->M11 * m->M20 );
	mi->M21 = -det_inv * ( m->M00 * m->M21 - m->M01 * m->M20 );
	mi->M22 =  det_inv * ( m->M00 * m->M11 - m->M01 * m->M10 );
	mi->M23 = 0.0f; 

	mi->M30 = -( m->M30 * mi->M00 + m->M31 * mi->M10 + m->M32 * mi->M20 );
	mi->M31 = -( m->M30 * mi->M01 + m->M31 * mi->M11 + m->M32 * mi->M21 );
	mi->M32 = -( m->M30 * mi->M02 + m->M31 * mi->M12 + m->M32 * mi->M22 );
	mi->M33 = 1.0f; 

	return(1);
}

void Mat_Init_4X4( MATRIX4X4_PTR ma, float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33 )
{
	ma->M00 = m00; ma->M01 = m01; ma->M02 = m02; ma->M03 = m03;
	ma->M10 = m10; ma->M11 = m11; ma->M12 = m12; ma->M13 = m13;
	ma->M20 = m20; ma->M21 = m21; ma->M22 = m22; ma->M23 = m23;
	ma->M30 = m30; ma->M31 = m31; ma->M32 = m32; ma->M33 = m33;
}

void QUAT_Add( QUAT_PTR q1, QUAT_PTR q2, QUAT_PTR qsum )
{
	qsum->x = q1->x + q2->x;
	qsum->y = q1->y + q2->y;
	qsum->z = q1->z + q2->z;
	qsum->w = q1->w + q2->w;
}

void QUAT_Sub( QUAT_PTR q1, QUAT_PTR q2, QUAT_PTR qdiff )
{
	qdiff->x = q1->x - q2->x;
	qdiff->y = q1->y - q2->y;
	qdiff->z = q1->z - q2->z;
	qdiff->w = q1->w - q2->w;
}

void QUAT_Conjugate( QUAT_PTR q, QUAT_PTR qconj )
{
	qconj->x = -q->x;
	qconj->y = -q->y;
	qconj->z = -q->z;
	qconj->w =  q->w;
}

void QUAT_Scale( QUAT_PTR q, float scale, QUAT_PTR qs )
{
	qs->x = q->x * scale;
	qs->y = q->y * scale;
	qs->z = q->z * scale;
	qs->w = q->w * scale;
}

void QUAT_Scale( QUAT_PTR q, float scale )
{
	q->x = q->x * scale;
	q->y = q->y * scale;
	q->z = q->z * scale;
	q->w = q->w * scale;
}

float QUAT_Norm( QUAT_PTR q )
{
	return (sqrtf(q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z));
}

float QUAT_Norm2( QUAT_PTR q )
{
	return (q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z);
}

void QUAT_Normalize( QUAT_PTR q, QUAT_PTR qn )
{
	float len_inv = 1.0f / (sqrtf(q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z));

	qn->x = q->x * len_inv;
	qn->y = q->y * len_inv;
	qn->z = q->z * len_inv;
	qn->w = q->w * len_inv;
}

void QUAT_Normalize( QUAT_PTR q )
{
	float len_inv = 1.0 / (sqrtf(q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z));

	q->x = q->x * len_inv;
	q->y = q->y * len_inv;
	q->z = q->z * len_inv;
	q->w = q->w * len_inv;
}

void QUAT_Unit_Inverse( QUAT_PTR q, QUAT_PTR qi )
{
	//q-1=q*/|q|2
	float norm2_inv = 1.0 / (q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z);

	qi->w =  q->w * norm2_inv;
	qi->x = -q->x * norm2_inv;
	qi->y = -q->y * norm2_inv;
	qi->z = -q->z * norm2_inv;
}

void QUAT_Unit_Inverse( QUAT_PTR q )
{
	float norm2_inv = 1.0 / (q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z);

	q->w =  q->w * norm2_inv;
	q->x = -q->x * norm2_inv;
	q->y = -q->y * norm2_inv;
	q->z = -q->z * norm2_inv;
}

void QUAT_Mul( QUAT_PTR q1, QUAT_PTR q2, QUAT_PTR qprod )
{
	float prd_0 = (q1->z - q1->y) * (q2->y - q2->z);
	float prd_1 = (q1->w + q1->x) * (q2->w + q2->x);
	float prd_2 = (q1->w - q1->x) * (q2->y + q2->z);
	float prd_3 = (q1->y + q1->z) * (q2->w - q2->x);
	float prd_4 = (q1->z - q1->x) * (q2->x - q2->y);
	float prd_5 = (q1->z + q1->x) * (q2->x + q2->y);
	float prd_6 = (q1->w + q1->y) * (q2->w - q2->z);
	float prd_7 = (q1->w - q1->y) * (q2->w + q2->z);

	float prd_8 = prd_5 + prd_6 + prd_7;
	float prd_9 = 0.5 * (prd_4 + prd_8);

	qprod->w = prd_0 + prd_9 - prd_5;
	qprod->x = prd_1 + prd_9 - prd_8;
	qprod->y = prd_2 + prd_9 - prd_7;
	qprod->z = prd_3 + prd_9 - prd_6;
}

void QUAT_Triple_Product( QUAT_PTR q1, QUAT_PTR q2, QUAT_PTR q3, QUAT_PTR qprod )
{
	QUAT qtmp;
	QUAT_Mul(q1, q2, &qtmp);
	QUAT_Mul(&qtmp, q3, qprod);
}

void VECTOR3D_Theta_To_QUAT( QUAT_PTR q, VECTOR3D_PTR v, float theta )
{
	float theta_div_2 = 0.5 * theta;

	float sin_theta = sinf(theta_div_2);

	q->x = v->x * sin_theta;
	q->y = v->y * sin_theta;
	q->z = v->z * sin_theta;
	q->w = cosf(theta_div_2);
}

void VECTOR4D_Theta_To_QUAT( QUAT_PTR q, VECTOR4D_PTR v, float theta )
{
	float theta_div_2 = 0.5 * theta;

	float sin_theta = sinf(theta_div_2);

	q->x = v->x * sin_theta;
	q->y = v->y * sin_theta;
	q->z = v->z * sin_theta;
	q->w = cosf(theta_div_2);
}

void EulerZYX_To_QUAT( QUAT_PTR q, float theta_z, float theta_y, float theta_x )
{

}

void QUAT_To_VECTOR3D_Theta( QUAT_PTR q, VECTOR3D_PTR v, float *theta )
{
	*theta = acosf(q->w);
	float sin_theta_inv = 1.0 / sinf(*theta);

	v->x = q->x * sin_theta_inv;
	v->y = q->y * sin_theta_inv;
	v->z = q->z * sin_theta_inv;

	*theta *= 2;
}

void Init_Parm_Line2D( POINT2D_PTR p_init, POINT2D_PTR p_term, PARMLINE2D_PTR p )
{
	VECTOR2D_INIT(&(p->p0), p_init);
	VECTOR2D_INIT(&(p->p1), p_term);
	VECTOR2D_Build(p_init, p_term, &(p->v));
}

void Compute_Parm_Lines2D( PARMLINE2D_PTR p, float t, POINT2D_PTR pt )
{
	pt->x = p->p0.x + p->v.x * t;
	pt->y = p->p0.y + p->v.y * t;
}

ePARM_LINE_INTERSECTION_TYPE Intersect_Parm_Lines2D( PARMLINE2D_PTR p1, PARMLINE2D_PTR p2, float *t1, float *t2 )
{
	float det_p1p2 = (p1->v.x*p2->v.y - p1->v.y*p2->v.x);
	if (fabs(det_p1p2) <= EPSILON_E5)
	{
		return(kPARM_LINE_INTERSECTION_TYPE_NONE);

	}

	*t1 = (p2->v.x*(p1->p0.y - p2->p0.y) - p2->v.y*(p1->p0.x - p2->p0.x)) 
		/det_p1p2;

	*t2 = (p1->v.x*(p1->p0.y - p2->p0.y) - p1->v.y*(p1->p0.x - p2->p0.x))
		/det_p1p2;

	if ((*t1>=0) && (*t1<=1) && (*t2>=0) && (*t2<=1))
		return(kPARM_LINE_INTERSECTION_TYPE_IN);
	else
		return(kPARM_LINE_INTERSECTION_TYPE_OUT);
}

ePARM_LINE_INTERSECTION_TYPE Intersect_Parm_Lines2D( PARMLINE2D_PTR p1, PARMLINE2D_PTR p2, POINT2D_PTR pt )
{
	float det_p1p2 = (p1->v.x*p2->v.y - p1->v.y*p2->v.x);
	if (fabs(det_p1p2) <= EPSILON_E5)
	{
		return(kPARM_LINE_INTERSECTION_TYPE_NONE);
	}

	float t1 = (p2->v.x*(p1->p0.y - p2->p0.y) - p2->v.y*(p1->p0.x - p2->p0.x)) 
		/det_p1p2;

	float t2 = (p1->v.x*(p1->p0.y - p2->p0.y) - p1->v.y*(p1->p0.x - p2->p0.x))
		/det_p1p2;

	pt->x = p1->p0.x + p1->v.x * t1;
	pt->y = p1->p0.y + p1->v.y * t1;

	if ((t1>=0) && (t1<=1) && (t2>=0) && (t2<=1))
		return(kPARM_LINE_INTERSECTION_TYPE_IN);
	else
		return(kPARM_LINE_INTERSECTION_TYPE_OUT);
}

void Init_Parm_Line3D( POINT3D_PTR p_init, POINT3D_PTR p_term, PARMLINE3D_PTR p )
{
	VECTOR3D_INIT(&(p->p0), p_init);
	VECTOR3D_INIT(&(p->p1), p_term);
	VECTOR3D_Build(p_init, p_term, &(p->v));
}

void Compute_Parm_Lines3D( PARMLINE3D_PTR p, float t, POINT3D_PTR pt )
{
	pt->x = p->p0.x + p->v.x * t;
	pt->y = p->p0.y + p->v.y * t;
	pt->z = p->p0.z + p->v.z * t;
}

void PLANE3D_Init( PLANE3D_PTR plane, POINT3D_PTR p0, VECTOR3D_PTR normal, int normalize )
{
	POINT3D_COPY(&(plane->p0), p0);
	
	if(normalize)
	{
		POINT3D_COPY(&(plane->n), normal);
	}
	else
	{
		VECTOR3D_Normalize(&(plane->n), normal);
	}
}

float Compute_Point_In_Plane3D( POINT3D_PTR p0, PLANE3D_PTR plane )
{
	float hs =	plane->n.x*(p0->x - plane->p0.x) + 
				plane->n.y*(p0->y - plane->p0.y) +
				plane->n.z*(p0->z - plane->p0.z); 

	return(hs);
}

int Intersect_Parm_Line3D_Plane3D( PARMLINE3D_PTR pline, PLANE3D_PTR plane, float *t, POINT3D_PTR pt )
{
	float plane_dot_line = VECTOR3D_Dot(&pline->v, &plane->n);

	if (fabs(plane_dot_line) <= EPSILON_E5)
	{
		if (fabs(Compute_Point_In_Plane3D(&pline->p0, plane)) <= EPSILON_E5)
			return(kPARM_LINE_INTERSECTION_TYPE_EVERYWHERE);
		else
			return(kPARM_LINE_INTERSECTION_TYPE_NONE);
	}

	*t = -(plane->n.x*pline->p0.x + 
		plane->n.y*pline->p0.y + 
		plane->n.z*pline->p0.z -
		plane->n.x*plane->p0.x - 
		plane->n.y*plane->p0.y - 
		plane->n.z*plane->p0.z) / (plane_dot_line);

	// now plug t into the parametric line and solve for x,y,z
	pt->x = pline->p0.x + pline->v.x*(*t);
	pt->y = pline->p0.y + pline->v.y*(*t);
	pt->z = pline->p0.z + pline->v.z*(*t);

	// test interval of t
	if (*t>=0.0 && *t<=1.0)
		return(kPARM_LINE_INTERSECTION_TYPE_IN);
	else
		return(kPARM_LINE_INTERSECTION_TYPE_OUT);
}

void Init_Point_Noraml_Line2D( POINT2D_PTR p0, VECTOR2D_PTR n, POINTNORMALLINE2D_PTR line )
{
  POINT2D_INIT(&line->p0, p0);
  VECTOR2D_INIT(&line->n, n);
  VECTOR2D_Normalize(&line->n);
}

void Init_Point_Noraml_Line2D_With_Point( POINT2D_PTR p0, POINT2D_PTR p1, POINTNORMALLINE2D_PTR line )
{
  POINT2D_INIT(&line->p0, p0);
  VECTOR2D n;
  VECTOR2D_Build(p0, p1, &n);
  VECTOR2D_Normalize(&n);
  float t = n.x;
  n.x = -n.y;
  n.y = n.x;
  VECTOR2D_INIT(&line->n, &n);
}

float Distance_Point2D_Point_Normal_Line2D( POINT2D_PTR p, POINTNORMALLINE2D_PTR line )
{
  return fabsf(line->n.x * (p->x - line->p0.x) + line->n.y * (p->y - line->p0.y));
}

float CalTriangleAreaByPoint( float x0, float y0, float x1, float y1, float x2, float y2 )
{
  /*
  POINT2D p0, p1, p2;
  p0.x = x0; p0.y = y0;
  p1.x = x1; p1.y = y1;
  p2.x = x2; p2.y = y2;
  POINTNORMALLINE2D line;
  Init_Point_Noraml_Line2D_With_Point(&p0, &p1, &line);
  float h = Distance_Point2D_Point_Normal_Line2D(&p2, &line);
  VECTOR2D_Sub(&p0, &p1, &p2);
  return (h * VECTOR2D_Lenght_Fast(&p2) * 0.5);
  */
  POINT2D p0, p1, p2, pt;
  p0.x = x0; p0.y = y0;
  p1.x = x1; p1.y = y1;
  p2.x = x2; p2.y = y2;
  VECTOR2D_Sub(&p1, &p0, &pt);
  float a = VECTOR2D_Lenght_Fast(&pt);
  VECTOR2D_Sub(&p2, &p0, &pt);
  float b = VECTOR2D_Lenght_Fast(&pt);
  VECTOR2D_Sub(&p1, &p2, &pt);
  float c = VECTOR2D_Lenght_Fast(&pt);
  float p = (a + b + c) / 2.0f;
  return Fast_Sqrt(p * (p - a) * (p - b) * (p - c));
}



