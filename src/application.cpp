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
	m_mouse_y(-1),
	m_query(0)
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
		delete m_light;
	}

//	glEndQuery(GL_SAMPLES_PASSED);
	if (m_query != 0)
	{
		glDeleteQueries(1, &m_query);
	}
}

//=============================================================================
// SetResolution
//=============================================================================

void Application::SetResolution(const int width, const int height)
{
	m_width = width;
	m_height = height;

	m_fbWorld.Create(1024, 1024);
	m_fbSurface.Create(1024, 1024);
	m_fbCausticMap.Create(1024, 1024);
	m_fbCausticSmooth.Create(1024, 1024);
}

//=============================================================================
// Initialize 
//=============================================================================

void create_vertex_grid(Mesh& mesh, const int GRID_SIZE);
bool Application::Initialize(const int screen_width, const int screen_height)
{
	glfwDisable(GLFW_MOUSE_CURSOR);

	const float aspect_ratio = (float)screen_width/screen_height;
	m_camera = new Camera(45.0f, aspect_ratio, 0.1f, 1000.0f);
	m_camera->LookAt(glm::vec3(5, 20, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	m_light = new Camera(45.0f, aspect_ratio, 0.1f, 1000.0f);
	m_light->LookAt(glm::vec3(5, 20, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	
	glViewport(0, 0, screen_width, screen_height);
	glEnable(GL_DEPTH_TEST);

	glGenQueries(1, &m_query);

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

	// Create the vertex grid for splatting
	create_vertex_grid(m_vertexGrid, 256);

	glfwGetMousePos(&m_mouse_x, &m_mouse_y);	

	glPointSize(1.0f);

	return m_shaders.LoadShaders("glsl/basic.vert", "glsl/basic.frag") &&
		   m_surfaceShader.LoadShaders("glsl/displace.vert", "glsl/displace.frag") &&
		   m_surfaceGeom.LoadShaders("glsl/displace.vert", "glsl/world.frag") &&
		   m_worldCoordShader.LoadShaders("glsl/world.vert", "glsl/world.frag") &&
		   m_imageShader.LoadShaders("glsl/image.vert", "glsl/image.frag") &&
		   m_causticMap.LoadShaders("glsl/causticmap.vert", "glsl/causticmap.frag") &&
		   m_allShader.LoadShaders("glsl/all.vert", "glsl/all.frag") &&
		   m_smoothShader.LoadShaders("glsl/image.vert", "glsl/smooth.frag");
}

void create_vertex_grid(Mesh& mesh, const int GRID_SIZE)
{
	mesh.NewMesh();
	mesh.SetPrimitiveType(GL_POINTS);	

	const float DY = 1.0f / GRID_SIZE;
	const float DX = 1.0f / GRID_SIZE;
	const float DX_OFF = DX*0.5+0.5;

	float ys = 0.0;
	for (int i = 0; i < GRID_SIZE; ++i)
	{
		float xs = 0.0;
		for (int j = 0; j < GRID_SIZE; ++j) 
		{
			mesh.AddPoint(glm::vec3(xs, ys, 0.0f));
			xs += DX;
		}
		ys += DY;
	}
	mesh.Finish();
}

//=============================================================================
// Update 
//=============================================================================

bool Application::Update(const double dt)
{
	const float CAM_SPEED = 10.0f;
	if (glfwGetKey('W') == GLFW_PRESS)
	{
		m_camera->MoveForward(CAM_SPEED * dt);
	}

	if (glfwGetKey('A') == GLFW_PRESS)
	{
		m_camera->Strafe(-CAM_SPEED * dt);
	}

	if (glfwGetKey('D') == GLFW_PRESS)
	{
		m_camera->Strafe(CAM_SPEED * dt);
	}

	if (glfwGetKey('S') == GLFW_PRESS)
	{
		m_camera->MoveForward(-CAM_SPEED * dt);
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
	delta = dt;
	time += dt;

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
	///////////////////////////////////////////////////////////////////////////
	// Step 1 - Render the scene to a world coord texture
	///////////////////////////////////////////////////////////////////////////
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	m_fbWorld.PrepareRender();
	m_worldCoordShader.Bind();
	glUniformMatrix4fv(m_worldCoordShader.GetUniformLocation("proj"),
						1, GL_FALSE, glm::value_ptr(m_light->GetProj()));
	glUniformMatrix4fv(m_worldCoordShader.GetUniformLocation("view"),
						1, GL_FALSE, glm::value_ptr(m_light->GetView()));
	glUniformMatrix4fv(m_worldCoordShader.GetUniformLocation("model"),
						1, GL_FALSE, glm::value_ptr(glm::scale(glm::mat4(1.0), 
													glm::vec3(10.0, 10.0, 10.0))));
	box.Render();

	///////////////////////////////////////////////////////////////////////////
	// Step 2 - Render the surface geometry to a texture
	///////////////////////////////////////////////////////////////////////////
	glDisable(GL_CULL_FACE);
	m_fbSurface.PrepareRender();
	m_surfaceGeom.Bind();
	glUniformMatrix4fv(m_surfaceGeom.GetUniformLocation("proj"),
						1, GL_FALSE, glm::value_ptr(m_light->GetProj()));
	glUniformMatrix4fv(m_surfaceGeom.GetUniformLocation("view"),
						1, GL_FALSE, glm::value_ptr(m_light->GetView()));
	glUniform1f(m_surfaceGeom.GetUniformLocation("time"), time);
	glUniformMatrix4fv(m_surfaceGeom.GetUniformLocation("model"),
						1, GL_FALSE, glm::value_ptr(glm::scale(glm::mat4(1.0), 
													glm::vec3(10.0, 10.0, 10.0))));
	glBeginQuery(GL_SAMPLES_PASSED, m_query);

	surface.Render();

	glEndQuery(GL_SAMPLES_PASSED);

	unsigned int samplesPassed;
	glGetQueryObjectuiv(m_query, GL_QUERY_RESULT, &samplesPassed);
	const float f_samplesPassed = (float)samplesPassed;
	
	///////////////////////////////////////////////////////////////////////////
	// Step 3 - Create the caustic map texture
	///////////////////////////////////////////////////////////////////////////
	
	m_fbCausticMap.PrepareRender();
	m_causticMap.Bind();
	glUniformMatrix4fv(m_causticMap.GetUniformLocation("proj"),
						1, GL_FALSE, glm::value_ptr(m_light->GetProj()));
	glUniformMatrix4fv(m_causticMap.GetUniformLocation("view"),
						1, GL_FALSE, glm::value_ptr(m_light->GetView()));

	const glm::vec3 lightPos(m_light->GetPosition());
	glUniform3f(m_causticMap.GetUniformLocation("light_pos"), 
					lightPos.x, lightPos.y, lightPos.z);

	//glUniform1f(m_causticMap.GetUniformLocation("totalV"), f_samplesPassed);
	// Bind all of the textures
	m_fbWorld.BindTexture(GL_TEXTURE0);
	m_fbSurface.BindTexture(GL_TEXTURE4);

	glUniform1i(m_causticMap.GetUniformLocation("texReceiverPos"), 0);
	glUniform1i(m_causticMap.GetUniformLocation("texSurfacePos"), 4);
	glUniform1i(m_causticMap.GetUniformLocation("texSurfaceNorm"), 5);
	//glUniform1f(m_causticMap.GetUniformLocation("time"), time);

	m_vertexGrid.Render();
//	surface.Render();	
	///////////////////////////////////////////////////////////////////////////
	// Step 4 - Smooth the caustic map
	///////////////////////////////////////////////////////////////////////////

	glEnable(GL_BLEND);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_fbCausticSmooth.PrepareRender();
	m_smoothShader.Bind();	

	m_fbCausticMap.BindTexture(GL_TEXTURE0);
	glUniform1i(m_smoothShader.GetUniformLocation("tex"), 0);
	glUniform1f(m_smoothShader.GetUniformLocation("vert_blur"), 0);

	glUniform2f(m_smoothShader.GetUniformLocation("resolution"), 
			m_fbCausticSmooth.GetWidth(),
			m_fbCausticSmooth.GetHeight());

	quad.Render();
	//m_fbCausticSmooth.Unbind();

	m_fbCausticMap.PrepareRender();
	//m_smoothShader.Bind();
	m_fbCausticSmooth.BindTexture(GL_TEXTURE0);
	glUniform1f(m_smoothShader.GetUniformLocation("vert_blur"), 1);
	quad.Render();
	
	m_fbCausticMap.Unbind();

	///////////////////////////////////////////////////////////////////////////
	// Do regular rendering
	///////////////////////////////////////////////////////////////////////////

	glViewport(0, 0, m_width, m_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glEnable(GL_CULL_FACE);
	m_allShader.Bind();
	m_fbCausticMap.BindTexture(GL_TEXTURE0);
	glUniformMatrix4fv(m_allShader.GetUniformLocation("proj"),
						1, GL_FALSE, glm::value_ptr(m_camera->GetProj()));
	glUniformMatrix4fv(m_allShader.GetUniformLocation("view"),
						1, GL_FALSE, glm::value_ptr(m_camera->GetView()));
	glUniformMatrix4fv(m_allShader.GetUniformLocation("model"),
						1, GL_FALSE, glm::value_ptr(glm::scale(glm::mat4(1.0), 
													glm::vec3(10.0, 10.0, 10.0))));
	glm::mat4 l_vp = m_light->GetProj() * m_light->GetView();
	glUniformMatrix4fv(m_allShader.GetUniformLocation("light_vp"), 
						1, GL_FALSE, glm::value_ptr(l_vp));
				
	glUniform3f(m_allShader.GetUniformLocation("light_pos"), 
					lightPos.x, lightPos.y, lightPos.z);
				
	glUniform1i(m_allShader.GetUniformLocation("texCaustics"), 0);
	box.Render();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_surfaceShader.Bind();
	glUniformMatrix4fv(m_surfaceShader.GetUniformLocation("proj"),
						1, GL_FALSE, glm::value_ptr(m_camera->GetProj()));
	glUniformMatrix4fv(m_surfaceShader.GetUniformLocation("view"),
						1, GL_FALSE, glm::value_ptr(m_camera->GetView()));
	glUniform1f(m_surfaceShader.GetUniformLocation("time"), time);
	glUniformMatrix4fv(m_surfaceShader.GetUniformLocation("model"),
						1, GL_FALSE, glm::value_ptr(glm::scale(glm::mat4(1.0), 
													glm::vec3(10.0, 10.0, 10.0))));
	glUniform3f(m_surfaceShader.GetUniformLocation("light_pos"),
					lightPos.x, lightPos.y, lightPos.z);
	const glm::vec3 camDir(m_camera->GetForward());
	glUniform3f(m_surfaceShader.GetUniformLocation("view_dir"),
					camDir.x, camDir.y, camDir.z);
	//glUniformMatrix4fv(m_surfaceShader.GetUniformLocation("light_view"),
	//					1, GL_FALSE, glm::value_ptr(m_light->GetView()));
	surface.Render();
	///////////////////////////////////////////////////////////////////////////
	// Debug windows
	///////////////////////////////////////////////////////////////////////////
	glViewport(0, 0, 128, 80);
	// World
	m_imageShader.Bind();
	m_fbWorld.BindTexture(GL_TEXTURE0);
	glUniform1i(m_imageShader.GetUniformLocation("tex"), 0);
	quad.Render();	
	glViewport(128, 0, 128, 80);
	glUniform1i(m_imageShader.GetUniformLocation("tex"), 1);
	quad.Render();
	m_fbSurface.BindTexture(GL_TEXTURE0);
	// Surface
	glViewport(256, 0, 128, 80);
	glUniform1i(m_imageShader.GetUniformLocation("tex"), 0);
	quad.Render();
	glViewport(384, 0, 128, 80);
	glUniform1i(m_imageShader.GetUniformLocation("tex"), 1);
	quad.Render();
	// Caustic Map
	glViewport(512, 0, 640, 320);
	m_fbCausticMap.BindTexture(GL_TEXTURE0);
	glUniform1i(m_imageShader.GetUniformLocation("tex"), 0);
	quad.Render();
}

//=============================================================================
// 
//=============================================================================
