#pragma once  


const GLchar* textureVS = R"glsl(
	#version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec4 aColor;
	layout (location = 2) in vec2 aTexCoord;

	out vec4 ourColor;
	out vec2 TexCoord;

	void main()
	{
		gl_Position = vec4(aPos, 1.0);
		ourColor = aColor;
		TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	}
)glsl";

const GLchar* textureFS = R"glsl(
	#version 330 core
	out vec4 FragColor;

	in vec4 ourColor;
	in vec2 TexCoord;

	// texture samplers
	uniform sampler2D texture1;
	uniform sampler2D texture2;

	void main()
	{
		// linearly interpolate between both textures (80% container, 20% awesomeface)
		//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.8);
		//FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1.0 - TexCoord.x, TexCoord.y)), 0.2);
		//FragColor = texture(texture1, TexCoord) * ourColor;
		FragColor = texture(texture1, TexCoord);
	}
)glsl";