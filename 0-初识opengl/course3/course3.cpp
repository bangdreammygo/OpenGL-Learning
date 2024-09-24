#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"Shader.h"
#include <iostream>



//��ⴰ�ڴ�С�ı�ĺ���
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//����������ĺ���
void processInput(GLFWwindow* window);

// �����������
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


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


    //����shader����
     Shader shader("shaders/trangles.vs","shaders/trangles.fs");

    // ׼��Ҫ��Ⱦ�������ε���������
    float vertices[] = {
         0.5f,  0.5f, 0.0f, 
         0.5f, -0.5f, 0.0f,  
         0.0f, -0.5f, 0.0f,
    };

    //������ɫ
    float colors[] = {
        1.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,
        0.0f,0.0f,1.0f
    };

    //˳�����
    unsigned int indices[] = {  
        0, 1, 2 
    };
    unsigned int VBO,CVBO, VAO, EBO;
    //����opengl���ö��󣬵�һ��������ʾ���ɵĶ�������
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);  
    glGenBuffers(1, &CVBO);

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
    glEnableVertexAttribArray(1);

    //����opengl��ô��������
    //��һ������ 0 ��ʾֻ��Ҫλ��ֵ0���������
    //�ڶ�������ָ���������ԵĴ�С��vec3������ֵ�����Դ�СΪ3
    //����������ָ����������
    //���ĸ����������Ƿ�ϣ������׼��
    //���������ʱ�������������������Ķ������Լ�ļ���������¸�������3��float֮��ֻ����3����������������
    //���һ������ʱ��ʾ�����ڻ����е���ʼƫ����
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, CVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

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

        //��ȡ��������
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        
        //�󶨼�����ɫ���������
        shader.use();
        //��ȡuniform������λ��
        int vertexColorLocation = glGetUniformLocation(shader.ID, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        //��VAO
        glBindVertexArray(VAO);
        //����ͼƬ����
        //��һ��������ͼ�����ͣ��ڶ���������Ҫ���������㣬�����������������ͣ����ĸ�������EBO�е�ƫ����
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);


        //��һ�ֻ��Ʒ�ʽ
        //glDrawArrays(GL_TRIANGLES,0,3);

        // ������������ɫ
        glfwSwapBuffers(window);
        glfwPollEvents();
        shader.unuse();
    }

    // ������Դ
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader.ID);

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