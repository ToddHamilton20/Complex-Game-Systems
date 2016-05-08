#version 410
layout(location=0) in vec2 position;
layout(location=1) in vec2 texCoords;

out vec2 vTexCoords;

uniform mat4 projectionView;
uniform mat4 model;

void main()
{
	vTexCoords = texCoords;
	gl_Position = projectionView * model * vec4(position, 0.0f, 1.0f);
}