#pragma once
const GLchar* vertexSource = R"glsl(
	#version 330 compatibility

	#extension GL_EXT_gpu_shader4 : enable

	layout (location = 0) in vec3 position;
	layout (location = 1) in vec3 normal;
	layout (location = 2) in vec4 color;
	//layout (location = 3) in vec2 aTexCoord;

	out vData
	{
		vec4 color;
	}vertices;
	
	uniform vec4 lightDir;
	void main()
	{
		
		gl_Position = vec4(position, 1.0f);
		
		float ndotl = dot(normal, lightDir.xyz);
		vec3 light1 = vec3(0.0, 0.0, 1.0);
		ndotl = dot(normal, light1);
		vec3 light2 = vec3(1.0, 0.0, 0.0);
		ndotl += dot(normal, light2);

		ndotl = clamp(ndotl, 0.3, 1.0) + 0.1;
		vertices.color.rgb = color.rgb * ndotl;
		vertices.color.a = color.a;
	}
)glsl";