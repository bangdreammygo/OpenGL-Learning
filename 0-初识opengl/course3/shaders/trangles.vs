#version 330 core
layout (location = 3) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 Color;
void main(){
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
  Color = aColor;
}