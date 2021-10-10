#version 460
#pragma debug(on)

out vec4 textureColor;

uniform sampler2D textureIn;
uniform vec2 textureSize;

void main()
{
	vec2 uv = gl_FragCoord.xy / textureSize;
	vec2 s = vec2(1.0, 1.0) / textureSize;
	vec2 base = floor(gl_FragCoord.xy * 0.25) * 4 / textureSize;

	vec4 pixel_00 = texture2D(textureIn, base + vec2(0, 0));
	vec4 pixel_10 = texture2D(textureIn, base + vec2(s.x, 0));
	vec4 pixel_20 = texture2D(textureIn, base + vec2(s.x*2, 0));
	vec4 pixel_30 = texture2D(textureIn, base + vec2(s.x*3, 0));

	vec4 pixel_01 = texture2D(textureIn, base + vec2(0, s.y));
	vec4 pixel_11 = texture2D(textureIn, base + vec2(s.x, s.y));
	vec4 pixel_21 = texture2D(textureIn, base + vec2(s.x*2, s.y));
	vec4 pixel_31 = texture2D(textureIn, base + vec2(s.x*3, s.y));

	vec4 pixel_02 = texture2D(textureIn, base + vec2(0, s.y*2));
	vec4 pixel_12 = texture2D(textureIn, base + vec2(s.x, s.y*2));
	vec4 pixel_22 = texture2D(textureIn, base + vec2(s.x*2, s.y*2));
	vec4 pixel_32 = texture2D(textureIn, base + vec2(s.x*3, s.y*2));

	vec4 pixel_03 = texture2D(textureIn, base + vec2(0, s.y*3));
	vec4 pixel_13 = texture2D(textureIn, base + vec2(s.x, s.y*3));
	vec4 pixel_23 = texture2D(textureIn, base + vec2(s.x*2, s.y*3));
	vec4 pixel_33 = texture2D(textureIn, base + vec2(s.x*3, s.y*3));

	if(gl_FragCoord.x < textureSize.x * 0.5 - 1.0) {
		textureColor = vec4(
			pixel_00 + pixel_10 + pixel_20 + pixel_30 +
			pixel_01 + pixel_11 + pixel_21 + pixel_31 +
			pixel_02 + pixel_12 + pixel_22 + pixel_32 +
			pixel_03 + pixel_13 + pixel_23 + pixel_33
		) / 16;
	} else {
		if(gl_FragCoord.x > textureSize.x * 0.5 + 1.0) {
			textureColor = texture2D(textureIn, uv);
		} else {
			textureColor = vec4(1.0, 0.0, 0.0, 1.0);
		}
	}
}