#version 330 core

// Interpolated values from the vertex shaders
in vec2 texCoord;

uniform sampler2D modelTexture;
out vec4 FragColor;

void main(){
	FragColor = texture(modelTexture,texCoord.xy);

}