#version 330 core

// Inputs.
in vec2 texCoord;
in vec3 color;

// Uniforms.
uniform vec2 timeColor;
uniform sampler2D atexture;

// Outputs.
out vec4 FragColor;

void main(){
  vec4 texColor = texture(atexture, texCoord);
  FragColor = texColor;
}