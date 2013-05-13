#version 330

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

uniform float time;

layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texCoord;

// Out variables
out vec3 out_vertex;
out vec3 out_normal;
out vec2 out_texCoord;

// Forward function declarations
float calcDisp(vec2 pos);

vec3 calcNormal()
{
	vec2 t1 = vec2(in_texCoord.x+0.01, in_texCoord.y+0.01);
	vec2 t2 = vec2(in_texCoord.x-0.01, in_texCoord.y-0.01);
	vec2 t3 = vec2(in_texCoord.x+0.01, in_texCoord.y-0.01);
	vec2 t4 = vec2(in_texCoord.x-0.01, in_texCoord.y+0.01);

	float y1 = calcDisp(t1);
	float y2 = calcDisp(t2);
	float y3 = calcDisp(t3);
	float y4 = calcDisp(t4);

	vec3 p1 = vec3(t1.x, y1, t1.y);
	vec3 p2 = vec3(t2.x, y2, t2.y);
	vec3 p3 = vec3(t3.x, y3, t3.y);
	vec3 p4 = vec3(t4.x, y4, t4.y);

	vec3 e1 = p1-p2;
	vec3 e2 = p3-p2;

	vec3 n1 = cross(e1, e2);

	vec3 e3 = p1-p4;
	vec3 e4 = p3-p4;

	vec3 n2 = cross(e3, e4);

	return normalize((n1+n2)/2.0);
}

float calcDisp(vec2 pos)
{
	vec2 cPos = -1.0 + 2.0*pos;
	float cLength = length(cPos);

	float x1 = cLength*14.0-time*4.0;
	float y1 = cos(x1);

	vec2 cPos2 = -1.5 + 4.0*pos;
	float cLength2 = length(cPos2);
	float x2 = cLength2*6.0-time*8.0;
	float y2 = cos(x2);

	return ((y1+y2)*0.5)*0.02;
}

void main()
{
	// Calculate the normal matrix and ModelView matrix
	mat4 modelView = view * model;
	mat4 normalMatrix = transpose(inverse(model));

	vec3 new_vertex = in_vertex;

	new_vertex.y += calcDisp(in_texCoord.st);
	vec3 new_normal = calcNormal();

	// Out variables
	out_vertex = vec3(model * vec4(new_vertex, 1.0));
	out_normal = vec3(normalMatrix * vec4(new_normal, 1.0));
	out_texCoord = in_texCoord;

	gl_Position = proj * modelView * vec4(new_vertex, 1.0);
}