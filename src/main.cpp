#include <GL/glfw.h>

#include <cstdlib>  // For EXIT_SUCCESS/FAILURE
#include <iostream> // For cout/cerr

#include "application.hpp"

using std::cerr;
using std::cout;

/* Setup the OpenGL context and window using GLFW. GLFW
 * is a very useful cross-platform library that will do
 * all the dirty work of setting up a window and context
 * for you. It also abstracts a lot of basic functions
 * like keyboard and mouse input for you.
 */
int main(void)
{
	if (!glfwInit()) 
	{
		exit(EXIT_FAILURE);
	}

	// Change these to change what version of OpenGL to use
	const int desired_major_version = 3;
	const int desired_minor_version = 3;

	// Set the version of OpenGL we want to use
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, desired_major_version);	
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, desired_minor_version);
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Try to create the context and open the window
	if (!glfwOpenWindow(640, 480, 8, 8, 8, 8, 24, 0, GLFW_WINDOW))
	{
		cerr << "Failed to create an OpenGL context\n";
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Make sure we got the version of OpenGL we asked for
	int v_major = glfwGetWindowParam(GLFW_OPENGL_VERSION_MAJOR);
	int v_minor = glfwGetWindowParam(GLFW_OPENGL_VERSION_MINOR);

	if (v_major < desired_major_version || 
		v_minor < desired_minor_version) 
	{
		cerr << "Failed to get appropriate OpenGL Context, got: "
			 << v_major << "." << v_minor << "\n";
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	cout << "OpenGL Context: " << v_major << "." << v_minor << "\n";

	int forward_compat = glfwGetWindowParam(GLFW_OPENGL_FORWARD_COMPAT);
	int opengl_context = glfwGetWindowParam(GLFW_OPENGL_PROFILE);
	int debug_context  = glfwGetWindowParam(GLFW_OPENGL_DEBUG_CONTEXT);

	if (forward_compat == GL_TRUE) 
	{
		cout << "Forward Compatible\n";
	}

	switch (opengl_context)
	{
		case GLFW_OPENGL_CORE_PROFILE:
			cout << "Core Profile\n";
			break;
		case GLFW_OPENGL_COMPAT_PROFILE:
			cout << "Compat Profile\n";
			break;
	}

	if (debug_context == GL_TRUE)
	{
		cout << "Debug Context\n";
	}

	// Everything's O.K., so let's start rendering
	glfwSetWindowTitle("Caustics Demo");

	// Create the application
	Application* app = new Application();
	app->Initialize();

	double previous_time = glfwGetTime();
	while (glfwGetWindowParam(GLFW_OPENED) && !glfwGetKey(GLFW_KEY_ESC))
	{
		// Get the elapsed time since the last update
		double current_time = glfwGetTime();
		const double delta_time = current_time - previous_time;
		previous_time = current_time;

		// Update the application's state
		app->Update(delta_time);

		// Render the application state
		app->Render();

		// Put the render on the screen
		glfwSwapBuffers();
	}

	delete app;

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
