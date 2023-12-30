#version 460 core
layout(std140, binding = 0) uniform PerFrameData
{
	uniform mat4 MVP;
};
layout (location=0) in vec3 vPos;
layout (location=1) in vec3 vColor;
layout (location=0) out vec3 color;
void main()
{
    gl_Position = MVP * vec4(vPos, 1.0);
    color = vColor;
}