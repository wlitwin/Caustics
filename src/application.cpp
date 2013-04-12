#include "application.hpp"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

//=============================================================================
// Constructor
//=============================================================================

Application::Application() :
	camera(NULL)
{
}

//=============================================================================
// Destructor
//=============================================================================

Application::~Application()
{
	if (camera != NULL) 
	{
		delete camera;
	}
}

//=============================================================================
// Initialize 
//=============================================================================

bool Application::Initialize(const int screen_width, const int screen_height)
{
	const float aspect_ratio = (float)screen_width/screen_height;
	camera = new Camera(45.0f, aspect_ratio, 0.1f, 100.0f);
	camera->LookAt(glm::vec3(2, 4, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	glViewport(0, 0, screen_width, screen_height);
	glEnable(GL_DEPTH_TEST);

	// Simple test box
	box = new glm::vec3[6*6];
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

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glBufferData(GL_ARRAY_BUFFER, 6*6*3*sizeof(float), box, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete [] box;

	return shaders.LoadShaders("glsl/basic.vert", "glsl/basic.frag");
}

//=============================================================================
// Update 
//=============================================================================

bool Application::Update(const double delta)
{
	return true;
}

//=============================================================================
// Render
//=============================================================================

void Application::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Setup the camera
	glUseProgram(shaders.GetProgram());
	glUniformMatrix4fv(shaders.GetUniformLocation("proj"),
						1, GL_FALSE, glm::value_ptr(camera->GetProj()));
	glUniformMatrix4fv(shaders.GetUniformLocation("view"),
						1, GL_FALSE, glm::value_ptr(camera->GetView()));

	// Setup the object we're drawing
	glUniformMatrix4fv(shaders.GetUniformLocation("model"),
						1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6*6);

	glBindVertexArray(0);
}

//=============================================================================
// 
//=============================================================================
