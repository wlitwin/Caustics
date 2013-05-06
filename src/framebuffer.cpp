#include "framebuffer.hpp"
#include "oglhelper.hpp"

#include <cassert>
#include <iostream>

//=============================================================================
// Constructor
//=============================================================================

FrameBuffer::FrameBuffer() :
	m_fbo(0),
	m_tex(0),
	m_rbo(0),
	m_width(0),
	m_height(0)
{

}

//=============================================================================
// Destructor
//=============================================================================

FrameBuffer::~FrameBuffer()
{
	cleanup();
}

//=============================================================================
// Create
//=============================================================================

void FrameBuffer::Create(const int width, const int height)
{
	assert(width > 0);
	assert(height > 0);

	m_width = width;
	m_height = height;

	checkOGLError("FBO Create Begin");

	if (m_fbo != 0 || m_tex != 0 || m_rbo != 0)
	{
		cleanup();
	}

	glGenFramebuffers(1, &m_fbo);
	if (m_fbo == 0)
	{
		std::cerr << "FrameBuffer: Failed to create FBO" << std::endl;
		return;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	// Create our texture
	glGenTextures(1, &m_tex);
	glBindTexture(GL_TEXTURE_2D, m_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);

	// Depth Buffer
	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

	// Attach to framebuffer object
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex, 0);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

	checkFBOError();

	checkOGLError("FBO Create 2");

	// Unbind our framebuffer/rebind the window framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//=============================================================================
// Bind
//=============================================================================

void FrameBuffer::Bind() const
{
	assert(m_fbo != 0);
	assert(m_tex != 0);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glViewport(0, 0, m_width, m_height);
}

//=============================================================================
// BindTexture
//=============================================================================

void FrameBuffer::BindTexture(const GLenum textureUnit) const
{
	assert(textureUnit >= GL_TEXTURE0);
	assert(textureUnit < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);

	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_tex);
}

//=============================================================================
// Cleanup
//=============================================================================

void FrameBuffer::cleanup()
{
	if (m_fbo != 0)
	{
		glDeleteFramebuffers(1, &m_fbo);
		m_fbo = 0;
	}

	if (m_tex != 0)
	{
		glDeleteTextures(1, &m_tex);
		m_tex = 0;
	}

	if (m_rbo != 0)
	{
		glDeleteRenderbuffers(1, &m_rbo);
		m_rbo = 0;
	}
}

//=============================================================================
// 
//=============================================================================


//=============================================================================
// 
//=============================================================================
