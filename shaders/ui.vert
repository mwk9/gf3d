#version 450
#extension GL_ARB_separate_shader_objects : enable

#define NUM_UI_COORDS 4

vec2 positions[NUM_UI_COORDS] = vec2[](
	vec2(-1.0, 1.0), //bottom-left
	vec2(1.0, 1.0), //bottom-right
	vec2(-1.0, -1.0), //top-left
	vec2(1.0, -1.0) //top-right
);

vec2 texturePositions[NUM_UI_COORDS] = vec2[](
	vec2(0.0, 1.0),
	vec2(1.0, 1.0),
	vec2(0.0, 0.0),
	vec2(1.0, 0.0)
);

layout(location = 0) out vec2 texturePosition;

void main()
{
	gl_Position = vec4(positions[gl_VertexIndex], 0.0f, 1.0f);
	texturePosition = texturePositions[gl_VertexIndex];
}
