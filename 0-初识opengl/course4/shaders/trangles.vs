#version 330 core
layout (location = 3) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 4) in vec2 aTexCoord;

out vec3 Color;
out vec2 TexCoord;


void main(){
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
  Color = aColor;
  TexCoord = aTexCoord;
}