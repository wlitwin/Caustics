#ifndef __FRAMEBUFFER_HPP__
#define __FRAMEBUFFER_HPP__

#include <GL/gl.h>

/* This class is a wrapper around FrameBuffer Objects in OpenGL. These
 * are used for off-screen render targets for use in things like deferred
 * shading. Currently only supports 1 color output and a depth buffer.
 */
class FrameBuffer
{
public:
	/* Create a new framebuffer object. This constructor allocates no
	 * resources. A call to Create() must be made before this framebuffer
	 * object can be used.
	 */
	FrameBuffer();

	/* Cleans up any allocated resources
	 */
	~FrameBuffer();

	/* Allocate an OpenGL Framebuffer Object with the given dimensions. This
	 * method must be called before you can call the Bind*() methods.
	 *
	 * The framebuffer object is created with a RGB32F texture for the color
	 * attachment and a 24-bit depth buffer.
	 *
	 * Params:
	 *   width - The width of the framebuffer in pixels
	 *   height - The height of the framebuffer in pixels
	 */
	void Create(const int width, const int height);

	/* Bind the framebuffer for rendering. After this method is called any
	 * rendered geometry will make use of this framebuffer object instead of
	 * the normal screen object.
	 *
	 * The shader should output to COLOR_ATTACHMENT0.
	 */
	void Bind() const;

	/* After rendering to this framebuffer object the output may be used in
	 * another rendering pass via a texture. This method will bind the output
	 * from this framebuffer object to a texture unit which can be used by
	 * a sampler2D.
	 *
	 * Params:
	 *   textureUnit - A valid OpenGL texture unit. Must be >= GL_TEXTURE0
	 *                 and < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS
	 */
	void BindTexture(const GLenum textureUnit) const;

private:
	/* Cleans up all of the OpenGL allocated buffers
	 */
	void cleanup();

private:
	GLuint m_fbo; // OGL framebuffer
	GLuint m_tex; // OGL texture (framebuffer color attachment 0)
	GLuint m_rbo; // OGL renderbuffer (framebuffer's depth buffer)
	int m_width;  // Framebuffer width
	int m_height; // Framebuffer height
};

#endif
