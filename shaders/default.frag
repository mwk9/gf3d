#version 450
#extension GL_ARB_separate_shader_objects : enable

<<<<<<< HEAD

layout(location = 0) out vec4 outColor; 
layout(location = 0) in vec3 fragColor; 

void main()
{
    outColor = vec4(fragColor, 1.0); 
=======
layout(binding = 1) uniform sampler2D texSampler;
layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;


void main()
{
    outColor = texture(texSampler, fragTexCoord);
>>>>>>> in_progress
}
