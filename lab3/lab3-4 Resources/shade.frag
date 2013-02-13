#version 150
 
in vec3 exNormal; 
in vec3 surf;
out vec4 outColor;


/*uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];
*/

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;
	
	shade = dot(normalize(exNormal), light);
	shade = clamp(shade, 0, 1);


	/* Shaders? */
/*	vec3 reflectedLightDirection = reflect(-ligntSourceDirPosArr[0], 1);
	vec3 eyeDirection = Normailize(-surf);

	float specularStrength = 0.0;
	if (dot(igntSourceDirPosArr[0], 1) > 0.0) {
		specularStrength = dot(reflectedLightDirection, eyeDirection);
		float exponent = 200.0;
		specularStrength = max(specularStrength, 0.01);
		specularStrength = pow(specularStrength, exponent);
	}

	outColor = vec4(specularStrength);*/
	outColor = vec4(1.0, 0.0, 0.0, 1.0) * vec4(shade, shade, shade, 1.0);
}
