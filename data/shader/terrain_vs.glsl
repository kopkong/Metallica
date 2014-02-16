#version 420

layout(location = 0) in vec2 Position;
layout(location = 1) in vec2 Texcoord; 

out vec2 Tex;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(Position.x,100.0,Position.y,1.0) ;
	Tex = Texcoord;
}