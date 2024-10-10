#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform vec4 uColor;
uniform sampler2D ourTexture;

void main()
{
	//FragColor = vec4(ourColor, 1.0);
	FragColor = texture(ourTexture, TexCoord) * uColor;
}