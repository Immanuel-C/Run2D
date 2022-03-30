#version 450

vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec3 inColour;

layout(location = 0) out vec3 outColour;

void main() {
    gl_Position = vec4(inPos, 0.0, 1.0);
    outColour = inColour;
}