#version 130
//The fragment shader operates on each pixel in a givne polygon

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

//out for output
out vec4 color;

//uniform variable time. Uniform is like global variable
//uniform float time;
uniform sampler2D mySampler;

void main(){

	vec4 textureColor = texture(mySampler, fragmentUV);
	
	color = fragmentColor * textureColor;
	
	//color = vec4(fragmentColor.r + (cos(fragmentPosition.x*4.0 + time))*0.5,
	//			fragmentColor.g + (cos(fragmentPosition.y*8.0 + time))*0.5,
	//			fragmentColor.b + (cos(fragmentPosition.x*2.0 + time))*0.5, fragmentColor.a) * textureColor;
	//color = vec4(tan(fragmentPosition.y/fragmentPosition.x*time),tan(fragmentPosition.y/fragmentPosition.x*time),tan(fragmentPosition.y/fragmentPosition.x*time), fragmentColor.a);
}