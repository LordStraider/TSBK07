#version 150
 
in vec2 exTexCoord;
in vec3 exNormal; 
out vec4 outColor;
uniform sampler2D texUnit;
uniform sampler2D texUnit2;


void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;
	
	shade = dot(normalize(exNormal), light);
	shade = clamp(shade, 0, 1);

	outColor = (texture(texUnit, exTexCoord) + texture(texUnit2, exTexCoord)) * shade * vec4(1.0, 1.0, 1.0, 0.5);
	//outColor = vec4(1.0,1.0,1.0, 0.5);
	// * vec4(shade, shade, shade, 1.0);
}
