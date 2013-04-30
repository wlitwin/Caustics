#ifndef __MESH_HPP__
#define __MESH_HPP__

#include <vector>
#include <GL/gl.h>
#include <glm/glm.hpp>

class Mesh
{
public:
	Mesh();

	~Mesh();

	void NewMesh();

	void AddTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);

	void AddQuad(const glm::vec3& p1, const glm::vec3& p2, 
				 const glm::vec3& p3, const glm::vec3& p4);

	void Finish();

	void Render();

private:
	void cleanup();
	
private:
	GLuint m_vao;
	GLuint m_vbo;
	std::vector<glm::vec3> mesh;	
};

#endif
