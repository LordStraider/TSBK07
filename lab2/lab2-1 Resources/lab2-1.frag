#version 150

out vec4 outColor;
in vec3 exNormal; // Phong

in vec4 exTexCoord;

void main(void)
{
	const vec3 light = vec3(0.8, 0.8, 0.88);
	float shade;
	
	shade = dot(normalize(exNormal), light);
	outColor = exTexCoord * vec4(shade, shade, shade, 1.0);
}
