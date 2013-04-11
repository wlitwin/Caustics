/* Shader.hpp
 *
 * Provides a utility class that handles create GLSL programs
 * from shader source files. Specify the two source filenames
 * in the constructor and then check isValid() before using
 * the shader class.
 */

#include <GL/gl.h>

#include <string>

#ifndef _SHADER_HPP_
#define _SHADER_HPP_

class Shader
{
public:
	/* Construct a new shader object. It will create a new OpenGL program
	 * object using the contents of the vertex and fragment shader files
	 * provided as arguments.
	 *
	 * Params:
	 *   vert_file - Path to the vertex shader source file.
	 *   frag_file - Path to the fragment shader source file.
	 *
	 * Errors will automatically be printed to standard error if any
	 * problems were encountered.
	 *
	 * Before using this object call isValid() and make sure it returns
	 * true. If it does not then something went wrong compiling the
	 * shaders or linking the final program.
	 */
	Shader(const std::string& vert_file, const std::string& frag_file);

	/* Destroy the shader object. This will delete the OpenGL  program 
	 * object as well.
	 */
	~Shader();

	/* Check if the shader program was successfully created.
	 *
	 * Returns:
	 *   True if everything worked, false if something went wrong.
	 */
	bool isValid() const;

	/* Returns the OpenGL program object. Use in the call to glUseProgram().
	 * If unable to successfully create a program object this method will
	 * return 0.
	 */
	GLuint getProgram() const;

// Private class functions
private: 
	/* Helper function that creates a shader object given a shader type and
	 * a path to a shader file.
	 *
	 * Params:
	 *   type     - The type of shader, must be GL_VERTEX_SHADER, GL_FRAGMENT_SHADER
	 *              or GL_GEOMETRY_SHADER
	 *   filename - The path to the shader's source file.
	 *   out_var  - An out parameter. Upon successful creation it is set to the
	 *              OpenGL shader object's handle. Upon failure it has the value 0.
	 *
	 * Returns:
	 *   True if successfully created the shader object. False otherwise.
	 *
	 *   All problems encountered during creation are printed to standard error.
	 */
	static bool create_shader(GLenum type, 
				const std::string& filename, GLuint& out_var);

	/* Creates an OpenGL program object. It stores the result in m_program.
	 *
	 * Params:
	 *   vert_file - The path to the vertex shader source file.
	 *   frag_file - The path to the fragment shader source file.
	 *
	 * Returns:
	 *   True if the OpenGL program was successfully created, false otherwise.
	 *
	 *   If any problems are encountered during the OpenGL program creation
	 *   they are printed to standard error and m_program is set to 0.
	 */
	bool create_program(const std::string& vert_file,
						const std::string& frag_file);


private: // Class Variables
	GLuint m_program;
	bool is_valid;
};

#endif
