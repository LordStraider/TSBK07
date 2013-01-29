#version 150

in  vec3 in_Position;
uniform mat4 myTranslationMatrix;
uniform mat4 myRotationMatrix;
uniform mat4 myRotationMatrix2;
uniform mat4 myProjMatrix;
in  vec3 in_Color;
out vec3 ex_Color;

void main(void)
{
	ex_Color = in_Color;
	gl_Position = myProjMatrix * myTranslationMatrix * myRotationMatrix * myRotationMatrix2 * vec4(in_Position, 1.0);
}
