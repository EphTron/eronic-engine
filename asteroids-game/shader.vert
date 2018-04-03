#version 330

uniform mat4 MVM;
uniform mat4 PM;

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec4 colour;
out vec4 colourOut;

// Simple test vertex shader
void main()
{
	// Pass colour to fragment shader (will be interpolated)
	colourOut = colour;

	// Generate model-view projection matrix
	mat4 MVPM = PM * MVM;

	// Translate position of vertex by MVP
	gl_Position = MVPM * vec4(vertexPos, 1);
}