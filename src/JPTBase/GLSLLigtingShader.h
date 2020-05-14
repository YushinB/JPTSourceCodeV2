#pragma once
#define STRINGIFY(A)  #A

const char *Lighting_vert = STRINGIFY(
	#version 330 core\n

	layout(location = 0) in vec3 position;
	layout(location = 1) in vec3 normal;
	layout(location = 2) in vec4 color;

	uniform mat4 modelview;
	uniform mat4 projection;
	uniform mat4 RotateMatrix;
	uniform vec3 lightPosition = vec3(0.f, 0.f, 1.f);
	uniform vec3 viewPos;

	out fData
	{
		vec4 color;
		vec3 Normal;
		vec4 lightpos;
		vec4 FragPos;
		vec3 viewPoint;
	}frag;

	void main()
	{
		vec4 pos = RotateMatrix*vec4(position, 1.0);
		gl_Position = projection * modelview * pos;
		frag.color = color;
		frag.Normal = mat3(transpose(inverse(modelview))) * normal;
		frag.lightpos = modelview*vec4(lightPosition, 1.0);
		frag.FragPos = modelview*pos;
		frag.viewPoint = viewPos;
	}
);


const char *Lighting_frag = STRINGIFY(
	#version 330 core \n
	out vec4 FragColor;

	uniform vec3 lightColor = vec3(1.f, 0.f, 0.f);
	
	in fData
	{
		vec4 color;
		vec3 Normal;
		vec4 lightpos;
		vec4 FragPos;
		vec3 viewPoint;
	}frag;

	void main()
	{
		// ambient
		float ambientStrength = 0.1;
		vec3 ambient = ambientStrength * lightColor;

		// diffuse 
		vec3 norm = normalize(frag.Normal);
		vec3 lightDir = normalize(frag.lightpos.xyz - frag.FragPos.xyz);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * lightColor;
		
		float specularStrength = 0.5;
		vec3 viewDir = normalize(frag.viewPoint - frag.FragPos.xyz);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
		vec3 specular = specularStrength * spec * lightColor;

		vec3 result = (ambient + diffuse + specular) * frag.color.rgb;

		FragColor = vec4(result, 1.0);
	}
);

const char *lamp_vert = STRINGIFY(
	#version 330 core\n

	layout(location = 0) in vec3 position;

	uniform mat4 modelview;
	uniform mat4 projection;
	uniform mat4 RotateMatrix;

	void main()
	{
		vec4 pos = RotateMatrix*vec4(position, 1.0);
		gl_Position = projection * modelview * pos;
	}
);


const char *lamp_frag = STRINGIFY(
	#version 330 core \n
	out vec4 FragColor;
	
	uniform vec3 ObjectColor = vec3(1.f,1.f,1.f);

	void main()
	{
		FragColor = vec4(ObjectColor, 1.0);
	}
);

