#version 330

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normals;

out vec3 pos;
out vec3 normal;

void main()
{
	// Calculate the normal matrix and ModelView matrix
	mat4 modelView = view * model;
	mat4 normalMatrix = transpose(inverse(model));

	// Out variables
	pos = vertex;
	normal = vec3(normalMatrix * vec4(normals, 1.0));

	gl_Position = proj * modelView * vec4(vertex, 1.0);
}
