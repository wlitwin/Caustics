#include "light.hpp"

#include <glm/gtc/matrix_transform.hpp>

Light::Light() :
	m_pos(0.0f)
{

}

void Light::SetPosition(const glm::vec3& pos)
{
	m_pos = pos;
}

glm::vec3 Light::GetPosition() const
{
	return m_pos;
}

glm::mat4 Light::GetView() const
{
	return glm::translate(glm::mat4(1.0f), -m_pos);	
}

