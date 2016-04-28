#version 410
in vec2 vTexCoords;

out vec4 fragColour;

uniform sampler2D diffuseTexture;
uniform float alpha;

void main()
{
	vec4 colour = texture(diffuseTexture, vTexCoords);
	colour.a = colour.a * alpha;
	fragColour = colour;
}