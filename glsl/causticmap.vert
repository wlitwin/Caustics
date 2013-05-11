#version 330

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

uniform sampler2D texPositions;

layout(location = 0) in vec3 in_vertex;

vec3 estimateIntersection(vec3 v, vec3 r, mat4 mvp)
{
	vec3 p1 = v + 1.0*r;
	vec4 texPt = vec4(p1, 1)*mvp;
	vec2 tc = vec2(0.5*(texPt.xy/texPt.w)+vec2(0.5, 0.5));
	tc.y = 1.0 - tc.y;
	vec4 recPos = texture2D(texPositions, tc);
	vec3 p2 = v + distance(v, recPos.xyz)*r;
	texPt = vec4(p2, 1.0)*mvp;
	tc = vec2(0.5*(texPt.xy/texPt.w)+vec2(0.5, 0.5));
	tc.y = 1.0 - tc.y;

	return texture2D(texPositions, tc).rgb;
}

void main()
{
	mat4 mvp = proj * view * model;

	vec3 point = in_vertex;
	for (int i = 0; i < 10; ++i)
	{
		point = estimateIntersection(point, -normalize(in_vertex), mvp);
	}


}
