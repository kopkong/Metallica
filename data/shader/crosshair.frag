#version 330 core

uniform vec3 CrossHairColor;
out vec4 FragColor;

void main(){
	FragColor = vec4(CrossHairColor,1.0);
}