#version 330

uniform vec3 light_pos;
uniform sampler2D texCaustics;

out vec4 color;

in vec3 pos;
in vec3 normal;
in vec2 causticCoord;

void main()
{
	vec3 N = normalize(normal);	
	vec3 L = normalize(light_pos - pos);
	float NdotL = dot(N, L);

	vec3 caustic = texture2D(texCaustics, causticCoord).rgb;

	vec3 cornflower_blue = vec3(0.390625, 0.58203125, 0.92678125);

	vec3 ambient = cornflower_blue*0.1;

	color = vec4(NdotL*cornflower_blue + ambient + caustic, 1.0);
}
