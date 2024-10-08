#version 330 core

uniform sampler2D ourTexture;
uniform vec4 ourColor;
uniform sampler2D texture2;

out vec4 FragColor;

in vec3 Color;
in vec2 TexCoord;


void main(){
    FragColor = (mix(texture(ourTexture,TexCoord),texture(texture2,TexCoord),0.2));
}