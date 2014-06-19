#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "math_3d.h"

class Camera
{
public:
	Camera();
	~Camera();

	void	LookAt(float eyex, float eyey, float eyez, float upx, float upy, float upz, float px, float py, float pz);
	void	PerspectiveProj(float angle, float aspect, float zscreen, float znear, float zfar);
	void 	OrthogonalProjection(float xleft, float xright, float z);

private:
	MATRIX4X4_PTR		m_mat_model_;
	MATRIX4X4_PTR		m_mat_view_;
	MATRIX4X4_PTR		m_mat_projection_;
};

#endif