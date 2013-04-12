#include <GL/glew.h>
#include <GL/glfw.h>

#include <cstdlib>  // For EXIT_SUCCESS/FAILURE
#include <iostream> // For cout/cerr

#include "application.hpp"

using std::cerr;
using std::cout;

// Window size
const int window_width  = 640;
const int window_height = 480;

// Change these to change what version of OpenGL to use
const int desired_major_version = 3;
const int desired_minor_version = 3;

// The application that does all the updating/rendering
Application* app = NULL;

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
		cerr << "Failed to initialize GLFW\n";
		exit(EXIT_FAILURE);
	}

	// Set the version of OpenGL we want to use
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, desired_major_version);	
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, desired_minor_version);
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Try to create the context and open the window
	if (!glfwOpenWindow(window_width, window_height, 8, 8, 8, 8, 24, 0, GLFW_WINDOW))
	{
		cerr << "Failed to create an OpenGL context\n";
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// We need to also initialize GLEW because it loads the entry
	// points for the extension functions, like glGenVertexArray()
	// and other functions that are required by a newer OpenGL
	// context.
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) 
	{
		cerr << "Failed to initialize GLEW\n";
		exit(EXIT_FAILURE);
	}

	if (!GLEW_VERSION_3_3)
	{
		cerr << "Insufficient GLEW version\n";
		exit(EXIT_FAILURE);
	}

	// Make sure we got the version of OpenGL we asked for
	const int v_major = glfwGetWindowParam(GLFW_OPENGL_VERSION_MAJOR);
	const int v_minor = glfwGetWindowParam(GLFW_OPENGL_VERSION_MINOR);

	if (v_major < desired_major_version || 
		v_minor < desired_minor_version) 
	{
		cerr << "Failed to get appropriate OpenGL Context, got: "
			 << v_major << "." << v_minor << "\n";
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Check what we ended up with
	cout << "OpenGL Context: " << v_major << "." << v_minor << "\n";

	const int forward_compat = glfwGetWindowParam(GLFW_OPENGL_FORWARD_COMPAT);
	const int opengl_context = glfwGetWindowParam(GLFW_OPENGL_PROFILE);
	const int debug_context  = glfwGetWindowParam(GLFW_OPENGL_DEBUG_CONTEXT);

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

	// Do some more minor setup things
	glfwSetWindowTitle("Caustics Demo");

	// Center the window
	GLFWvidmode video_mode;
	glfwGetDesktopMode(&video_mode);
	glfwSetWindowPos((video_mode.Width - window_width) / 2,
					 (video_mode.Height - window_height) / 2);
	
	// Create the application
	app = new Application();
	if (!app->Initialize(window_width, window_height))
	{
		std::cerr << "Application failed to initialize\n";
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Enter the actual update/render loop
	double previous_time = glfwGetTime();
	while (glfwGetWindowParam(GLFW_OPENED) && !glfwGetKey(GLFW_KEY_ESC))
	{
		// Get the elapsed time since the last update
		double current_time = glfwGetTime();
		const double delta_time = current_time - previous_time;
		previous_time = current_time;

		// Update the application's state
		if (!app->Update(delta_time)) 
		{
			break;
		}

		// Render the application's state
		app->Render();

		// Put the render on the screen
		glfwSwapBuffers();
	}

	delete app;

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
