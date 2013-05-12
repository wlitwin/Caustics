#ifndef __LIGHT_HPP__
#define __LIGHT_HPP__

#include <glm/glm.hpp>

class Light
{
public:
	Light();

	void SetPosition(const glm::vec3& pos);	
	
	glm::vec3 GetPosition() const;

	glm::mat4 GetView() const;

private:
	glm::vec3 m_pos;
};

#endif
