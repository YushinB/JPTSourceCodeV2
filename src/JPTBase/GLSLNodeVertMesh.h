#pragma once
const GLchar* vertexSourceMesh = R"glsl(
	#version 330 compatibility
	layout (location = 0) in vec3 position;
	layout (location = 1) in vec4 color;

	//out vec2 TexCoord;
	out vData
	{
		//vec3 normal;
		vec4 color;
	}vertex;
	
	void main()
	{
		gl_Position = vec4(position, 1.0f);
		vertex.color = color;
	}
)glsl";