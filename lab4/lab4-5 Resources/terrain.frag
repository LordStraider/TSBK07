#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 exNormal; 
uniform sampler2D tex;
uniform vec3 camPos;
uniform bool useSpec;

void main(void)
{
	const vec3 light = vec3(0.58, 1, 0.58);
	float shade;
	
//		diffuse = dot(norm, normalize(lightSourcesDirPosArr[i]));
	//shade = max(dot(normalize(exNormal), normalize(camPos)), 0.0);
	shade = max(dot(normalize(exNormal), normalize(light)), 0.0);
	clamp(shade, 0, 1);

	if (useSpec) {
		outColor = vec4(shade, shade, shade, 1.0);// * texture(tex, texCoord);
	} else {
		outColor = shade * texture(tex, texCoord);
	}
}
