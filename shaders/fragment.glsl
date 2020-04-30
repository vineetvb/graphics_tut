#version 330 core

// Inputs.
in vec3
color;
in vec2
tex;

// Uniforms.
uniform sampler2D
texture0_sampler;

uniform sampler2D
texture1_sampler;

uniform vec2 timeColor;

// Outputs.
out vec4 frag_color;

void main() {
//  frag_color = vec4(color, 1.0);
//  frag_color = texture(texture_sampler, tex) * vec4(color, 1.0);
  frag_color = mix(texture(texture1_sampler, tex), texture(texture0_sampler, tex), 0.3);
//  frag_color = texture(texture1_sampler, tex) * vec4(color, 1.0);

}
