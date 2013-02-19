#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in  vec2 inTexCoord;
in 	vec3 inCam;

out vec3 exNormal;
out vec3 surf;
out vec3 exCam;
out mat3 exCamMatrix;
out mat3 exMdlMatrix;

uniform mat4 mdlMatrix;
uniform mat4 camMatrix;
uniform mat4 projMatrix;

void main(void)
{
	surf = inPosition;
	exCam = inCam;

	exCamMatrix = mat3(camMatrix);
	exMdlMatrix = mat3(mdlMatrix);

	exNormal = mat3(mdlMatrix) * inNormal;
	
	gl_Position = projMatrix * camMatrix * mdlMatrix * vec4(inPosition, 1.0);
}
