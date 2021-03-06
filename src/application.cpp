#include "application.hpp"
#include "oglhelper.hpp"

#include <iostream>
#include <GL/glfw.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

//=============================================================================
// Constructor
//=============================================================================

Application::Application() :
	m_width(0),
	m_height(0),
	delta(0),
	time(0),
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
// SetResolution
//=============================================================================

void Application::SetResolution(const int width, const int height)
{
	m_width = width;
	m_height = height;

	m_fbWorld.Create(width, height);
	m_fbSurface.Create(width, height);
}

//=============================================================================
// Initialize 
//=============================================================================

bool Application::Initialize(const int screen_width, const int screen_height)
{
	glfwDisable(GLFW_MOUSE_CURSOR);

	const float aspect_ratio = (float)screen_width/screen_height;
	m_camera = new Camera(45.0f, aspect_ratio, 0.1f, 1000.0f);
	m_camera->LookAt(glm::vec3(2, 20, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, screen_width, screen_height);
	glEnable(GL_DEPTH_TEST);

	box.NewMesh();

	// Front
	box.AddQuad(glm::vec3(-0.5,  0.5, 0.5), glm::vec3( 0.5,  0.5, 0.5),
				glm::vec3( 0.5, -0.5, 0.5), glm::vec3(-0.5, -0.5, 0.5));
	
	// Back
	box.AddQuad(glm::vec3( 0.5,  0.5, -0.5), glm::vec3(-0.5, 0.5, -0.5),
				glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0.5, -0.5, -0.5));

	// Top
//	box.AddQuad(glm::vec3(-0.5, 0.5,  0.5), glm::vec3( 0.5, 0.5,  0.5),
//				glm::vec3( 0.5, 0.5, -0.5), glm::vec3(-0.5, 0.5, -0.5));
	// Make a grid

	// Bottom
	box.AddQuad(glm::vec3( 0.5, -0.5, -0.5), glm::vec3(-0.5, -0.5, -0.5), 
				glm::vec3(-0.5, -0.5,  0.5), glm::vec3( 0.5, -0.5,  0.5));

	// Right
	box.AddQuad(glm::vec3(0.5,  0.5,  0.5), glm::vec3(0.5,  0.5, -0.5), 
				glm::vec3(0.5, -0.5, -0.5), glm::vec3(0.5, -0.5,  0.5));

	// Left
	box.AddQuad(glm::vec3(-0.5,  0.5, -0.5), glm::vec3(-0.5,  0.5,  0.5),
				glm::vec3(-0.5, -0.5,  0.5), glm::vec3(-0.5, -0.5, -0.5));

	box.Finish();

	quad.NewMesh();

	quad.AddQuad(glm::vec3(-1.0, -1.0, 0.0), glm::vec3( 1.0, -1.0, 0.0),
				 glm::vec3( 1.0,  1.0, 0.0), glm::vec3(-1.0,  1.0, 0.0));

	quad.Finish();

	// This will be the water surface
	surface.NewMesh();

	const int GRID_SIZE = 100;
	const float GRID_STEP = 1.0f / float(GRID_SIZE);
	float zs = -0.5;
	for (int z = 0; z < GRID_SIZE; ++z)
	{
		float xs = -0.5;
		for (int x = 0; x < GRID_SIZE; ++x)
		{
			const glm::vec3 vert1(xs, 0.5, zs);
			const glm::vec3 vert2(xs+GRID_STEP, 0.5, zs);
			const glm::vec3 vert3(xs+GRID_STEP, 0.5, zs+GRID_STEP);
			const glm::vec3 vert4(xs, 0.5, zs+GRID_STEP);

			const glm::vec2 t1(xs+0.5, zs+0.5);
			const glm::vec2 t2(xs+0.5+GRID_STEP, zs+0.5);
			const glm::vec2 t3(xs+0.5+GRID_STEP, zs+0.5+GRID_STEP);
			const glm::vec2 t4(xs+0.5, zs+0.5+GRID_STEP);

			surface.AddQuad(vert1, t1, vert4, t4, vert3, t3, vert2, t2);

			xs += GRID_STEP;	
		}
		zs += GRID_STEP;
	}

	surface.Finish();

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	if (m_texture == 0)
	{
		std::cerr << "Error creating OpenGL texture\n";
		return false;
	}
	
	glfwGetMousePos(&m_mouse_x, &m_mouse_y);	

	return m_shaders.LoadShaders("glsl/basic.vert", "glsl/basic.frag") &&
		   m_surfaceShader.LoadShaders("glsl/displace.vert", "glsl/displace.frag") &&
		   m_worldCoordShader.LoadShaders("glsl/world.vert", "glsl/world.frag") &&
		   m_imageShader.LoadShaders("glsl/image.vert", "glsl/image.frag") &&
		   m_causticMap.LoadShaders("glsl/causticmap.vert", "glsl/causticmap.frag");
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

	delta = dt;
	time += dt;

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
	m_fbWorld.Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	m_worldCoordShader.Bind();
	m_shaders.Bind();
	glUniformMatrix4fv(m_worldCoordShader.GetUniformLocation("proj"),
						1, GL_FALSE, glm::value_ptr(m_camera->GetProj()));
	glUniformMatrix4fv(m_worldCoordShader.GetUniformLocation("view"),
						1, GL_FALSE, glm::value_ptr(m_camera->GetView()));
	glUniformMatrix4fv(m_worldCoordShader.GetUniformLocation("model"),
						1, GL_FALSE, glm::value_ptr(glm::scale(glm::mat4(1.0), 
													glm::vec3(10.0, 10.0, 10.0))));
	box.Render();
	m_fbWorld.Unbind();
	// Render the world to 
	//m_fbWorld.Bind();
	//m_worldCoordShader.Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	m_shaders.Bind();
	glUniformMatrix4fv(m_worldCoordShader.GetUniformLocation("proj"),
						1, GL_FALSE, glm::value_ptr(m_camera->GetProj()));
	glUniformMatrix4fv(m_worldCoordShader.GetUniformLocation("view"),
						1, GL_FALSE, glm::value_ptr(m_camera->GetView()));
	glUniformMatrix4fv(m_worldCoordShader.GetUniformLocation("model"),
						1, GL_FALSE, glm::value_ptr(glm::scale(glm::mat4(1.0), 
													glm::vec3(10.0, 10.0, 10.0))));
	box.Render();
	// Now we need to render the surface to a texture
	//m_fbSurface.Bind();
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_surfaceShader.Bind();
	glUniformMatrix4fv(m_surfaceShader.GetUniformLocation("proj"),
						1, GL_FALSE, glm::value_ptr(m_camera->GetProj()));
	glUniformMatrix4fv(m_surfaceShader.GetUniformLocation("view"),
						1, GL_FALSE, glm::value_ptr(m_camera->GetView()));
	glUniform1f(m_surfaceShader.GetUniformLocation("time"), time);
	glUniformMatrix4fv(m_surfaceShader.GetUniformLocation("model"),
						1, GL_FALSE, glm::value_ptr(glm::scale(glm::mat4(1.0), 
													glm::vec3(10.0, 10.0, 10.0))));
	surface.Render();
	//m_fbSurface.Unbind();

	// Do regular rendering
	//glViewport(0, 0, m_width, m_height);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

//	m_causticMap.Bind();

	// Debug window
	glViewport(0, 0, 128, 80);
	m_imageShader.Bind();
	m_fbWorld.BindTexture(GL_TEXTURE0);
	glUniform1i(m_imageShader.GetUniformLocation("tex"), 0);
	quad.Render();	
	
}

//=============================================================================
// 
//=============================================================================
