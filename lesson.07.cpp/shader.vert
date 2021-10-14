#version 460 compatibility
#pragma debug(on)

out vec4 color;
out vec3 normal;
out vec3 position;

void main()
{
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	color = vec4(0.7, 0.7, 0.7, 0.7);
//	normal = normalize(gl_NormalMatrix * gl_Normal);
	normal = gl_Normal;
	position = vec3(gl_ModelViewMatrix * gl_Vertex);
}