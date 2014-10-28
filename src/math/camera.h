#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "math/math_3d.h"

class Camera
{
public:
	Camera();
	~Camera();

	void	LookAt(	float eyex, float eyey, float eyez, 
					      float upx, float upy, float upz, 
					      float px, float py, float pz);
  void 	PerspectiveProj(float xleft, float xright, float ytop, float ybuttom, float znear, float zfar);
	void	PerspectiveProj(float angle, float aspect, float znear, float zfar);
	void 	OrthogonalProj(float xleft, float xright, float ytop, float ybuttom, float znear, float zfar);

  void          ResetViewMat();
  void          ResetProjMat();
	MATRIX4X4_PTR	GetViewMat(){return &m_mat_view_;}
  MATRIX4X4_PTR	GetProjMat(){return &m_mat_proj_;}
	MATRIX4X4_PTR GetViewProjMat(){return &m_mat_view_proj_;}

  void          MoveCameraDeltaXY(float x, float y);

private:
	MATRIX4X4		m_mat_view_;
	MATRIX4X4		m_mat_proj_;
	MATRIX4X4   m_mat_view_proj_;
  POINT3D     m_point_eye_;
  POINT3D     m_point_view_;
  VECTOR3D    m_vec_up_;
  float       m_fznear_;
  float       m_fangle_2_;
};

#endif