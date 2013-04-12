#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

//=============================================================================
// Constructor
//=============================================================================

Camera::Camera(const float fov, const float aspect_ratio,
				const float near, const float far) :
	m_view(1.0f)
{
	SetPerspective(fov, aspect_ratio, near, far);
}

//=============================================================================
// SetPos
//=============================================================================

void Camera::SetPos(const glm::vec3& pos)
{
	m_view = glm::translate(glm::mat4(1.0),	pos);
}

//=============================================================================
// LookAt
//=============================================================================

void Camera::LookAt(const glm::vec3& eye, const glm::vec3& center, 
					const glm::vec3& up)
{
	m_view = glm::lookAt(eye, center, up);
}

//=============================================================================
// MoveForward
//=============================================================================

void Camera::MoveForward(const float amount)
{

}

//=============================================================================
// Strafe
//=============================================================================

void Camera::Strafe(const float amount)
{
}

//=============================================================================
// Roll
//=============================================================================

void Camera::Roll(const float amount)
{
	Rotate(0, 0, amount);
}

//=============================================================================
// Yaw
//=============================================================================

void Camera::Yaw(const float amount)
{
	Rotate(amount, 0, 0);
}

//=============================================================================
// Pitch
//=============================================================================

void Camera::Pitch(const float amount)
{
	Rotate(0, amount, 0);
}

//=============================================================================
// Rotate
//=============================================================================

void Camera::Rotate(const float yaw, const float pitch, const float roll)
{
}

//=============================================================================
// SetPerspective
//=============================================================================

void Camera::SetPerspective(const float fov, const float aspect_ratio, 
		const float near, const float far)
{
	m_proj = glm::perspective(fov, aspect_ratio, near, far);
}

//=============================================================================
// GetView
//=============================================================================

const glm::mat4& Camera::GetView() const
{
	return m_view;
}

//=============================================================================
// GetProj
//=============================================================================

const glm::mat4& Camera::GetProj() const
{
	return m_proj;
}

//=============================================================================
// 
//=============================================================================