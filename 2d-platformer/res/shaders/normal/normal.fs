#version 330 core
out vec4 color;

in vec2 texCoord;

uniform sampler2D tex;

void main()
{
	color = texture(tex, texCoord);
	if(color.a < 0.05)
		discard;
}