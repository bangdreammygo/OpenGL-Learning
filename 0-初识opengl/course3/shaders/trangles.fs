#version 330 core
uniform vec4 ourColor;
out vec4 FragColor;
in vec3 Color;
void main(){
    FragColor = vec4(Color.x,ourColor.y,Color.z,1.0f);
}