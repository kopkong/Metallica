#version 420

layout(location = 0) in vec2 Position;
layout(location = 1) in vec2 Texcoord; 

out vec2 Tex;

void main()
{
	gl_Position = vec4(Position,10.0,1.0);
	Tex = Texcoord;
}