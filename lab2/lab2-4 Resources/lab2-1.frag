#version 150
 
in vec2 exTexCoord;
in vec3 exNormal; 
out vec4 outColor;
uniform sampler2D texUnit;


void main(void)
{
	const vec3 light = vec3(0.7, 0.7, 0.7);
	float shade;
	
	shade = dot(normalize(exNormal), light);

	outColor = texture(texUnit, exTexCoord) * vec4(shade, shade, shade, 1.0);
}
