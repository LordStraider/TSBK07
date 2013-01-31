#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;

//out vec3 exNormal;
out vec2 texCoord;

uniform mat4 rotationMatrixX;
uniform mat4 rotationMatrixY;
uniform mat4 rotationMatrixZ;
uniform mat4 translationMatrix;
uniform mat4 projMatrix;

void main(void)
{
	texCoord = inTexCoord;
	
	//mat3 normalMatrix = mat3(translationMatrix * rotationMatrixX * rotationMatrixY * rotationMatrixZ);
	//exNormal = normalMatrix * inNormal;

	gl_Position = projMatrix * translationMatrix * rotationMatrixX * rotationMatrixY * rotationMatrixZ * vec4(inPosition, 1.0);
}
