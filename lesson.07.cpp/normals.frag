#version 460 compatibility
#pragma debug(on)

uniform sampler2D diffuse;
uniform sampler2D normals;

in vec4 color;
in vec3 normal;
in vec3 position;

uniform float time;

void main()
{
	vec4 lightAmbientDiffuse = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 lightSpecular = vec4(0.0, 0.0, 0.0, 1.0);

	vec3 eyeDir = normalize(position);
	vec4 texNormal = texture2D(normals, gl_TexCoord[0].xy);
	vec4 texDiffuse = texture2D(diffuse, gl_TexCoord[0].xy)*0.5 + 0.5;

   vec3 T = normalize(vec3(gl_NormalMatrix * vec3(1,0,0)));
   vec3 B = normalize(vec3(gl_NormalMatrix * vec3(0,1,0)));
   vec3 N = normalize(vec3(gl_NormalMatrix * vec3(normal)));
   mat3 TBN = mat3(T, B, N);

	vec3 normalFixed1 = normalize(gl_NormalMatrix * normal);
	vec3 normalFixed2 = normalize(gl_NormalMatrix * (texNormal.xyz * 2 - 1) * vec3(-1,1,1));
//	vec3 normalFixed = normalFixed2 * texAlpha.rgb + normalFixed1 * (1-texAlpha.rgb);
	vec3 normalFixed = normalFixed2;
//	if((gl_TexCoord[0].x - 0.5) * (gl_TexCoord[0].y - 0.5) < 0) {
//		normalFixed = normalFixed1;
//	}


//	vec3 lightPos = gl_LightSource[0].position.xyz;
	vec3 lightPos = vec3(1.5*sin(time*1.0), 1.0*cos(time*1.5), 0.5);

	vec3 lightDir = normalize(lightPos - position);
	lightAmbientDiffuse += gl_LightSource[0].ambient;
	lightAmbientDiffuse += gl_LightSource[0].diffuse * max(dot(normalFixed, lightDir), 0.0);

	vec3 r = normalize(reflect(lightDir, normalFixed));
	lightSpecular += gl_LightSource[0].specular * 
                  pow(max(dot(r, eyeDir), 0.0), gl_FrontMaterial.shininess);  

	vec4 texColor = vec4(color.rgb * texDiffuse.rgb, 1.0); 
	gl_FragColor  = texColor * (lightAmbientDiffuse + lightSpecular);
}