#version 330

in vec3 out_vertex;
in vec3 out_normal;
in vec2 out_texCoord;

uniform vec3 light_pos;
uniform vec3 view_dir;

out vec4 color;
void main()
{
	vec3 norm = normalize(out_normal);

	vec3 L = normalize(light_pos - out_vertex);

	float NdotL = dot(norm, L);

	vec3 water_color = vec3(0.4, 0.4, 1.0);

	vec3 spec = vec3(0);
	if (NdotL > 0)
	{
		spec = vec3(1) * pow(max(0, 
			dot(reflect(L, norm), normalize(-view_dir))), 50);
	}

	color = vec4(water_color*NdotL+spec, 0.8);
}
