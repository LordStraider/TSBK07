#version 150


in vec3 exNormal;		//Model coordinates
in vec3 surf;			//Model coordinates
in vec3 exCam;			//World coordinates
in mat3 exCamMatrix;	
in mat3 exMdlMatrix;	

out vec4 outColor;

uniform vec3 lightSourcesDirPosArr[4];	//World coordinates
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];




void main(void)
{
	vec3 cam = exCamMatrix * exCam;
	vec3 norm = normalize(exCamMatrix * exNormal);
	vec3 pos = exCamMatrix * exMdlMatrix * surf;

/*	const vec3 light = vec3(0.58, 0.58, 0.58); // Given in VIEW coordinates!
	float diffuse, specular, shade;
	
	// Diffuse
	diffuse = dot(normalize(exNormal), light);
	diffuse = max(0.0, diffuse); // No negative light
	
	// Specular
	vec3 r = reflect(-light, normalize(exNormal));
	vec3 v = normalize(-surf); // View direction
	specular = dot(r, v);
	if (specular > 0.0)
		specular = 1.0 * pow(specular, 150.0);
	specular = max(specular, 0.0);
	shade = 0.7*diffuse + 1.0*specular;
	outColor = vec4(shade, shade, shade, 1.0);
//*/
	

	vec3 eyeDirection = normalize(cam - pos);
	vec3 reflected;
	vec3 lightDirection;
	float specularStrength;
	float shadeR = 0;//diffuse * 0.2;
	float shadeG = 0;//diffuse * 0.2;
	float shadeB = 0;//diffuse * 0.2;
	
	for (int i = 0; i < 4; i++) {
		specularStrength = 0.0;
		lightDirection = exCamMatrix * lightSourcesDirPosArr[i];
		reflected = reflect(-lightDirection, exNormal);

		specularStrength = dot(reflected, eyeDirection);
		
		if (dot(lightDirection, exNormal) > 0.0) {
			specularStrength = max(specularStrength, 0.01);
			specularStrength = pow(specularStrength, specularExponent[i]);
		}
//		specularStrength = max(specularStrength, 0.0);

		shadeR += specularStrength * 0.09;// * lightSourcesColorArr[i].x;
		shadeG += specularStrength * 0.09;// * lightSourcesColorArr[i].y;
		shadeB += specularStrength * 0.09;// * lightSourcesColorArr[i].z;
	}

	shadeR = clamp(shadeR, 0, 1);
	shadeG = clamp(shadeG, 0, 1);
	shadeB = clamp(shadeB, 0, 1);

	outColor = vec4(shadeR, shadeG, shadeB, 1.0);
//*/
/*
	vec3 s;
	vec3 h;
	float alpha;
	float ispec;

	for (int i = 0; i < 4; i++) {
		ispec = 0.0;
		s = lightSourcesDirPosArr[i];

		h = (s + exCam) / abs(s + exCam);

		alpha = dot(normalize(exNormal), normalize(h));

		ispec = pow(alpha, 50);

		shadeR += ispec * 0.6 * lightSourcesColorArr[i].x;
		shadeG += ispec * 0.6 * lightSourcesColorArr[i].y;
		shadeB += ispec * 0.6 * lightSourcesColorArr[i].z;
	}

	shadeR = clamp(shadeR, 0, 1);
	shadeG = clamp(shadeG, 0, 1);
	shadeB = clamp(shadeB, 0, 1);
	outColor = vec4(shadeR, shadeG, shadeB, 1.0);
//*/
//*/
}
