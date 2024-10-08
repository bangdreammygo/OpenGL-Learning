#version 330 core
in vec3 Normal;
/////////获取片段位置
in vec3 fragPos;


out vec4 FragColor;
  
/////////lightpos:光源的位置

uniform vec3 lightpos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
    /////////环境光
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    ///////漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightpos - fragPos);
    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = diff * lightColor;

    ///////镜面反射
    float specularStrength = 0.5;
    //计算观察视角
    vec3 viewDir = normalize(viewPos - fragPos);
    //反射向量
    vec3 reflectDir = reflect(-lightDir, norm);  
    //高光需要很小的角度才行
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  


    vec3 result = (ambient+diffuse+specular) * objectColor;
    FragColor = vec4(result, 1.0);
}