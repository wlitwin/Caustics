#version 330

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec3 in_normal;

out vec3 out_vertex;
out vec3 out_normal;

void main()
{
	mat4 modelView = view * model;
	mat4 normalMatrix = transpose(inverse(model));

	out_normal = vec3(normalMatrix * vec4(in_normal, 1.0));
	out_vertex = vec3(model * vec4(in_vertex, 1.0));

	gl_Position = proj * modelView * vec4(in_vertex, 1.0);	
}
