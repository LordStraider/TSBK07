#version 150

in  vec3 in_Position;
uniform mat4 myTranslationMatrix;
uniform mat4 myRotationMatrix;
in  vec3 in_Color;
out vec3 ex_Color;

void main(void)
{
	ex_Color = in_Color;
	gl_Position = myRotationMatrix * myTranslationMatrix * vec4(in_Position, 1.0);
}
