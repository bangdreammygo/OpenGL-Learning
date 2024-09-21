#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>



//��ⴰ�ڴ�С�ı�ĺ���
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//����������ĺ���
void processInput(GLFWwindow* window);

// �����������
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//������ɫ��Դ��
//location�����ṩ��shaderȥvao�ĵ�n��������ȥѰ������
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 3) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//Ƭ����ɫ��Դ�루��ʾ������ɫ��
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.82f, 0.65f, 0.42f, 0.8f);\n"
"}\n\0";

int main()
{
    // glfw: ��ʼ������opengl
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // ��������

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "first trangle", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //ע��ص�����
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ����glad����������opengl�ĺ���ָ��
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // ��ɫ������
    //�洢������ɫ��
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //����ɫ����������
    //glShaderSource������Ҫ�������ɫ��������Ϊ��һ���������ڶ�����ָ���˴��ݵ�Դ���ַ�������������ֻ��һ���������������Ƕ�����ɫ��������Դ��
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // �����ɫ������ɹ����
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Ƭ����ɫ��������úͱ���
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // ���Ƭ����ɫ������ɹ����
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    

    //����ɫ������Ϊһ����ɫ���������
    unsigned int shaderProgram = glCreateProgram();
    //���Ӷ���
    glAttachShader(shaderProgram, vertexShader);
    //����Ƭ��
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // �鿴�Ƿ����ӳɹ�
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    //ɾ����ɫ������
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ׼��Ҫ��Ⱦ�������ε���������
    float vertices[] = {
         0.5f,  0.5f, 0.0f, 
         0.5f, -0.5f, 0.0f,  
         0.0f, -0.5f, 0.0f,
         0.0f, -0.5f, 0.0f,
         -0.5f, 0.5f, 0.0f,
    };

    //˳�����
    unsigned int indices[] = {  
        0, 1, 2 ,2,1,4
    };
    unsigned int VBO, VAO, EBO;
    //����opengl���ö��󣬵�һ��������ʾ���ɵĶ�������
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //��VAO
    glBindVertexArray(VAO);

    //����һ��������ʹ�õ��κΣ���GL_ARRAY_BUFFERĿ���ϵģ�������ö����������õ�ǰ�󶨵Ļ���(VBO)��
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData��һ��ר���������û���������ݸ��Ƶ���ǰ�󶨻���ĺ��������ĵ�һ��������Ŀ�껺������ͣ����㻺�����ǰ�󶨵�GL_ARRAY_BUFFERĿ���ϡ��ڶ�������ָ���������ݵĴ�С(���ֽ�Ϊ��λ)����һ���򵥵�sizeof������������ݴ�С���С�����������������ϣ�����͵�ʵ�����ݡ�
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //��EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //�������
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //�������λ�õ�������Ϣ
    glEnableVertexAttribArray(3);

    //����opengl��ô��������
    //��һ������ 0 ��ʾֻ��Ҫλ��ֵ0���������
    //�ڶ�������ָ���������ԵĴ�С��vec3������ֵ�����Դ�СΪ3
    //����������ָ����������
    //���ĸ����������Ƿ�ϣ������׼��
    //���������ʱ�������������������Ķ������Լ�ļ���������¸�������3��float֮��ֻ����3����������������
    //���һ������ʱ��ʾ�����ڻ����е���ʼƫ����
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //��ʽ�����ʵ���ã������߼��ϸ�������Ҳ������
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //���VAO
    glBindVertexArray(0);


    // ѭ����Ⱦ
    while (!glfwWindowShouldClose(window))
    {
        // ����������
        processInput(window);

        // ������Ⱦ
        //���������ɫ
        glClearColor(0.1f, 0.33f, 0.43f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ���������εĻ���
        //����������
        glUseProgram(shaderProgram);
        //��VAO
        glBindVertexArray(VAO);
        //����ͼƬ����
        //��һ��������ͼ�����ͣ��ڶ���������Ҫ���������㣬�����������������ͣ����ĸ�������EBO�е�ƫ����
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        //��һ�ֻ��Ʒ�ʽ
        //glDrawArrays(GL_TRIANGLES,0,3);



        // ������������ɫ
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ������Դ
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // �ͷ���Դ���˳���Ⱦ����
    glfwTerminate();
    return 0;
}

// �����̵Ŀո���Ƿ񱻰���
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// һ���ص�������������ڴ�С���ı�ʱ�����ص�
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //�ӿڱ任����
    glViewport(0, 0, width, height);
}