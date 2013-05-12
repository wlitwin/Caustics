#ifndef _WAVE_FRONT_HPP_
#define _WAVE_FRONT_HPP_

#include <string>
#include <vector>
#include <glm/glm.hpp>

class WavefrontLoader
{
	WavefrontLoader();
	WavefrontLoader(const std::string& file);

	bool LoadFile(const std::string& file);

	bool IsLoaded() const;

private:
	bool is_loaded;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> tex_coords;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> faces;
};

#endif
