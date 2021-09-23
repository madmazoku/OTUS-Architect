#version 460

in vec2 textureCoord;
out vec4 textureColor;

uniform sampler2D textureIn;
uniform vec2 textureSize;

void main()
{
	vec2 uv = gl_FragCoord.xy / textureSize;
	vec2 s = vec2(1.0, 1.0) / textureSize;
	vec4 pixel_lt = texture2D(textureIn, (uv + vec2(-s.x,-s.y)));
	vec4 pixel_mt = texture2D(textureIn, (uv + vec2(0,-s.y)));
	vec4 pixel_rt = texture2D(textureIn, (uv + vec2(s.x,-s.y)));
	vec4 pixel_lm = texture2D(textureIn, (uv + vec2(-s.x,0)));
	vec4 pixel_mm = texture2D(textureIn, (uv + vec2(0,0)));
	vec4 pixel_rm = texture2D(textureIn, (uv + vec2(s.x,0)));
	vec4 pixel_lb = texture2D(textureIn, (uv + vec2(-s.x,s.y)));
	vec4 pixel_mb = texture2D(textureIn, (uv + vec2(0,s.y)));
	vec4 pixel_rb = texture2D(textureIn, (uv + vec2(s.x,s.x)));

	int n = int(round(pixel_lt.x) + round(pixel_mt.x) + round(pixel_rt.x) + round(pixel_lm.x) + round(pixel_rm.x) + round(pixel_lb.x) + round(pixel_mb.x) + round(pixel_rb.x));

	int n_age = int(pixel_mm.x * 255.0);
	if(n_age != 0 && n_age != 128)
		--n_age;
	float f_age = n_age / 255.0;

	if(n == 2)
		textureColor = vec4(f_age,f_age,f_age,1.0);
	else if(n == 3)
		if(n_age >= 128)
			textureColor = vec4(f_age,f_age,f_age,1.0);
		else
			textureColor = vec4(1.0,1.0,1.0,1.0);
	else
		if(n_age >= 128)
			textureColor = vec4(0.5,0.5,0.5,1.0);
		else
			textureColor = vec4(f_age,f_age,f_age,1.0);
}