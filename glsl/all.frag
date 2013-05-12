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

	if (caustic != vec3(0))
	{
		color = vec4(NdotL*mix(abs(N), caustic, 0.5), 1.0);
	}
	else
	{
		color = vec4(NdotL*abs(N), 1.0);
	}
}
