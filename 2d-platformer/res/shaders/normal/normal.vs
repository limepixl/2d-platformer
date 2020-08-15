#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

uniform mat4 PV;

void main()
{
	texCoord = aTexCoord;
	gl_Position = PV * vec4(aPos.xy, 0.0, 1.0);
}