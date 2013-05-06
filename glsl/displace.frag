#version 330

uniform float dt;
uniform float time;

in vec3 out_vertex;
in vec3 out_normal;
in vec2 out_texCoord;

uniform sampler2D tex;

out vec4 color;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
	vec3 norm = normalize(out_normal);
	vec3 Lpos = vec3(1, 5, 1);

	vec3 L = normalize(Lpos - out_vertex);

	float NdotL = dot(norm, L);

	vec3 water_color = vec3(0.4, 0.4, 1.0);

	color = vec4(water_color*NdotL, 0.5);
	//color = color + vec4(0.1, 0.1, 0.1, 0.0);
	//color = vec4(texture2D(tex, out_texCoord.st).r, 0.1, 0.1, 1.0);//vec4(out_texCoord.rg, 1.0, 1.0);
	//color = vec4(abs(texture2D(tex, out_texCoord.st).rgb), 1.0);
	//vec3 n = vec3(norm.x + 1.0, norm.y + 1.0, norm.z + 1.0);
	//n = n / 2.0;
	//color = vec4(n, 1);
	//color = vec4(1, 0, 0, 1);
}
