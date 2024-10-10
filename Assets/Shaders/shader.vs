#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat3 uModel_to_NDC;
void main()
{
	gl_Position = vec4(vec2(uModel_to_NDC * vec3(aPos)), 0.0, 1.0);
	//gl_Position = uModel_to_NDC * vec4(aPos, 1.0f);
	//gl_Position = vec4(aPos.x/0.5f*0.0625f, aPos.y/0.5f*0.0833f, 0.0f, 1.0f);
	ourColor = aColor;
	TexCoord = aTexCoord;
}