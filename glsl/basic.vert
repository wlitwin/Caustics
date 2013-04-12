#version 330

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

in vec3 vertex;

void main()
{
	gl_Position = proj * view * model * vec4(vertex, 1.0);
}
