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

	void AddTriangle(const glm::vec3& p1, const glm::vec2& t1,
					 const glm::vec3& p2, const glm::vec2& t2,
					 const glm::vec3& p3, const glm::vec2& t3);

	void AddQuad(const glm::vec3& p1, const glm::vec3& p2, 
				 const glm::vec3& p3, const glm::vec3& p4);

	void AddQuad(const glm::vec3& p1, const glm::vec2& t1,
				 const glm::vec3& p2, const glm::vec2& t2,
				 const glm::vec3& p3, const glm::vec2& t3,
				 const glm::vec3& p4, const glm::vec2& t4);

	void Finish();

	void Render();

private:
	void cleanup();
	
private:
	GLuint m_vao;
	GLuint m_vbo;
	std::vector<float> m_mesh;
};

#endif
