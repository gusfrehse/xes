#version 440 core
layout (location = 0) in vec2 pos;
out vec3 color;

void main()
{
	color = vec3(1.0, 1.0, 1.0);
	gl_Position = vec4(pos, 0.0, 1.0);
}
arandomstring
#version 440 core
out vec4 FragColor;

in vec3 color;

void main()
{
	FragColor = vec4(color, 1.0);
	//FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}