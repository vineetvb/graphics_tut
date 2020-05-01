#version 330 core

// Inputs
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTex;

// Uniforms
uniform mat4 proj;
uniform mat4 camera;

// Outputs
out vec3 color;
out vec2 tex;

void main(){
    gl_Position = camera * vec4(inPos, 1.0);
//    gl_Position = vec4(inPos, 1.0);
    color = inColor;
    tex = inTex;
}
