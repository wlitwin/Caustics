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
	glfwDisable(GLFW_MOUSE_CURSOR);

	const float aspect_ratio = (float)screen_width/screen_height;
	m_camera = new Camera(45.0f, aspect_ratio, 0.1f, 1000.0f);
	m_camera->LookAt(glm::vec3(2, 2, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	glDisable(GL_CULL_FACE);

	glViewport(0, 0, screen_width, screen_height);
	glEnable(GL_DEPTH_TEST);

	box.NewMesh();

	// Front
	box.AddQuad(glm::vec3(-0.5, -0.5, 0.5), glm::vec3( 0.5, -0.5, 0.5),
				glm::vec3( 0.5,  0.5, 0.5), glm::vec3(-0.5,  0.5, 0.5));
	
	// Back
	box.AddQuad(glm::vec3(0.5, -0.5, -0.5), glm::vec3(-0.5, -0.5, -0.5),
				glm::vec3(-0.5, 0.5, -0.5), glm::vec3( 0.5,  0.5, -0.5));

	// Top
	box.AddQuad(glm::vec3(-0.5, 0.5,  0.5), glm::vec3( 0.5, 0.5,  0.5),
				glm::vec3( 0.5, 0.5, -0.5), glm::vec3(-0.5, 0.5, -0.5));

	// Bottom
	box.AddQuad(glm::vec3( 0.5, -0.5,  0.5), glm::vec3(-0.5, -0.5,  0.5),
				glm::vec3(-0.5, -0.5, -0.5), glm::vec3( 0.5, -0.5, -0.5));

	// Right
	box.AddQuad(glm::vec3(0.5, -0.5,  0.5), glm::vec3(0.5, -0.5, -0.5),
				glm::vec3(0.5,  0.5, -0.5), glm::vec3(0.5,  0.5,  0.5));

	// Left
	box.AddQuad(glm::vec3(-0.5, -0.5, -0.5), glm::vec3(-0.5, -0.5,  0.5),
				glm::vec3(-0.5,  0.5,  0.5), glm::vec3(-0.5,  0.5, -0.5));

	box.Finish();

	glfwGetMousePos(&m_mouse_x, &m_mouse_y);	

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

	box.Render();
}

//=============================================================================
// 
//=============================================================================
