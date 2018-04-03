#version 330

uniform vec4 globalColour;

in vec4 colourOut;

void main()
{
   // Set the final colour
   gl_FragColor = colourOut * globalColour;
}