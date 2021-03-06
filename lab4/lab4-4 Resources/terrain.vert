#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec2 texCoord;
out vec3 exNormal;


// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;

void main(void)
{
	texCoord = inTexCoord;
	
	mat3 normalMatrix = mat3(mdlMatrix);
	exNormal = normalMatrix * inNormal;

	gl_Position = projMatrix * camMatrix * mdlMatrix * vec4(inPosition, 1.0);
}
