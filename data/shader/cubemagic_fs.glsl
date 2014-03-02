#version 330 core

// Interpolated values from the vertex shaders
in vec2 texCoord;

uniform sampler2D Sampler2D;
out vec4 FragColor;

void main(){
	FragColor = texture(Sampler2D,texCoord.xy);

}