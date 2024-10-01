#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"Shader.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//�������������
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float move = 0.0f;


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

    // ׼��Ҫ��Ⱦ��������Ķ���
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f,  
     0.5f,  0.5f, -0.5f,  
     0.5f,  0.5f, -0.5f,  
    -0.5f,  0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f, 

    -0.5f, -0.5f,  0.5f,  
     0.5f, -0.5f,  0.5f, 
     0.5f,  0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f, 
    -0.5f,  0.5f,  0.5f,  
    -0.5f, -0.5f,  0.5f, 

    -0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f,  
    -0.5f, -0.5f,  0.5f, 
    -0.5f,  0.5f,  0.5f,  

     0.5f,  0.5f,  0.5f, 
     0.5f,  0.5f, -0.5f, 
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f, 
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f, 
     0.5f, -0.5f,  0.5f, 
     0.5f, -0.5f,  0.5f, 
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f, 

    -0.5f,  0.5f, -0.5f,  
     0.5f,  0.5f, -0.5f,  
     0.5f,  0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f, 
    -0.5f,  0.5f, -0.5f,
    };

    //������ɫ
    float colors[] = {
        1.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,
        0.0f,0.0f,1.0f,
        0.0f,0.0f,0.0f
    };
    

    //˳�����
    unsigned int indices[] = {  
        0, 1, 2 ,
        0, 2, 3
    };
    unsigned int VBO,CVBO,TVBO, VAO, EBO;
    //����opengl���ö��󣬵�һ��������ʾ���ɵĶ�������
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);  
    glGenBuffers(1, &CVBO);
    glGenBuffers(1,&TVBO);

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


    //��������
    float texCoords[] = {
      0.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,
      0.0f, 0.0f,

      0.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,
      0.0f, 0.0f,

      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,
      0.0f, 1.0f,
      0.0f, 0.0f,
      1.0f, 0.0f,

      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,
      0.0f, 1.0f,
      0.0f, 0.0f,
      1.0f, 0.0f,

      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
      1.0f, 0.0f,
      0.0f, 0.0f,
      0.0f, 1.0f,

      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
      1.0f, 0.0f,
      0.0f, 0.0f,
      0.0f, 1.0f
    };
    //��������������
    //��һ������ָ������Ŀ��Ϊ2D�����ڶ�������ָ������wrapѡ����Ʒ�ʽ�����ᣨs,t,r��Ӧ��x,y,z�ᣩ��������������ѡ���Ʒ�ʽ����������Ϊ�����ƣ�
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    //ps:���ѡto_border���͵Ļ������£�����Ҫ����ָ��һ�±�Ե��ɫ
    //float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    //�������������Ϣ
    //���ö༶��Զ����
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //������Ϊ�Ŵ������ö༶��Զ����
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //��ȡͼƬ
    int width, height, nrChannels;
    //��һ��������ͼƬ·�����ڶ����ĸ���������ȣ��߶ȣ���ɫͨ������
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("container.jpg",&width,&height,&nrChannels,0);

    //����������opengl����һ��������Ҳ����id�����õ�
    unsigned int texture;
    glGenTextures(1,&texture);

    //������
    glBindTexture(GL_TEXTURE_2D,texture);

    //ʹ�ô�ǰ�洢��ͼƬ��������
    //��һ��������ָ������Ŀ�꣨������2D��Ŀ�꣩���ڶ�������Ϊ�༶��Զ��������ã�������������洢��ʽ�����ĵ�����Ǹ߶ȿ�ȣ�������������0�����߰˸�������Դͼ�ĸ�ʽ���������ͣ��ھŸ���������ͼ������
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //���ɶ༶��Զ����
    glGenerateMipmap(GL_TEXTURE_2D);
    //�ͷ��ڴ�
    stbi_image_free(data);

    //Ӧ������
    glBindBuffer(GL_ARRAY_BUFFER,TVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(texCoords),texCoords,GL_STATIC_DRAW);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);

    //����Ԫ
    //��������Ԫ
    //��������Ԫ֮�󣬽�������glBindTexture�������û�����������ǰ���������Ԫ������ԪGL_TEXTURE0Ĭ�����Ǳ��������������ǰ��������ﵱ����ʹ��glBindTexture��ʱ�����輤���κ�����Ԫ
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texture);
    //���¼���һ������
    glActiveTexture(GL_TEXTURE1);
    unsigned int texture2;
    glad_glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D,texture2);
    unsigned char* data2 = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
    if (data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    }

    //��(�ڶ���)����
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    shader.use();
    //������ɫ����ÿ����������Ӧ���ĸ�����Ԫ
    glUniform1i(glGetUniformLocation(shader.ID,"ourTexture"),0);
    shader.setInt("texture2",1);


    /////////////////////////////////////////////////////////////////������ά
    ///////////////////ͶӰ����
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f),(float)SCR_WIDTH/(float)SCR_HEIGHT,0.1f,100.0f);//ǿתfloat



    //���VAO
    glBindVertexArray(0);

    /////////////////������Ȳ���
    glEnable(GL_DEPTH_TEST);

    // ѭ����Ⱦ
    while (!glfwWindowShouldClose(window))
    {
        // ����������
        processInput(window);

        // ������Ⱦ
        //���������ɫ
        glClearColor(0.1f, 0.33f, 0.43f, 1.0f);
        //�Լ����֮ǰ���µ���Ȳ�������
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ���������εĻ���

        //��ȡ��������
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;



        //�󶨼�����ɫ���������
        shader.use();
        //��ȡuniform������λ��
        int vertexColorLocation = glGetUniformLocation(shader.ID, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        ////////////////////////////////////////ģ�;���
        glm::mat4 model;
        model = glm::rotate(glm::mat4(1.0f),(float)glfwGetTime() *glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        ///////////////////�۲����
        glm::mat4 view;
        view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f+move));

        ////////////////////////////////����uniform
        int modelLoc = glGetUniformLocation(shader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(shader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projectionLoc = glGetUniformLocation(shader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        //��VAO
        glBindVertexArray(VAO);
        //����ͼƬ����
        //��һ��������ͼ�����ͣ��ڶ���������Ҫ���������㣬�����������������ͣ����ĸ�������EBO�е�ƫ����
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //��һ��������ͼ�����ͣ��ڶ�����������ʼ������������������������Ŀ
        glDrawArrays(GL_TRIANGLES,0,36);


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
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
               move+=0.2f;
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        move-=0.2f;
}

// һ���ص�������������ڴ�С���ı�ʱ�����ص�
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //�ӿڱ任����
    glViewport(0, 0, width, height);
}