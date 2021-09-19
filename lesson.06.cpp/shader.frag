#version 460

in vec2 textureCoord;
out vec4 textureColor;

uniform sampler2D textureIn;
uniform vec2 textureSize;

void main()
{
	vec2 uv = gl_FragCoord.xy;
	vec4 pixel1 = texture2D(textureIn, (uv + vec2(0,0)) / textureSize);
	vec4 pixel2 = texture2D(textureIn, (uv + vec2(1,1)) / textureSize);
	textureColor = (pixel1 + pixel2) * 0.501;
}