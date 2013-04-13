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
	// Code adapted from http://www.arcsynthesis.org/gltut/Positioning/Tutorial%2008.html

	const float radians = amount * PIOVER180;
	glm::vec3 axis(0.0f, 0.0f, 1.0f);

	axis = axis * sinf(radians / 2.0f);
	const float scalar = cosf(radians / 2.0f);

	const glm::fquat offset(scalar, axis.x, axis.y, axis.z);

	m_rot = offset * m_rot;
	m_up = m_up * offset;
	m_rot = glm::normalize(m_rot);
}

//=============================================================================
// Yaw
//=============================================================================

void Camera::Yaw(const float amount)
{
	// We keep a special up vector because if you keep
	// doing Yaw -> Pitch -> Yaw -> Pitch the whole
	// scene will Roll. This is because the Yaw is around
	// the axis of a sphere. By maintaining a world based
	// up vector we can avoid this rolling and only rotate
	// the up vector when we actually roll. I'll try to
	// come up with a better explanation of what's happening.

	const float radians = amount * PIOVER180;
	glm::vec3 axis(m_up);//0.0f, 1.0f, 0.0f);

	axis = axis * sinf(radians / 2.0f);
	const float scalar = cosf(radians / 2.0f);

	const glm::fquat offset(scalar, axis.x, axis.y, axis.z);

	m_rot = m_rot * offset;//offset * m_rot;
	m_rot = glm::normalize(m_rot);
}

//=============================================================================
// Pitch
//=============================================================================

void Camera::Pitch(const float amount)
{
	const float radians = amount * PIOVER180;
	glm::vec3 axis(1.0f, 0.0f, 0.0f);

	axis = axis * sinf(radians / 2.0f);
	const float scalar = cosf(radians / 2.0f);

	const glm::fquat offset(scalar, axis.x, axis.y, axis.z);

	m_rot = offset * m_rot;
	m_rot = glm::normalize(m_rot);
}

//=============================================================================
// Rotate
//=============================================================================

void Camera::Rotate(const float yaw, const float pitch, const float roll)
{
	if (yaw != 0.0f) 
	{
		Yaw(yaw);
	}

	if (pitch != 0.0f)
	{
		Pitch(pitch);
	}


	if (roll != 0.0f)
	{
		Roll(roll);
	}
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
