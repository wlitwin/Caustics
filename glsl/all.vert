#version 330

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform mat4 light_vp;

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normals;

out vec3 pos;
out vec3 normal;
out vec2 causticCoord;

void main()
{
	// Calculate the normal matrix and ModelView matrix
	mat4 modelView = view * model;
	mat4 normalMatrix = transpose(inverse(model));

	// Out variables
	pos = vertex;
	normal = vec3(normalMatrix * vec4(normals, 1.0));

	gl_Position = proj * modelView * vec4(vertex, 1.0);

	vec4 vlspace = light_vp * model * vec4(vertex, 1.0);
	causticCoord = vec2(0.5*(vlspace.xy/vlspace.w)+vec2(0.5));
}
