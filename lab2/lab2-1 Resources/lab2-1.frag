#version 150
 
in vec2 texCoord;
//in vec3 exNormal; 
out vec4 outColor;
//uniform sampler2D texUnit;


void main(void)
{

	//const vec3 light = vec3(0.7, 0.7, 0.7);
	//float shade;
	
	//shade = dot(normalize(exNormal), light);

	//outColor = texture(texUnit, exTexCoord) * vec4(shade, shade, shade, 1.0);;


	float a = sin(texCoord.s*30)/2+0.5;
	float b = sin(texCoord.t*30)/2+0.5;
	outColor = vec4(a,b,1.0,0.0);
}
