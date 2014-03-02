#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 textureCoord;

out vec2 texCoord;
uniform mat4 MVP;
uniform samplerBuffer InstancedOffset;

void main(){
	vec3 vertex_pos = vertexPosition_modelspace + texelFetch(InstancedOffset,gl_InstanceID).xyz;
	gl_Position =  MVP * vec4(vertex_pos,1);
	texCoord = textureCoord;
}