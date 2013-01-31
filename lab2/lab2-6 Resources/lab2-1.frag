#version 150
 
in vec2 exTexCoord;
out vec4 outColor;
in vec4 shading;
uniform sampler2D texUnit;


void main(void)
{
	

	outColor = texture(texUnit, exTexCoord) * shading;
}
