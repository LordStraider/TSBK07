#version 150

in  vec3 in_Position;
uniform mat4 myMatrix;
uniform mat4 myTranslationMatrix;

void main(void)
{
	gl_Position = myMatrix * myTranslationMatrix *  vec4(in_Position, 1.0);
}
