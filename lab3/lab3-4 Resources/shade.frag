#version 150


in vec3 exNormal;		//Model coordinates
in vec3 surf;			//Model coordinates
in vec3 exCam;			//World coordinates
uniform mat4 camMatrix;
uniform mat4 mdlMatrix;

out vec4 outColor;

uniform vec3 lightSourcesDirPosArr[4];	//World coordinates
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];




void main(void)
{	
	vec3 reflected;
	vec3 lightDirection;

	vec3 eyeDirection = normalize( - surf);//vec3(camMatrix * vec4(surf, 1.0)));
	vec3 norm = normalize(mat3(mdlMatrix) * exNormal);

	float diffuse, specularStrength;
	float shadeR = 0;
	float shadeG = 0;
	float shadeB = 0;

	for (int i = 0; i < 4; i++) {
		diffuse = dot(norm, normalize(lightSourcesDirPosArr[i]));
		diffuse = max(0.0, diffuse); // No negative light
		clamp(diffuse, 0, 1);

		specularStrength = 0.0;
		lightDirection = normalize(vec3(camMatrix * vec4(lightSourcesDirPosArr[i], 1.0)));// - vec3(mdlMatrix * vec4(surf, 1.0)));
		reflected = reflect(-lightDirection, norm);

		specularStrength = dot(reflected, eyeDirection);
		
		//if (dot(-lightDirection, norm) > 0.0) {
		//	specularStrength = max(specularStrength, 0.01);
			specularStrength = pow(specularStrength, specularExponent[i]);
		//}
		specularStrength = max(specularStrength, 0.0);

		shadeR += ( diffuse * 0.7 + specularStrength * 0.9) * lightSourcesColorArr[i].x;
		shadeG += ( diffuse * 0.7 + specularStrength * 0.9) * lightSourcesColorArr[i].y;
		shadeB += ( diffuse * 0.7 + specularStrength * 0.9) * lightSourcesColorArr[i].z;
	}

	shadeR = clamp(shadeR, 0, 1);
	shadeG = clamp(shadeG, 0, 1);
	shadeB = clamp(shadeB, 0, 1);

	outColor = vec4(shadeR, shadeG, shadeB, 1.0);
}
