#include "mesh.hpp"

#include <cassert>

//=============================================================================
// Constructor
//=============================================================================

Mesh::Mesh() : 
	m_vao(0), 
	m_vbo(0)
{ }

//=============================================================================
// Destructor
//=============================================================================

Mesh::~Mesh() 
{
	cleanup();
}

//=============================================================================
// cleanup 
//=============================================================================

void Mesh::cleanup()
{
	if (m_vao != 0)
	{
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0;
	}

	if (m_vbo != 0)
	{
		glDeleteBuffers(1, &m_vbo);
		m_vbo = 0;
	}

	mesh.clear();
}

//=============================================================================
// NewMesh
//=============================================================================

void Mesh::NewMesh()
{
	cleanup();	
}

//=============================================================================
// NewMesh
//=============================================================================

void Mesh::AddTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
	assert(m_vao == 0);
	assert(m_vbo == 0);

	// Calculate the triangles normal vector
	const glm::vec3 e1(p2 - p1);
	const glm::vec3 e2(p3 - p1);
	const glm::vec3 normal(glm::normalize(glm::cross(e1, e2)));

	// Add the triangle and normals to the buffer
	mesh.push_back(p1);
	mesh.push_back(normal);

	mesh.push_back(p2);
	mesh.push_back(normal);

	mesh.push_back(p3);
	mesh.push_back(normal);
}

//=============================================================================
// NewMesh
//=============================================================================

void Mesh::AddQuad(const glm::vec3& p1, const glm::vec3& p2, 
				   const glm::vec3& p3, const glm::vec3& p4)
{
	AddTriangle(p1, p2, p4);
	AddTriangle(p2, p3, p4);
}

//=============================================================================
// Finish
//=============================================================================

void Mesh::Finish()
{
	assert(m_vao == 0);
	assert(m_vbo == 0);
	assert(mesh.size() != 0);

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	assert(m_vao != 0);
	assert(m_vbo != 0);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	size_t size = mesh.size() * sizeof(glm::vec3);
	glBufferData(GL_ARRAY_BUFFER, size, &mesh[0], GL_STATIC_DRAW);
	// Vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3), 0);
	// Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3), (void*)sizeof(glm::vec3));

	// These are on a per vertex array basis
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// Unbind TODO - rebind what was previously bound?
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//=============================================================================
// NewMesh
//=============================================================================

void Mesh::Render()
{
	assert(m_vao != 0);
	assert(m_vbo != 0);

	glBindVertexArray(m_vao);

//	glEnableVertexAttribArray(0);
//	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, mesh.size());

//	glDisableVertexAttribArray(1);
//	glDisableVertexAttribArray(0);

	glBindVertexArray(0);
}

//=============================================================================
// 
//=============================================================================
