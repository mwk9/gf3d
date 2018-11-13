#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler;
layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in float fragTime;

layout(location = 0) out vec4 outColor;

vec2 SineWave(vec2 p)
{
	float pi = 3.14159;
	float A = 0.15;
	float w = 10.0 * pi;
	float t = 30.0 * pi / 180.0;
	float y = sin(w * p.x + t) * A;
	return vec2(p.x, (p.y + y));
}

void main()
{
	vec2 uv = SineWave(fragTexCoord);
    outColor = texture(texSampler, uv);
}
