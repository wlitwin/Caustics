#version 330

uniform sampler2D tex;

in vec2 out_texCoord;

out vec4 color;

void main()
{
	if (out_texCoord.y < 0.01 || out_texCoord.x < 0.01 ||
		out_texCoord.y > 0.99 || out_texCoord.x > 0.99)
	{
		color = vec4(1.0);
	}
	else
	{
		color = vec4(texture2D(tex, out_texCoord).rgb, 1.0);
	}
}
