#version 330 core

// Inputs.
in vec3 color;

// Outputs.
out vec4 FragColor;

void main(){
  FragColor = vec4(color, 1.0);
}
