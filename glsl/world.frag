#version 330

in vec3 out_vertex;
in vec3 out_normal;

layout(location = 0) out vec4 worldPos;
layout(location = 1) out vec4 normalPos;

void main()
{
	worldPos = vec4(out_vertex, 1.0);
	normalPos = vec4(out_normal, 1.0);
}
