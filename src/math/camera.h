#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "math/math_3d.h"

class Camera
{
public:
	Camera();
	~Camera();

	void	LookAt( float eyex, float eyey, float eyez, 
                float upx, float upy, float upz, 
                float px, float py, float pz);
	void	PerspectiveProj(float angle, float aspect, float znear, float zfar);
	void 	OrthogonalProj(float xleft, float xright, float z);

	MATRIX4X4_PTR	GetViewMat();
	MATRIX4X4_PTR	GetProjMat();
  MATRIX4X4_PTR GetViewProjMat();

private:
	MATRIX4X4		m_mat_view_;
	MATRIX4X4		m_mat_proj_;
  MATRIX4X4   m_mat_view_proj_;
};

#endif