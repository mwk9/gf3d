#version 450
#extension GL_ARB_separate_shader_objects : enable

#layout: how we map from system to shader, shader to system
#location map to vertex shader, map to output


layout(location = 0) out vec4 outColor; #output four values
layout(location = 0) in vec3 fragColor; #taking fragment color as input, taking in three inputs

void main()
{
    outColor = vec4(fragColor, 1.0); #output rgb values from input, add alpha manually
}
