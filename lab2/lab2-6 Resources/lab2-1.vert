#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;

out vec2 exTexCoord;

uniform mat4 mdlMatrix;
uniform mat4 camMatrix;
uniform mat4 projMatrix;

out vec4 shading;

void main(void)
{
	exTexCoord = inTexCoord;
	
	mat3 normalMatrix = mat3(camMatrix * mdlMatrix);
	vec3 exNormal = normalMatrix * inNormal;

	gl_Position = projMatrix * camMatrix * mdlMatrix * vec4(inPosition, 1.0);


	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;
	
	shade = dot(normalize(exNormal), light);
	shade = clamp(shade, 0, 1);

	shading = vec4(shade, shade, shade, 1.0);
}
