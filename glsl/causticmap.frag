#version 330

in vec3 out_vertex;
in float is_valid;

layout(location = 0) out vec4 causticColor;

void main()
{
/*	if(dot(gl_PointCoord-0.5,gl_PointCoord-0.5)>0.25)
	{
		discard;
	}
	else
	{

	*/
		causticColor = vec4(vec3(1.0), 1.0);
//	}
}
