#version 130
//The vertex shader operates on each vertex of a given polygon

//in for input
in vec2 vertexPosition; 
in vec4 vertexColor;
in vec2 vertexUV;

out vec2 fragmentPosition;
out vec4 fragmentColor;
out vec2 fragmentUV;

void main(){
//gl_Position is a name for the position of the output
	gl_Position.xy = vertexPosition; 
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;
	
	fragmentColor = vertexColor;
	fragmentPosition = vertexPosition;
	fragmentUV = vec2(vertexUV.x, 1.0-vertexUV.y);
}