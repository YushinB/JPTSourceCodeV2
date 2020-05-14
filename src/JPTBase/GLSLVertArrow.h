#pragma once
const GLchar* vertexArrowShader = R"glsl(
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
		ftranfer.Fcolor = color;

		//gl_ClipDistance[0] = dot(ClipPlane, viewPos);
		//float testClipping = dot(ClipPlane, viewPos);
		//if(testClipping < 0)
			//ftranfer.Fcolor = vec4(1,1,1,0.1);
	}
)glsl";