#pragma once  
const GLchar* GeoArrowShader = R"glsl(
	#version 330 compatibility
	
	#extension GL_EXT_gpu_shader4 : enable
	#extension GL_EXT_geometry_shader4 : enable
	#extension GL_NV_shader_buffer_load : enable

	layout(points) in;
	layout(line_strip, max_vertices = 9) out;
	//layout(points, max_vertices = 1) out;
	out vec4 fagcolor;
	
	in FTranfer
	{
		vec4 Fcolor;
		vec3 norm;
	} ftranfer[];

	uniform mat4 modelview;
	uniform mat4 projection;
	uniform float fscale;
    const float M_PI = 3.14159265358979323846;
	const float ARROW_HEAD_ANGLE = 15.0 * M_PI / 180.0;

	void main()
	{
		vec3 origin = gl_PositionIn[0].xyz;
		vec3 Dir = ftranfer[0].norm;
		fagcolor = ftranfer[0].Fcolor;

		vec3 target = origin + Dir * fscale;
		float fhead = 0.3* fscale;
		vec3 headPts = target - Dir*fhead;
		
		vec3 GlobAxis[3];
		GlobAxis[0] = vec3(1, 0, 0);
		GlobAxis[1] = vec3(0, 1, 0);
		GlobAxis[2] = vec3(0, 0, 1);
	
		int iAxis = 0;
		float len = length(Dir);
		for (int i = 0; i < 3; i++)
		{
			float ddotVal = dot(GlobAxis[i], Dir);
			if (abs(abs(ddotVal) - len) > 1e-6) {
				iAxis = i;
				break;
			}		
		}
		vec3 refAxis[2];
		vec3 ArrowPts[4];
		refAxis[0] = cross(GlobAxis[iAxis],Dir);
		refAxis[0] = normalize(refAxis[0]);
		refAxis[1] = cross(refAxis[0], Dir);
		refAxis[1] = normalize(refAxis[1]);
		
		float ddist = tan(ARROW_HEAD_ANGLE)*fhead;
		ArrowPts[0] = headPts + refAxis[0]*ddist;
		ArrowPts[1] = headPts - refAxis[0]*ddist;
		ArrowPts[2] = headPts + refAxis[1]*ddist;
		ArrowPts[3] = headPts - refAxis[1]*ddist;
		
		gl_Position = projection * modelview*vec4(origin, 1);
		EmitVertex();		
		for (int i=0; i<4; i++)
		{
			gl_Position = projection * modelview*vec4(target , 1);
			EmitVertex();	
			gl_Position = projection * modelview*vec4(ArrowPts[i], 1);
			EmitVertex();			
		}
	};
)glsl";