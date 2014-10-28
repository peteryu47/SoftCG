#include "camera.h"
#include "math_3d.h"
#include <math.h>

Camera::Camera()
{
  ResetProjMat();
  ResetViewMat();
}

Camera::~Camera()
{

}

void Camera::LookAt( float eyex, float eyey, float eyez, float upx, float upy, float upz, float px, float py, float pz )
{
  VECTOR3D u;
  VECTOR3D v = {upx, upy, upz};
  VECTOR3D n = {px - eyex, py - eyey, pz - eyez};
  VECTOR3D p = {eyex, eyey, eyez};

  VECTOR3D_Normalize(&v);
  VECTOR3D_Normalize(&n);

  VECTOR3D_Cross(&v, &n, &u);
  VECTOR3D_Normalize(&u);
  VECTOR3D_Cross(&n, &u, &v);
  VECTOR3D_Normalize(&v);
  
  m_mat_view_.M00 = u.x; m_mat_view_.M01 = v.x; m_mat_view_.M02 = n.x; m_mat_view_.M03 = 0;
  m_mat_view_.M10 = u.y; m_mat_view_.M11 = v.y; m_mat_view_.M12 = n.y; m_mat_view_.M13 = 0;
  m_mat_view_.M20 = u.z; m_mat_view_.M21 = v.z; m_mat_view_.M22 = n.z; m_mat_view_.M23 = 0;
  m_mat_view_.M30 = - VECTOR3D_Dot(&u, &p); 
  m_mat_view_.M31 = - VECTOR3D_Dot(&v, &p); 
  m_mat_view_.M32 = - VECTOR3D_Dot(&n, &p); 
  m_mat_view_.M33 = 1;

  Mat_Mul_4X4(&m_mat_view_, &m_mat_proj_, &m_mat_view_proj_);

  POINT3D_COPY(&m_point_eye_, &p);
  m_point_view_.x = px; m_point_view_.y = py; m_point_view_.z = pz;
  VECTOR3D_COPY(&m_vec_up_, &v);
}

void Camera::PerspectiveProj(float xleft, float xright, float ytop, float ybuttom, float znear, float zfar)
{
  m_mat_proj_.M00 = 2.0f * znear / (xright - xleft); m_mat_proj_.M01 = 0; m_mat_proj_.M02 = 0; m_mat_proj_.M03 = 0;
  m_mat_proj_.M10 = 0; m_mat_proj_.M11 = 2.0f * znear / (ytop - ybuttom); m_mat_proj_.M12 = 0; m_mat_proj_.M13 = 0;
  m_mat_proj_.M20 = 0; m_mat_proj_.M21 = 0; m_mat_proj_.M22 = 1.0f / (zfar - znear); m_mat_proj_.M23 = 1;
  m_mat_proj_.M30 = - (xright + xleft) / (xright - xleft); m_mat_proj_.M31 = - (ytop + ybuttom) / (ytop - ybuttom); m_mat_proj_.M32 = - znear / (zfar - znear); m_mat_proj_.M33 = 0;
  Mat_Mul_4X4(&m_mat_view_, &m_mat_proj_, &m_mat_view_proj_);

  m_fznear_ = znear;
  m_fangle_2_ = atanf((ytop - ybuttom) / (xright - xright));
}

void Camera::PerspectiveProj( float angle, float aspect, float znear, float zfar )
{
  float cot_a_2 = 1.0f / tanf(A2R(angle / 2));
  m_mat_proj_.M00 = 1.0f / aspect * cot_a_2; m_mat_proj_.M01 = 0; m_mat_proj_.M02 = 0; m_mat_proj_.M03 = 0;
  m_mat_proj_.M10 = 0; m_mat_proj_.M11 = cot_a_2; m_mat_proj_.M12 = 0; m_mat_proj_.M13 = 0;
  m_mat_proj_.M20 = 0; m_mat_proj_.M21 = 0; m_mat_proj_.M22 = 1.0f / (zfar - znear); m_mat_proj_.M23 = 1;
  m_mat_proj_.M30 = 0; m_mat_proj_.M31 = 0; m_mat_proj_.M32 = - znear / (zfar - znear); m_mat_proj_.M33 = 0;
  Mat_Mul_4X4(&m_mat_view_, &m_mat_proj_, &m_mat_view_proj_);

  m_fznear_ = znear;
  m_fangle_2_ = angle / 2;
}

void Camera::OrthogonalProj( float xleft, float xright, float ytop, float ybuttom, float znear, float zfar )
{
  m_mat_proj_.M00 = 2.0f / (xright - xleft); m_mat_proj_.M01 = 0; m_mat_proj_.M02 = 0; m_mat_proj_.M03 = 0;
  m_mat_proj_.M10 = 0; m_mat_proj_.M11 = 2.0f / (ytop - ybuttom); m_mat_proj_.M12 = 0; m_mat_proj_.M13 = 0;
  m_mat_proj_.M20 = 0; m_mat_proj_.M21 = 0; m_mat_proj_.M22 = 1.0f / (zfar - znear); m_mat_proj_.M23 = 0;
  m_mat_proj_.M30 = - (xright + xleft) / (xright - xleft); m_mat_proj_.M31 = - (ytop + ybuttom) / (ytop - ybuttom); m_mat_proj_.M32 = - znear / (zfar - znear); m_mat_proj_.M33 = 1;
  Mat_Mul_4X4(&m_mat_view_, &m_mat_proj_, &m_mat_view_proj_);
}

void Camera::ResetViewMat()
{
  MAT_IDENTITY_4X4(&m_mat_view_);
}

void Camera::ResetProjMat()
{
  MAT_IDENTITY_4X4(&m_mat_proj_);
}

void Camera::MoveCameraDeltaXY( float x, float y )
{
  VECTOR3D n;
  VECTOR3D_Sub(&m_point_view_, &m_point_eye_, &n);
  VECTOR3D_Normalize(&n);

  VECTOR3D u;
  VECTOR3D_Cross(&m_vec_up_, &n, &u);
  VECTOR3D_Normalize(&u);
}
