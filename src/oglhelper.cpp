#include "oglhelper.hpp"

#include "GL/gl.h"
#include <iostream>

//=============================================================================
// checkOGLError
//=============================================================================

void checkOGLError(const std::string& prefix)
{
	using std::cerr;
	using std::endl;

	GLenum error(glGetError());

	switch (error)
	{
		case GL_NO_ERROR:
			return;
		case GL_INVALID_ENUM:
			cerr << prefix << " - OGL Error - Invalid Enum" << endl;
			break;
		case GL_INVALID_VALUE:
			cerr << prefix << " - OGL Error - Invalid Value" << endl;
			break;
		case GL_INVALID_OPERATION:
			cerr << prefix << " - OGL Error - Invalid Operation" << endl;
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			cerr << prefix << " - OGL Error - Invalid Framebuffer Operation" << endl;
			break;
		case GL_OUT_OF_MEMORY:
			cerr << prefix << " - OGL Error - Out of Memory" << endl;
			break;
		case GL_STACK_UNDERFLOW:
			cerr << prefix << " - OGL Error - Stack Underflow" << endl;
			break;
		case GL_STACK_OVERFLOW:
			cerr << prefix << " - OGL Error - Stack Overflow" << endl;
			break;
		default:
			cerr << prefix << " - OGL Error - Unknown" << endl;
			break;
	}
}

//=============================================================================
// checkFBOError
//=============================================================================

void checkFBOError()
{
	using std::cerr;
	using std::endl;
	
	const GLenum error(glCheckFramebufferStatus(GL_FRAMEBUFFER));

	switch(error)
	{
		case GL_FRAMEBUFFER_COMPLETE:
			return;
		case GL_FRAMEBUFFER_UNDEFINED:
			cerr << "FBO: Undefined" << endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			cerr << "FBO: Incomplete Attachment" << endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			cerr << "FBO: Incomplete Missing Attachment" << endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			cerr << "FBO: Incomplete Draw Buffer" << endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			cerr << "FBO: Incomplete Read Buffer" << endl;
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			cerr << "FBO: Unsupported Framebuffer" << endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			cerr << "FBO: Incomplete Multisample" << endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			cerr << "FBO: Incomplete Layer Targets" << endl;
		default:
			cerr << "FBO: Unknown error" << endl;
			break;
	}
}

//=============================================================================
//
//=============================================================================
