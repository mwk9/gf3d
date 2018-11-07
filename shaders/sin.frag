#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler;
layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

uniform vec3 lightDir;


void main()
{
	vec3 n = normalize(fragNormal);
	vec3 l = normalize(light_dir);
	float kd = clamp(dot(n, -l), 0.0, 1.0);
    outColor = texture(texSampler, fragTexCoord);
	vec3 color = kd * outColor;
	gl_FragColor = vec4(color, 1.0);
}
