#version 130
//The fragment shader operates on each pixel in a givne polygon

in vec4 fragmentColor;

//out for output
out vec4 color;

void main(){
	color = fragmentColor; //vec3 RGB color
}