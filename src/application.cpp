#include "application.hpp"

#include <iostream>
#include <GL/glfw.h>
#include <glm/gtc/type_ptr.hpp>

//=============================================================================
// Constructor
//=============================================================================

Application::Application() :
	m_camera(NULL),
	m_mouse_x(-1),
	m_mouse_y(-1)
{
}

//=============================================================================
// Destructor
//=============================================================================

Application::~Application()
{
	if (m_camera != NULL) 
	{
		delete m_camera;
	}
}

//=============================================================================
// Initialize 
//=============================================================================

bool Application::Initialize(const int screen_width, const int screen_height)
{
	const float aspect_ratio = (float)screen_width/screen_height;
	m_camera = new Camera(45.0f, aspect_ratio, 0.1f, 1000.0f);
	m_camera->LookAt(glm::vec3(2, 2, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	glViewport(0, 0, screen_width, screen_height);
	glEnable(GL_DEPTH_TEST);

	// Simple test box
	glm::vec3* box = new glm::vec3[6*6*2];
	// Front
	box[0] = glm::vec3(-0.5f, -0.5f, 0.5f);
	box[1] = glm::vec3( 0.5f, -0.5f, 0.5f);
	box[2] = glm::vec3( 0.5f,  0.5f, 0.5f);

	box[3] = glm::vec3( 0.5f,  0.5f, 0.5f);
	box[4] = glm::vec3(-0.5f,  0.5f, 0.5f);
	box[5] = glm::vec3(-0.5f, -0.5f, 0.5f);

	// Top
	box[6] = glm::vec3(-0.5f, 0.5f, -0.5f);
	box[7] = glm::vec3(-0.5f, 0.5f,  0.5f);
	box[8] = glm::vec3( 0.5f, 0.5f,  0.5f);

	box[9]  = glm::vec3( 0.5f, 0.5f,  0.5f);
	box[10] = glm::vec3( 0.5f, 0.5f, -0.5f);
	box[11] = glm::vec3(-0.5f, 0.5f, -0.5f);

	// Right
	box[12] = glm::vec3(0.5f, -0.5f,  0.5f);
	box[13] = glm::vec3(0.5f, -0.5f, -0.5f);
	box[14] = glm::vec3(0.5f,  0.5f, -0.5f);

	box[15] = glm::vec3(0.5f,  0.5f, -0.5f);
	box[16] = glm::vec3(0.5f,  0.5f,  0.5f);
	box[17] = glm::vec3(0.5f, -0.5f,  0.5f);

	// Left
	box[18] = glm::vec3(-0.5f, -0.5f, -0.5f);
	box[19] = glm::vec3(-0.5f, -0.5f,  0.5f);
	box[20] = glm::vec3(-0.5f,  0.5f,  0.5f);

	box[21] = glm::vec3(-0.5f,  0.5f,  0.5f);
	box[22] = glm::vec3(-0.5f,  0.5f, -0.5f);
	box[23] = glm::vec3(-0.5f, -0.5f, -0.5f);

	// Back
	box[24] = glm::vec3( 0.5f, -0.5f, -0.5f);
	box[25] = glm::vec3(-0.5f, -0.5f, -0.5f);
	box[26] = glm::vec3(-0.5f,  0.5f, -0.5f);

	box[27] = glm::vec3(-0.5f,  0.5f, -0.5f);
	box[28] = glm::vec3( 0.5f,  0.5f, -0.5f);
	box[29] = glm::vec3( 0.5f, -0.5f, -0.5f);

	// Bottom
	box[30] = glm::vec3(-0.5f, -0.5f, -0.5f);
	box[31] = glm::vec3( 0.5f, -0.5f, -0.5f);
	box[32] = glm::vec3( 0.5f, -0.5f,  0.5f);

	box[33] = glm::vec3( 0.5f, -0.5f,  0.5f);
	box[34] = glm::vec3(-0.5f, -0.5f,  0.5f);
	box[35] = glm::vec3(-0.5f, -0.5f, -0.5f);

	float scale = 1.0f;
	for (int i = 0; i < 36; ++i)
	{
		box[i] *= scale;
	}

	bool negate = true;

	for (int i = 36; i < 42; ++i)
	{
		box[i] = glm::vec3(0, 0, 1);
		if (negate) box[i] = -box[i];
	}

	for (int i = 42; i < 48; ++i)
	{
		box[i] = glm::vec3(0, -1, 0);
		if (negate) box[i] = -box[i];
	}

	for (int i = 48; i < 54; ++i)
	{
		box[i] = glm::vec3(1, 0, 0);
		if (negate) box[i] = -box[i];
	}

	for (int i = 54; i < 60; ++i)
	{
		box[i] = glm::vec3(-1, 0, 0);
		if (negate) box[i] = -box[i];
	}

	for (int i = 60; i < 66; ++i)
	{
		box[i] = glm::vec3(0, 0, -1);
		if (negate) box[i] = -box[i];
	}

	for (int i = 66; i < 72; ++i)
	{
		box[i] = glm::vec3(0, 1, 0);
		if (negate) box[i] = -box[i];
	}

	// Box
	glGenVertexArrays(1, &m_vao);
	GLuint vbo;
	glGenBuffers(1, &vbo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glBufferData(GL_ARRAY_BUFFER, 6*6*2*3*sizeof(float), box, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(6*6*3*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glfwGetMousePos(&m_mouse_x, &m_mouse_y);	

	delete [] box;

	return m_shaders.LoadShaders("glsl/basic.vert", "glsl/basic.frag");
}

//=============================================================================
// Update 
//=============================================================================

bool Application::Update(const double dt)
{
	if (glfwGetKey('W') == GLFW_PRESS)
	{
		m_camera->MoveForward(2.0f * dt);
	}

	if (glfwGetKey('A') == GLFW_PRESS)
	{
		m_camera->Strafe(-2.0f * dt);
	}

	if (glfwGetKey('D') == GLFW_PRESS)
	{
		m_camera->Strafe(2.0f * dt);
	}

	if (glfwGetKey('S') == GLFW_PRESS)
	{
		m_camera->MoveForward(-2.0f * dt);
	}

	if (glfwGetKey('Q') == GLFW_PRESS)
	{
		m_camera->Roll(-45.0f * dt);
	}

	if (glfwGetKey('E') == GLFW_PRESS)
	{
		m_camera->Roll(45.0f * dt);
	}

	if (glfwGetKey('R') == GLFW_PRESS)
	{
		m_camera->Pitch(45.0f * dt);
	}

	if (glfwGetKey('F') == GLFW_PRESS)
	{
		m_camera->Pitch(-45.0f * dt);
	}

	if (glfwGetKey('T') == GLFW_PRESS)
	{
		m_camera->Yaw(45.0f * dt);
	}

	if (glfwGetKey('G') == GLFW_PRESS)
	{
		m_camera->Yaw(-45.0f * dt);
	}

	// Get mouse delta
	int new_mouse_x, new_mouse_y;
	glfwGetMousePos(&new_mouse_x, &new_mouse_y);

	const int delta_mouse_x = new_mouse_x - m_mouse_x;
	const int delta_mouse_y = new_mouse_y - m_mouse_y;

	m_camera->Yaw(delta_mouse_x * 2.0f * dt);
	m_camera->Pitch(delta_mouse_y * 2.0f * dt);

	m_mouse_x = new_mouse_x;
	m_mouse_y = new_mouse_y;

	return true;
}

//=============================================================================
// Render
//=============================================================================

void Application::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Setup the camera
	glUseProgram(m_shaders.GetProgram());
	glUniformMatrix4fv(m_shaders.GetUniformLocation("proj"),
						1, GL_FALSE, glm::value_ptr(m_camera->GetProj()));
	glUniformMatrix4fv(m_shaders.GetUniformLocation("view"),
						1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

	// Setup the object we're drawing
	glUniformMatrix4fv(m_shaders.GetUniformLocation("model"),
						1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));

	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6*6);

	glBindVertexArray(0);
}

//=============================================================================
// 
//=============================================================================
