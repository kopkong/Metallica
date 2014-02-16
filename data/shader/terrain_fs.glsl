#version 420

in vec2 Tex;

out vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
	FragColor = texture2D(gSampler,Tex.xy);
}