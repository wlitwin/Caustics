#include "mesh.hpp"

#include <cassert>

//=============================================================================
// Constructor
//=============================================================================

Mesh::Mesh() : 
	m_primitiveType(GL_TRIANGLES),
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

	m_mesh.clear();
}

//=============================================================================
// NewMesh
//=============================================================================

void Mesh::NewMesh()
{
	cleanup();	
}

//=============================================================================
// calcNormal(vector<float>, vec3)
//=============================================================================

static
void addToVector(std::vector<float>& vec, const glm::vec3& p)
{
	vec.push_back(p.x);
	vec.push_back(p.y);
	vec.push_back(p.z);
}

//=============================================================================
// addToVector(vector<float>, vec2)
//=============================================================================

static
void addToVector(std::vector<float>& vec, const glm::vec2& p)
{
	vec.push_back(p.x);
	vec.push_back(p.y);
}

//=============================================================================
// calcNormal
//=============================================================================

static 
glm::vec3 calcNormal(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
	// Calculate the triangles normal vector
	const glm::vec3 e1(p2 - p1);
	const glm::vec3 e2(p3 - p1);
	return glm::normalize(glm::cross(e1, e2));
}

//=============================================================================
// AddTriangle(vec3, vec2, vec3, vec2, vec3, vec2)
//=============================================================================
	
void Mesh::AddTriangle(const glm::vec3& p1, const glm::vec2& t1,
			  		   const glm::vec3& p2, const glm::vec2& t2,
					   const glm::vec3& p3, const glm::vec2& t3)
{
	assert(m_vao == 0);
	assert(m_vbo == 0);

	const glm::vec3 normal(calcNormal(p1, p2, p3));

	addToVector(m_mesh, p1);
	addToVector(m_mesh, normal);
	addToVector(m_mesh, t1);

	addToVector(m_mesh, p2);
	addToVector(m_mesh, normal);
	addToVector(m_mesh, t2);

	addToVector(m_mesh, p3);
	addToVector(m_mesh, normal);
	addToVector(m_mesh, t3);
}

//=============================================================================
// AddTriangle(vec3, vec3, vec3)
//=============================================================================

void Mesh::AddTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
	assert(m_vao == 0);
	assert(m_vbo == 0);

	const glm::vec2 zero(0.0, 0.0);

	AddTriangle(p1, zero, p2, zero, p3, zero);
}

//=============================================================================
// AddQuad(vec3, vec2, vec3, vec2, vec3, vec2, vec3, vec2)
//=============================================================================

void Mesh::AddQuad(const glm::vec3& p1, const glm::vec2& t1,
				   const glm::vec3& p2, const glm::vec2& t2,
				   const glm::vec3& p3, const glm::vec2& t3,
				   const glm::vec3& p4, const glm::vec2& t4)
{
	AddTriangle(p1, t1, p2, t2, p4, t4);
	AddTriangle(p2, t2, p3, t3, p4, t4);
}

//=============================================================================
// AddQuad(vec3, vec3, vec3, vec3)
//=============================================================================

void Mesh::AddQuad(const glm::vec3& p1, const glm::vec3& p2, 
				   const glm::vec3& p3, const glm::vec3& p4)
{
	const glm::vec2 zero(0.0, 0.0);
	AddQuad(p1, zero, p2, zero, p3, zero, p4, zero);
}

//=============================================================================
// Finish
//=============================================================================

void Mesh::Finish()
{
	assert(m_vao == 0);
	assert(m_vbo == 0);
	assert(m_mesh.size() != 0);

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	assert(m_vao != 0);
	assert(m_vbo != 0);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	size_t size = m_mesh.size() * sizeof(float);
	glBufferData(GL_ARRAY_BUFFER, size, &m_mesh[0], GL_STATIC_DRAW);
	// Vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), 0);
	// Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
	// TexCoords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));

	// These are on a per vertex array basis
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// Unbind TODO - rebind what was previously bound?
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//=============================================================================
// SetPrimitiveType
//=============================================================================

void Mesh::SetPrimitiveType(GLenum type)
{
	assert(type == GL_TRIANGLES || type == GL_POINTS);

	m_primitiveType = type;
}

//=============================================================================
// AddPoint
//=============================================================================

void Mesh::AddPoint(const glm::vec3& point)
{
	assert(m_vao == 0);
	assert(m_vbo == 0);

	// TODO - Create a special VAO for point primitives
	// so we don't have to add null values
	addToVector(m_mesh, point);
	// Dummy normal
	addToVector(m_mesh, glm::vec3(0.0));
	// Dummy tex coord
	addToVector(m_mesh, glm::vec2(0.0));
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

	glDrawArrays(m_primitiveType, 0, m_mesh.size());

//	glDisableVertexAttribArray(1);
//	glDisableVertexAttribArray(0);

	glBindVertexArray(0);
}

//=============================================================================
// 
//=============================================================================
