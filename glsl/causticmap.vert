#version 330

uniform mat4 proj;
uniform mat4 view;
uniform vec3 light_pos;

layout(location = 0) in vec3 in_vertex;

uniform sampler2D texReceiverPos;
uniform sampler2D texSurfacePos;
uniform sampler2D texSurfaceNorm;
/*
vec3 estimateIntersection(vec3 v, vec3 r, mat4 vp)
{
	vec3 p1 = v + 1.0*r;
	vec4 texPt = vec4(p1, 1.0)*vp;
	vec2 tc = vec2(0.5*(texPt.xy/texPt.w)+vec2(0.5, 0.5));
	tc.y = 1.0 - tc.y;
	vec4 recPos = texture2D(texReceiverPos, tc);
	vec3 p2 = v + distance(v, recPos.xyz)*r;
	texPt = vec4(p2, 1.0)*vp;
	tc = vec2(0.5*(texPt.xy/texPt.w)+vec2(0.5, 0.5));
	tc.y = 1.0 - tc.y;

	return texture2D(texReceiverPos, tc).xyz;
}
*/
vec2 getTC(mat4 vp, vec3 pos)
{
	vec4 tp = vp*vec4(pos, 1.0);
	vec2 tc = vec2(0.5*(tp.xy/tp.w)+vec2(0.5, 0.5));
	tc.y = 1.0 - tc.y;
	return tc;
}

float estimateIntersection(vec3 pos, vec3 dir, mat4 vp, int num_iter)
{
	float eps = 0.1;
	vec2 tc = vec2(0, 0);

	// Initial guess
	float x_k = 0.10;
	for (int i = 0; i < num_iter; ++i)
	{
		// f(x_k)
		vec3 pos_p = pos + dir*x_k;
		tc = getTC(vp, pos_p);
		vec3 newPos1 = texture2D(texReceiverPos, tc).xyz;
		float f_x_k = distance(newPos1, pos_p);

		// f(x_k + eps)
		vec3 pos_q = pos + dir*(x_k+eps);
		tc = getTC(vp, pos_q);
		vec3 newPos2 = texture2D(texReceiverPos, tc).xyz;
		float f_x_k_eps = distance(newPos2, pos_q);

		float deriv = (f_x_k_eps - f_x_k)/eps;

		// Newton Raphson Iteration
		x_k = x_k - (f_x_k/deriv);
	}

	return x_k;
}

out vec3 out_vertex;

void main()
{
	mat4 vp = proj * view;

	vec3 world_vertex = texture2D(texSurfacePos, in_vertex.st).xyz;
	vec3 world_normal = normalize(texture2D(texSurfaceNorm, in_vertex.st)).xyz;
	// Calculate light direction
	vec3 light_dir = normalize(world_vertex - light_pos);

	vec3 r = normalize(refract(light_dir, world_normal, 0.75));
/*	vec3 point = world_vertex;
	for (int i = 0; i < 1; ++i)
	{
		point = estimateIntersection(point, r, vp);
	}
	*/
	float d = estimateIntersection(world_vertex, r, vp, 5);
	vec3 point = world_vertex + r*d;

	//point = point + r*10.0;

	out_vertex = point;

	gl_Position = vp*vec4(point, 1.0);
	//vec4(point, 1.0);
}
