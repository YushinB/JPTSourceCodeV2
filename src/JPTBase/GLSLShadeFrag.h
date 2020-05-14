#pragma once  
const GLchar* fragmentSource = R"glsl(
	#version 330 compatibility
	out vec4 FragColor;

	in fData
	{
		vec4 color;
	}frag;

	void main()
	{
		FragColor = frag.color;
	}
)glsl";