#version 460 compatibility
#pragma debug(on)

uniform sampler2D diffuse;
uniform sampler2D normals;
uniform vec2 lightPos[16];
uniform int lightCount;
uniform vec2 vmWindow;

in vec4 position;
in vec4 color;

void main()
{
	vec4 texDiffuse = texture2D(diffuse, gl_TexCoord[0].xy);
	vec4 texNormal = texture2D(normals, gl_TexCoord[0].xy);

	vec3 normalBase =  normalize(vec3(0.0, 0.0, 1.0));
	vec3 normal = normalize(texNormal.xyz * 2 - 1) * vec3(-1.0, 1.0, 1.0);

	float light = 0;
	for(int i = 0; i < lightCount; ++i) {
		vec3 pPos = vec3(gl_FragCoord.xy, 0.0);
		vec3 lPos = vec3(lightPos[i].x, vmWindow.y - lightPos[i].y, 20);
		float dist = distance(pPos, lPos);
		if(dist < 256.0) {
			float att = clamp(1.0 - dist*dist/(256.0*256.0), 0.0, 1.0); 
			att *= att;
			vec3 lightDir = normalize(pPos - lPos);
			float lightDiffuse = max(dot(normal, -lightDir), 0.0) * att;
			light += lightDiffuse;
		}
	}
	gl_FragColor = gl_Color * vec4(vec3(1.0, 1.0, 1.0) * (light  + 0.1), texDiffuse.a);
}
