#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;

out vec3 exNormal;
out vec2 exTexCoord;

uniform mat4 mdlMatrix;
uniform mat4 projMatrix;

void main(void)
{
	exTexCoord = inTexCoord;
	
	mat3 normalMatrix = mat3(mdlMatrix);
	exNormal = normalMatrix * inNormal;

	gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);
}
