#version 330 

layout(location = 0) in vec3 in_vertex;

out vec2 out_texCoord;

void main()
{
	//float stepX = 1.0 / resolution.x;
	//float stepY = 1.0 / resolution.y;

	out_texCoord = (in_vertex.xy+1.0)/2.0;

	gl_Position = vec4(in_vertex, 1.0);	
}
