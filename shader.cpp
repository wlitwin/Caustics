/* Implementation of the shader class 
 */

#include "shader.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

using std::string;
using std::fstream;

//=============================================================================
// Constructor
//=============================================================================

Shader::Shader(const string& vert_file, const string& frag_file) :
	m_program(0),
	is_valid(false)
{
	is_valid = create_program(vert_file, frag_file);
}

//=============================================================================
// Destructor
//=============================================================================

Shader::~Shader()
{
}

//=============================================================================
// isValid
//=============================================================================

bool Shader::isValid() const
{
	return is_valid;
}

//=============================================================================
// getProgram
//=============================================================================

GLuint Shader::getProgram() const
{
	return m_program;
}

//=============================================================================
// create_program
//=============================================================================

bool Shader::create_program(const string& vert_file, const string& frag_file)
{
	if (m_program != 0)
	{
		glDeleteProgram(m_program);
		m_program = 0;
	}

	GLuint vert_shader = 0;
	if (!create_shader(GL_VERTEX_SHADER, vert_file, vert_shader))
	{
		return false;
	}

	GLuint frag_shader = 0;
	if (!create_shader(GL_FRAGMENT_SHADER, frag_file, frag_shader))
	{
		// Cleanup the vertex shader object
		glDeleteShader(vert_shader);
		return false;
	}

	m_program = glCreateProgram();
	glAttachShader(m_program, vert_shader);
	glAttachShader(m_program, frag_shader);

	glLinkProgram(m_program);

	GLint status;
	glGetProgramiv(m_program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint log_length;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &log_length);

		GLchar* log = new GLchar[log_length+1];
		glGetProgramInfoLog(m_program, log_length, NULL, log);
		std::cerr << "Failed to link program:\n"
			<< vert_file << "\n" 
			<< frag_file << "\n"
			<< log << "\n";
		delete [] log;

		// Cleanup the program and shader variables
		glDetachShader(m_program, vert_shader);
		glDetachShader(m_program, frag_shader);
		glDeleteProgram(m_program);
		m_program = 0;
	}

	// The shader objects are no longer needed after they
	// have been compiled into the program.
	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);
	return status != GL_FALSE;
}

//=============================================================================
// create_shader
//=============================================================================

bool Shader::create_shader(GLenum type, const string& filename, GLuint& out_var)
{
	// Clear the output variable just in case this doesn't work
	out_var = 0;

	fstream file(filename.c_str(), std::ios::in);
	string source;
	
	// See if we could open the file
	if (!file.is_open())
	{
		return false;
	}

	// Read in the source
	std::stringstream ss;
	ss << file.rdbuf();
	source = ss.str();

	// Create the shader object after successfully reading the shader source	
	GLuint shader = glCreateShader(type);	

	const char* c_source = source.c_str();
	glShaderSource(shader, 1, &c_source, NULL);

	glCompileShader(shader);

	// Check if compilation worked
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint log_length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

		GLchar* log = new GLchar[log_length+1];
		glGetShaderInfoLog(shader, log_length, NULL, log);

		std::cerr << "Failed to compile shader:\n"
			<< filename << "\n"
			<< log << "\n";

		delete [] log;

		glDeleteShader(shader);
		return false;
	}

	// Set the output variable appropriately
	out_var = shader;
	return true;
}

//=============================================================================
//
//=============================================================================
