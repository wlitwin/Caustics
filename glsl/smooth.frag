#version 330

uniform sampler2D tex;
uniform vec2 resolution;
uniform float vert_blur;

in vec2 out_texCoord;

out vec4 color;

float sigma = 4;

const float PI = 3.1415926;
const float numBlurPixelsPerSize = 4.0f;

void main()
{
	float px = 1.0 / resolution.x;
	float py = 1.0 / resolution.y;

	float blurSize = px;
	vec2 blurMultiplyVec = vec2(0.0, 1.0);

	if (vert_blur > 0)
	{
		blurSize = py;
		blurMultiplyVec = vec2(1.0, 0.0);
	}

	vec3 incrementalGaussian;
	incrementalGaussian.x = 1.0 / (sqrt(2.0f * PI) * sigma);
	incrementalGaussian.y = exp(-0.5f / (sigma * sigma));
	incrementalGaussian.z = incrementalGaussian.y * incrementalGaussian.y;

	vec4 avgValue = vec4(0);
	float coefficientSum = 0;

	avgValue += texture2D(tex, out_texCoord)*incrementalGaussian.x;
	coefficientSum += incrementalGaussian.x;
	incrementalGaussian.xy *= incrementalGaussian.yz;

	for (float i = 1.0f; i <= numBlurPixelsPerSize; i++)
	{
		avgValue += texture2D(tex, out_texCoord-i*blurSize*
									blurMultiplyVec) * incrementalGaussian.x;
		avgValue += texture2D(tex, out_texCoord+i*blurSize*
									blurMultiplyVec) * incrementalGaussian.x;
		coefficientSum += 2* incrementalGaussian.x;
		incrementalGaussian.xy *= incrementalGaussian.yz;
	}

	color = avgValue / coefficientSum;
}
