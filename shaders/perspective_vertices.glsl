#version 330 core

// Inputs
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTex;

// Uniforms
uniform mat4 proj;
uniform mat4 world_to_camera;
uniform mat4 mesh_local_to_world;

// Outputs
out vec3 color;
out vec2 tex;

void main(){
    gl_Position = proj * world_to_camera * mesh_local_to_world * vec4(inPos, 1.0);
    color = inColor;
    tex = inTex;
}
