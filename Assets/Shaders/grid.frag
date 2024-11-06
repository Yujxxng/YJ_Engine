#version 450 core // Fragment shader code goes here...

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform vec4 uColor;
uniform float uTileSize;
uniform sampler2D ourTexture;

void main()
{
	vec4 C0 = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 C1 = vec4(0.7, 0.7, 0.7, 1.0);

	float size = 60;
	//int x = int(gl_FragCoord.x) / size;
	//int y = int(gl_FragCoord.y) / size;
	int x = int(TexCoord.x * 900.f / size);
	int y = int(TexCoord.y * 780.f / size);
	int c = (x + y) % 2;

	if(c == 0)
		FragColor = texture(ourTexture, TexCoord) * uColor * C0;
	else
		FragColor = texture(ourTexture, TexCoord) * uColor * C1;
}