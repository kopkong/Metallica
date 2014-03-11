#version 420

in vec3 TexCoord;
out vec4 FragColor;
uniform samplerCube CubeMap;

void main()
{
	FragColor = texture(CubeMap,TexCoord);
}