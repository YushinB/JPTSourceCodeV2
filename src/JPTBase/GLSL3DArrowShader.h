#pragma once

#pragma once
const GLchar* vertex3DArrowShader = R"glsl(
	#version 330 compatibility

	layout (location = 0) in vec3 position;
	layout (location = 1) in vec3 normal;
	layout (location = 2) in vec4 color;
	
	out FTranfer
	{
		vec4 Fcolor;
		vec3 norm;
	} ftranfer;	

	uniform vec4 ClipPlane;

	void main()
	{
		gl_Position = vec4(position, 1.0f);
		//vec4 viewPos = model * vec4(position, 1.0f);
		ftranfer.norm = normal;
		ftranfer.Fcolor = color;//vec4(1,1,0.0,1.0);

		//gl_ClipDistance[0] = dot(ClipPlane, viewPos);
		//float testClipping = dot(ClipPlane, viewPos);
		//if(testClipping < 0)
		//ftranfer.Fcolor = vec4(1,1,1,0.1);
	}
)glsl";

const GLchar* Geo3DArrowShader = R"glsl(
	#version 330 compatibility
	
	#extension GL_EXT_gpu_shader4 : enable
	#extension GL_EXT_geometry_shader4 : enable
	#extension GL_NV_shader_buffer_load : enable

	layout(points) in;
	//layout(triangle_strip, max_vertices = 11) out;
	layout(triangle_strip, max_vertices = 112) out;
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
	const float ARROW_HEAD_ANGLE = 10.0 * M_PI / 180.0;

	mat4 GetRotMat(vec3 axis, float dAngle)
	{
		float dRadAngle = M_PI * dAngle / 180;

		float cosTheta = cos(dRadAngle);
		float sinTheta = sin(dRadAngle);
		mat4 Rotmat;
		Rotmat[0][0] = cosTheta + axis[0] * axis[0] * (1 - cosTheta);
		Rotmat[0][1] = axis[0] * axis[1] * (1 - cosTheta) - axis[2] * sinTheta;
		Rotmat[0][2] = axis[0] * axis[2] * (1 - cosTheta) + axis[1] * sinTheta;
		Rotmat[0][3] = 0.0;
		Rotmat[1][0] = axis[1] * axis[0] * (1 - cosTheta) + axis[2] * sinTheta;
		Rotmat[1][1] = cosTheta + axis[1] * axis[1] * (1 - cosTheta);
		Rotmat[1][2] = axis[1] * axis[2] * (1 - cosTheta) - axis[0] * sinTheta;
		Rotmat[1][3] = 0.0;
		Rotmat[2][0] = axis[2] * axis[0] * (1 - cosTheta) - axis[1] * sinTheta;
		Rotmat[2][1] = axis[2] * axis[1] * (1 - cosTheta) + axis[0] * sinTheta;
		Rotmat[2][2] = cosTheta + axis[2] * axis[2] * (1 - cosTheta);
		Rotmat[2][3] = 0.0;
		Rotmat[3][0] = 0.0;
		Rotmat[3][1] = 0.0;
		Rotmat[3][2] = 0.0;
		Rotmat[3][3] = 1.0;
		return Rotmat;
	}

	vec3 GetRotPos(vec3 tagpos, vec3 orgpos, vec3 axis, float dAngle)
	{
		mat4 rot = 	GetRotMat(axis, dAngle);
		tagpos = tagpos - orgpos;
		vec4 TagPts = vec4(tagpos, 1.0);
		vec4 outPts = rot*TagPts;
		tagpos = outPts.xyz;
		tagpos = tagpos + orgpos;
		return tagpos;
	}

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
		vec3 refAxis;
		refAxis = cross(GlobAxis[iAxis],Dir);
		refAxis = normalize(refAxis);		
		vec3 ArrowPts[4];
		float ddist = tan(ARROW_HEAD_ANGLE)*fhead;
		ArrowPts[0] = origin + refAxis*ddist*0.4;
		ArrowPts[1] = headPts + refAxis*ddist*0.4;
		ArrowPts[2] = headPts + refAxis*ddist;
		// 30 pts
		vec3 tmpPts;
		for (int i = 0; i <= 9; i++)
		{
			gl_Position = projection * modelview*vec4(origin, 1);
			EmitVertex();

			tmpPts = GetRotPos(ArrowPts[0], origin, Dir, i*36);
			gl_Position = projection * modelview*vec4(tmpPts, 1);
			EmitVertex();
			
			tmpPts = GetRotPos(ArrowPts[0], origin, Dir, (i+1)*36);
			gl_Position = projection * modelview*vec4(tmpPts, 1);
			EmitVertex();
		}
		//22 pts
		for (int i = 0; i <= 10; i++)
		{
			tmpPts = GetRotPos(ArrowPts[0], origin, Dir, i*36);
			gl_Position = projection * modelview*vec4(tmpPts, 1);
			EmitVertex();

			tmpPts = GetRotPos(ArrowPts[1], headPts, Dir, i*36);
			gl_Position = projection * modelview*vec4(tmpPts, 1);
			EmitVertex();				
		}
		// 30 pts
		for (int i = 0; i <= 9; i++)
		{
			gl_Position = projection * modelview*vec4(headPts, 1);
			EmitVertex();

			tmpPts = GetRotPos(ArrowPts[2], headPts, Dir, i*36);
			gl_Position = projection * modelview*vec4(tmpPts, 1);
			EmitVertex();
			
			tmpPts = GetRotPos(ArrowPts[2], headPts, Dir, (i+1)*36);
			gl_Position = projection * modelview*vec4(tmpPts, 1);
			EmitVertex();
		}
		// 30 pts		
		for (int i = 0; i <= 9; i++)
		{
			tmpPts = GetRotPos(ArrowPts[2], target, Dir, i*36);
			gl_Position = projection * modelview*vec4(tmpPts, 1);
			EmitVertex();
			
			gl_Position = projection * modelview*vec4(target, 1);
			EmitVertex();

			tmpPts = GetRotPos(ArrowPts[2], target, Dir, (i+1)*36);
			gl_Position = projection * modelview*vec4(tmpPts, 1);
			EmitVertex();
		}	
		EndPrimitive();
	};
)glsl";


const GLchar* fragment3DArrowShader = R"glsl(
	#version 330 compatibility
	
	out vec4 FragColor;

	//in vec2 TexCoord;
	in vec4 fagcolor;
	
	//uniform sampler2D texture1;
	//uniform sampler2D texture2;

	void main()
	{
		//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
		FragColor = fagcolor;
	}
)glsl";