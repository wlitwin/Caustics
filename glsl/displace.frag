#version 330

in vec3 out_vertex;
in vec3 out_normal;
in vec2 out_texCoord;

uniform vec3 light_pos;

out vec4 color;
void main()
{
	vec3 norm = normalize(out_normal);
	vec3 Lpos = light_pos;

	vec3 L = normalize(Lpos - out_vertex);

	float NdotL = dot(norm, L);

	vec3 water_color = vec3(0.4, 0.4, 1.0);

	color = vec4(water_color*NdotL, 0.4);
}
