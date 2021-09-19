#version 460 compatibility
#pragma debug(on)

// in vec3 position;
// in vec2 texCoord;

out vec2 textureCoord;

void main()
{
  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
  gl_FrontColor = gl_Color;
  textureCoord = gl_Position.xy;
  textureCoord = vec2(0.5, 0.5);
}