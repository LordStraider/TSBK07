#version 150

out vec4 out_Color;
in  vec3 ex_Color;

void main(void)
{
	out_Color = vec4(ex_Color, 1.0);
}
