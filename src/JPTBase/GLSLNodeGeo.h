#pragma once
const GLchar* GeometrySource = R"glsl(
	#version 330 compatibility
	
	#extension GL_EXT_gpu_shader4 : enable
	#extension GL_EXT_geometry_shader4 : enable
	#extension GL_NV_shader_buffer_load : enable
	
	layout(triangles) in;
	layout(triangle_strip, max_vertices = 9) out;

	in vData
	{
		vec4 color;
	}vertices[];

	out fData
	{
		vec4 color;
	}frag;

	uniform mat4 modelview;
	uniform mat4 projection;
	uniform vec4 ClipPlane;

	const int Tri_Line_Cnt[8] = int[8](0, 1, 1, 2, 1, 2, 2, -1);
	const int Tri_Line_Node[6] = int[6](0, 1, 1, 2, 2, 0);
	const int Tri_Line_Table[16] = int[16](-1, -1, 4, 0, 0, 2, 2, 4, 4, 2, 2, 0, 0, 4,-1,-1);
	const int Tri_End[24] = int[24](-1,-1,-1, 0,2,1, 1,0,2, 2,1,0, 2,0,1, 1,2,0, 0,1,2, -1,-1,-1);

	void EmitVertexTriagle(vec4 color)
	{
		frag.color = color;
		gl_Position = projection * modelview*gl_PositionIn[0];
		EmitVertex();

		frag.color = color;
		gl_Position = projection * modelview*gl_PositionIn[1];
		EmitVertex();	

		frag.color = color;
		gl_Position = projection * modelview*gl_PositionIn[2];
		EmitVertex();
	}
	
	void main()
	{
		vec3 fres;
		fres[0] = dot(ClipPlane, gl_PositionIn[0]);
		fres[1] = dot(ClipPlane, gl_PositionIn[1]);
		fres[2] = dot(ClipPlane, gl_PositionIn[2]);	
		
		int flag = 0;
		if (fres[0] < 0) flag = flag  + 1;
		if (fres[1] < 0) flag = flag  + 2;
		if (fres[2] < 0) flag = flag  + 4;
		
        int flagcheck = Tri_Line_Cnt[flag];
		if(	flagcheck == 0)
        {
			EmitVertexTriagle(vertices[0].color);	
		}
		else if(flagcheck == -1)
        {
			EmitVertexTriagle(vec4(1.0,1.0,1.0 ,0.2));	
		}
		else {
			int idx11 = Tri_Line_Node[Tri_Line_Table[flag*2]];
			int idx12 = Tri_Line_Node[Tri_Line_Table[flag*2]+1];
			int idx21 = Tri_Line_Node[Tri_Line_Table[flag*2+1]];
			int idx22 = Tri_Line_Node[Tri_Line_Table[flag*2+1]+1];
			
			float fRatio1 = fres[idx12]/(fres[idx12] - fres[idx11]);
			float fRatio2 = fres[idx22]/(fres[idx22] - fres[idx21]);

			vec4 midPts1 = gl_PositionIn[idx11]*fRatio1 + gl_PositionIn[idx12]*(1 -fRatio1);
			vec4 midPts2 = gl_PositionIn[idx21]*fRatio2 + gl_PositionIn[idx22]*(1 -fRatio2);

			if(flagcheck == 1)
			{
				frag.color = vec4(1.0,1.0,1.0 ,0.2);
				gl_Position = projection * modelview*gl_PositionIn[Tri_End[flag*3]];
				EmitVertex();

				frag.color = vec4(1.0,1.0,1.0 ,0.2);
				gl_Position = projection * modelview*midPts1;
				EmitVertex();	

				frag.color = vec4(1.0,1.0,1.0 ,0.2);
				gl_Position = projection * modelview*midPts2;
				EmitVertex();


				frag.color = vertices[idx21].color;
				gl_Position = projection * modelview*midPts2;
				EmitVertex();

				frag.color = vertices[idx11].color;
				gl_Position = projection * modelview*midPts1;
				EmitVertex();	

				frag.color = vertices[Tri_End[flag*3+1]].color;
				gl_Position = projection * modelview*gl_PositionIn[Tri_End[flag*3+1]];
				EmitVertex();

				frag.color = vertices[Tri_End[flag*3+1]].color;
				gl_Position = projection * modelview*gl_PositionIn[Tri_End[flag*3+1]];
				EmitVertex();

				frag.color = vertices[Tri_End[flag*3+2]].color;
				gl_Position = projection * modelview*gl_PositionIn[Tri_End[flag*3+2]];
				EmitVertex();

				frag.color = vertices[idx21].color;
				gl_Position = projection * modelview*midPts2;
				EmitVertex();
			}
			else if(flagcheck == 2)
			{
				frag.color = vertices[Tri_End[flag*3]].color;
				gl_Position = projection * modelview*gl_PositionIn[Tri_End[flag*3]];
				EmitVertex();

				frag.color = vertices[idx11].color;
				gl_Position = projection * modelview*midPts1;
				EmitVertex();	

				frag.color = vertices[idx21].color;
				gl_Position = projection * modelview*midPts2;
				EmitVertex();


				frag.color = vec4(1.0,1.0,1.0 ,0.2);
				gl_Position = projection * modelview*midPts2;
				EmitVertex();

				frag.color = vec4(1.0,1.0,1.0 ,0.2);
				gl_Position = projection * modelview*midPts1;
				EmitVertex();	

				frag.color = vec4(1.0,1.0,1.0 ,0.2);
				gl_Position = projection * modelview*gl_PositionIn[Tri_End[flag*3+1]];
				EmitVertex();

				frag.color = vec4(1.0,1.0,1.0 ,0.2);
				gl_Position = projection * modelview*gl_PositionIn[Tri_End[flag*3+1]];
				EmitVertex();

				frag.color = vec4(1.0,1.0,1.0 ,0.2);
				gl_Position = projection * modelview*gl_PositionIn[Tri_End[flag*3+2]];
				EmitVertex();

				frag.color = vec4(1.0,1.0,1.0 ,0.2);
				gl_Position = projection * modelview*midPts2;
				EmitVertex();
			}			
		}
	};
)glsl";