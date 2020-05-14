#pragma once  
const GLchar* fragmentArrowShader = R"glsl(
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