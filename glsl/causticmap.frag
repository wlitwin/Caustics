#version 330

in vec3 out_vertex;
in float out_ndotl;

uniform float totalV;

layout(location = 0) out vec4 causticColor;

void main()
{
	vec3 intensity = vec3(1.0);
	causticColor = vec4(intensity, 0.6);
}
