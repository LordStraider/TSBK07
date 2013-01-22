#version 150

in  vec3 in_Position;
uniform mat4 myMatrix;
uniform mat4 myRotationMatrix;

void main(void)
{
	gl_Position = myMatrix * vec4(in_Position, 1.0);
}
