#version 330 core

// Inputs.
in vec3
color;
in vec2
tex;

// Uniforms.
uniform sampler2D
texture_sampler;

// Outputs.
out vec4 frag_color;

void main() {
//  frag_color = vec4(color, 1.0);
  frag_color = texture(texture_sampler, tex) * vec4(color, 1.0);
}
