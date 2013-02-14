#version 150


in vec3 exNormal; 
in vec3 surf;
out vec4 outColor;



uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];




void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;
	
	shade = dot(normalize(exNormal), light);
	clamp(shade, 0, 1);


	vec3 eyeDirection = normalize(-surf);
	vec3 reflectedLightDirection;
	vec3 lightDirection;
	float specularStrength;
	float shadeR = 0.0;
	float shadeG = 0.0;
	float shadeB = 0.0;
	
	for (int i = 0; i < 4; i++) {
		lightDirection = lightSourcesDirPosArr[i];
		reflectedLightDirection = reflect(-lightDirection, exNormal);
		specularStrength = 0.0;
		
		if (dot(lightDirection, exNormal) > 0.0) {
			specularStrength = dot(reflectedLightDirection, eyeDirection);
			specularStrength = max(specularStrength, 0.01);
			specularStrength = pow(specularStrength, specularExponent[i]);
		}

		shadeR += shade * 0.2 + specularStrength * 0.5 * lightSourcesColorArr[i].x;
		shadeG += shade * 0.2 + specularStrength * 0.5 * lightSourcesColorArr[i].y;
		shadeB += shade * 0.2 + specularStrength * 0.5 * lightSourcesColorArr[i].z;
	}

	clamp(shadeR, 0, 1);
	clamp(shadeG, 0, 1);
	clamp(shadeB, 0, 1);

	outColor = vec4(shadeR, shadeG, shadeB, 1.0);

}
