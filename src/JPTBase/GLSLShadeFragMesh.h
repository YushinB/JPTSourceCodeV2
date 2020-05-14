#pragma once  
const GLchar* fragmentSourceMesh = R"glsl(
	#version 330 compatibility
	out vec4 FragColor;

	//in vec2 TexCoord;
	in fData
	{
		//vec3 normal;
		vec4 color;
	}frag;

	
	//uniform sampler2D texture1;
	//uniform sampler2D texture2;

	void main()
	{
		//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
		if (frag.color.a < 0.5)
			discard;
		else
			FragColor = frag.color;
	}
)glsl";