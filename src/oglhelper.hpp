#ifndef __OGL_HELPER_HPP__
#define __OGL_HELPER_HPP__

#include <GL/gl.h>
#include <string>

/* Checks to see if there are any OpenGL errors.
 *
 * Takes an optional prefix string. This string will
 * be printed before the error message if there is
 * an error. If there are no errors this method simply
 * returns without printing anything.
 *
 * Params:
 *   prefix - String to be printed before the error
 *            message
 */
void checkOGLError(const std::string& prefix = "");

/* Check the currently bound framebuffer for errors. If
 * there is an error it is printed to stderr. Otherwise
 * nothing is printed.
 */
void checkFBOError();

#endif
