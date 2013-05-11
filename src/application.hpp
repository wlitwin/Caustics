#ifndef _APPLICATION_HPP_
#define _APPLICATION_HPP_

#include <GL/gl.h>

#include "mesh.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "framebuffer.hpp"

/* This class contains all of the update and render
 * logic for the application. Very basic.
 */
class Application
{

public:
	
	/* Constructor, initializes all members to appropriate
	 * values.
	 */
	Application();
	
	/* Cleans up any allocated memory or buffers
	 */
	~Application();

	/* Initialize the application. Create necessary
	 * buffers and load textures, etc.
	 *
	 * Params:
	 *   screen_width  - The current screen width.
	 *   screen_height - The current screen height.
	 *
	 * Returns:
	 *   True if successfully completed initialization,
	 *   false otherwise.
	 */
	bool Initialize(const int screen_width, const int screen_height);

	/* Perform an update. This is where the state
	 * of the application happens. Check what
	 * keys are pressed, etc.
	 *
	 * Params:
	 *   dt - How much time has passed since the last call
	 *        to this method. (Expected to be called continuously)
	 *
	 * Returns:
	 *   True if this method should be called again, false otherwise.
	 */
	bool Update(const double dt);

	/* 
	 */
	void SetResolution(const int width, const int height);

	/* Render the scene.
	 */
	void Render();

private:
	int m_width;
	int m_height;
	double delta;
	double time;
	Camera* m_camera;
	int m_mouse_x;
	int m_mouse_y;
	Shader m_shaders;
	Shader m_surfaceShader;
	Shader m_worldCoordShader;
	Shader m_imageShader;
	Shader m_causticMap;
	GLuint m_texture;
	Mesh box;
	Mesh surface;
	Mesh quad;
	FrameBuffer m_fbWorld;
	FrameBuffer m_fbSurface;
};

#endif
