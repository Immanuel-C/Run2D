#version 450

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 inColour;

void main() {
    outColor = vec4(inColour, 1.0);
}