#version 330

in vec3 pos;
in vec3 normal;

out vec4 color;

void main()
{
	vec3 norm = normalize(normal);
	vec3 Lpos = vec3(0, 5, 0);

	vec3 L = normalize(Lpos - pos);

	float NdotL = dot(norm, L);

	color = vec4(NdotL, 0, 0, 1);
	//vec3 n = vec3(norm.x + 1.0, norm.y + 1.0, norm.z + 1.0);
	//n = n / 2.0;
	//color = vec4(n, 1);
	//color = vec4(1, 0, 0, 1);
}
