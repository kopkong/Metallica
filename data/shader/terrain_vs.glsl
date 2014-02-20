#version 420

layout(location = 0) in vec2 Position;
layout(location = 1) in vec2 Texcoord; 

float MAP_SCALE	= 10.0;

out vec2 Tex;

uniform mat4 MVP;
uniform sampler2D HeightMap;

void main()
{
	float u = Position.x;
	float v = Position.y;
	float adjustY = texture2D(HeightMap,vec2(u/32.0,v/32.0)).x * 256.0;

	// remember the first element is Posiont is X, and the second is Z
	gl_Position = MVP * vec4(u * MAP_SCALE,adjustY,v * MAP_SCALE,1.0) ;
	Tex = Texcoord;
}