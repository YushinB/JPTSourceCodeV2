#pragma once
const GLchar* GeometrySourceMesh = R"glsl(
	#version 330 compatibility
	
	#extension GL_EXT_gpu_shader4 : enable
	#extension GL_EXT_geometry_shader4 : enable
	#extension GL_NV_shader_buffer_load : enable

	layout(lines) in;
	layout(line_strip, max_vertices = 4) out;
		
	in vData
	{
		//vec3 normal;
		vec4 color;
	}vertices[];

	out fData
	{
		//vec3 normal;
		vec4 color;
	}frag;

	uniform mat4 modelview;
	uniform mat4 projection;
	uniform vec4 ClipPlane;

	void main()
	{
		float fres1 = dot(ClipPlane, gl_PositionIn[0]);
		float fres2 = dot(ClipPlane, gl_PositionIn[1]);
		
		int flag = 0;
		if (fres1 < 0)
			flag = flag  + 1;
		if (fres2 < 0)
			flag = flag  + 2;
		
		if(flag == 1){
			float fRatio = fres2/(fres2 - fres1);
			vec4 midPts = gl_PositionIn[0]*fRatio + gl_PositionIn[1]*(1 -fRatio);

			frag.color = vec4(1.0,1.0,1.0, 0.2);			
			gl_Position = projection * modelview*gl_PositionIn[0];
			EmitVertex();

			frag.color = vec4(1.0,1.0,1.0, 0.2);
			gl_Position = projection * modelview*midPts;
			EmitVertex();

			frag.color = vertices[1].color;		
			gl_Position = projection * modelview*midPts;
			EmitVertex();

			frag.color = vertices[1].color;
			gl_Position = projection * modelview*gl_PositionIn[1];
			EmitVertex();
		}
		else if(flag == 2){
			float fRatio = fres1/(fres1 - fres2);
			vec4 midPts = gl_PositionIn[1]*fRatio + gl_PositionIn[0]*(1 -fRatio);

			frag.color = vertices[0].color;
			gl_Position = projection * modelview*gl_PositionIn[0];
			EmitVertex();			

			frag.color = vertices[0].color;
			gl_Position = projection * modelview*midPts;
			EmitVertex();

			frag.color = vec4(1.0,1.0,1.0 ,0.2);
			gl_Position = projection * modelview*midPts;
			EmitVertex();

			frag.color = vec4(1.0,1.0,1.0 ,0.2);
			gl_Position = projection * modelview*gl_PositionIn[1];
			EmitVertex();
		}
		else if (flag == 0){
			frag.color = vertices[0].color;
			gl_Position = projection * modelview*gl_PositionIn[0];
			EmitVertex();

			frag.color = vertices[1].color;
			gl_Position = projection * modelview*gl_PositionIn[1];
			EmitVertex();		
		}
		else if(flag == 3){
			frag.color = vec4(1.0,1.0,1.0 ,0.2);
			gl_Position = projection * modelview*gl_PositionIn[0];
			EmitVertex();

			frag.color = vec4(1.0,1.0,1.0 ,0.2);
			gl_Position = projection * modelview*gl_PositionIn[1];
			EmitVertex();	
		}
		EndPrimitive();
	};
)glsl";