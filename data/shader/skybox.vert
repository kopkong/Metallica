#version 420

layout(location = 0) in vec3 Position;

uniform vec3 CameraPosition;
uniform mat4 ViewProjectionMatrix;

out vec3 TexCoord;

void main()
{
	TexCoord = vec3(Position.x,-Position.y,-Position.z);
	// vec4 newPos = vec4(2.0 * Position + CameraPosition, 1.0);
	// vec4 tanslatedPosition = ViewProjectionMatrix * Position ;
	vec4 tanslatedPosition = ViewProjectionMatrix * vec4(Position,1.0);
	gl_Position = tanslatedPosition.xyww;
}

