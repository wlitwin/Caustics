#include "camera.hpp"

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

const float PIOVER180 = 3.1415926f / 180.0f;

//=============================================================================
// Constructor
//=============================================================================

Camera::Camera(const float fov, const float aspect_ratio,
				const float near, const float far) :
	m_pos(0.0f),
	m_rot(glm::vec3(0.0f)),
	m_up(0.0f, 1.0f, 0.0f)
{
	SetPerspective(fov, aspect_ratio, near, far);
}

//=============================================================================
// SetPos
//=============================================================================

void Camera::SetPos(const glm::vec3& pos)
{
	m_pos = pos;
}

//=============================================================================
// LookAt
//=============================================================================

void Camera::LookAt(const glm::vec3& eye, const glm::vec3& center, 
					const glm::vec3& up)
{
	m_pos = eye;
	m_up = glm::normalize(up);

	// Easiest way is to just create a lookAt matrix and then convert
	// the rotational part to a quaternion.
	const glm::mat4 look_at = glm::lookAt(eye, center, m_up);
	m_rot = glm::toQuat(look_at);
}

//=============================================================================
// MoveForward
//=============================================================================

void Camera::MoveForward(const float amount)
{
	m_pos -= glm::vec3(0.0f, 0.0f, amount) * m_rot;
}

//=============================================================================
// Strafe
//=============================================================================

void Camera::Strafe(const float amount)
{
	m_pos += glm::vec3(amount, 0.0f, 0.0f) * m_rot;
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
	glm::fquat q_yaw(glm::gtx::quaternion::angleAxis(yaw, 0.f, 1.f, 0.f));
	glm::fquat q_pitch(glm::gtx::quaternion::angleAxis(pitch, 1.f, 0.f, 0.f));
	glm::fquat q_roll(glm::gtx::quaternion::angleAxis(roll, 0.f, 0.f, -1.f));

	if (roll == 0.0f)
	{
		// TODO - Fix rolling problem!
		// We need to apply a corrective factor
	}

	glm::fquat q_t= q_roll * q_pitch * q_yaw;
	m_rot = q_t * m_rot;
	m_rot = glm::normalize(m_rot);
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
// GetPosition
//=============================================================================

glm::vec3 Camera::GetPosition() const
{
	return m_pos;
}

//=============================================================================
// GetView
//=============================================================================

const glm::mat4& Camera::GetView()
{
	const glm::mat4 trans = glm::translate(glm::mat4(1.0f), -m_pos);
	const glm::mat4 view = glm::gtx::quaternion::toMat4(m_rot);
	m_view = view * trans;

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
