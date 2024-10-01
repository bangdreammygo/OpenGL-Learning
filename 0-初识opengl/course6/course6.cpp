#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"Shader.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//用这个宏来启动
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float move = 0.0f;


//检测窗口大小改变的函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//检查键盘输入的函数
void processInput(GLFWwindow* window);

// 画面配置相关
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


int main()
{


    // glfw: 初始化启动opengl
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // 创建窗口

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "first trangle", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //注册回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 启动glad来辅助管理opengl的函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    //生成shader对象
     Shader shader("shaders/trangles.vs","shaders/trangles.fs");

    // 准备要渲染的正方体的顶点
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

    //顶点颜色
    float colors[] = {
        1.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,
        0.0f,0.0f,1.0f,
        0.0f,0.0f,0.0f
    };
    

    //顺序绘制
    unsigned int indices[] = {  
        0, 1, 2 ,
        0, 2, 3
    };
    unsigned int VBO,CVBO,TVBO, VAO, EBO;
    //创建opengl内置对象，第一个参数表示生成的对象数量
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);  
    glGenBuffers(1, &CVBO);
    glGenBuffers(1,&TVBO);

    //绑定VAO
    glBindVertexArray(VAO);

    //从这一刻起，我们使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)。
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数。它的第一个参数是目标缓冲的类型：顶点缓冲对象当前绑定到GL_ARRAY_BUFFER目标上。第二个参数指定传输数据的大小(以字节为单位)；用一个简单的sizeof计算出顶点数据大小就行。第三个参数是我们希望发送的实际数据。
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //绑定EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //填充数据
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //激活这号位置的描述信息
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(1);

    //告诉opengl怎么解析数据
    //第一个参数 0 表示只需要位置值0的这个属性
    //第二个参数指定顶点属性的大小，vec3有三个值，所以大小为3
    //第三个参数指定数据类型
    //第四个参数定义是否希望被标准化
    //第五个参数时步长，告诉我们连续的顶点属性间的间隔，由于下个数据在3个float之后（只包含3个顶点坐标的数据嘛）
    //最后一个参数时表示数据在缓冲中的起始偏移量
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, CVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //显式解绑（其实不用，但是逻辑上更完整，也更合理）
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    //纹理坐标
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
    //设置纹理坐标轴
    //第一个参数指出纹理目标为2D纹理，第二个参数指出配置wrap选项（环绕方式）和轴（s,t,r对应于x,y,z轴），第三个参数是选择环绕方式（这里设置为镜像环绕）
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    //ps:如果选to_border类型的话，如下，还需要额外指定一下边缘颜色
    //float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    //设置纹理过滤信息
    //配置多级渐远纹理
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //不可以为放大来配置多级渐远纹理
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //读取图片
    int width, height, nrChannels;
    //第一个参数：图片路径，第二三四个参数：宽度，高度，颜色通道个数
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("container.jpg",&width,&height,&nrChannels,0);

    //生成纹理，和opengl对象一样，纹理也是用id来引用的
    unsigned int texture;
    glGenTextures(1,&texture);

    //绑定纹理
    glBindTexture(GL_TEXTURE_2D,texture);

    //使用此前存储的图片生成纹理
    //第一个参数，指定纹理目标（这里是2D的目标），第二个参数为多级渐远纹理的配置，第三个是纹理存储格式，第四第五个是高度宽度，第六个必须是0，第七八个定义了源图的格式和数据类型，第九个是真正的图像数据
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //生成多级渐远纹理
    glGenerateMipmap(GL_TEXTURE_2D);
    //释放内存
    stbi_image_free(data);

    //应用纹理
    glBindBuffer(GL_ARRAY_BUFFER,TVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(texCoords),texCoords,GL_STATIC_DRAW);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);

    //纹理单元
    //激活纹理单元
    //激活纹理单元之后，接下来的glBindTexture函数调用会绑定这个纹理到当前激活的纹理单元，纹理单元GL_TEXTURE0默认总是被激活，所以我们在前面的例子里当我们使用glBindTexture的时候，无需激活任何纹理单元
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texture);
    //重新加载一张纹理
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

    //绑定(第二个)纹理
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    shader.use();
    //告诉着色器，每个采样器对应于哪个纹理单元
    glUniform1i(glGetUniformLocation(shader.ID,"ourTexture"),0);
    shader.setInt("texture2",1);


    /////////////////////////////////////////////////////////////////进入三维
    ///////////////////投影矩阵
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f),(float)SCR_WIDTH/(float)SCR_HEIGHT,0.1f,100.0f);//强转float



    //解绑VAO
    glBindVertexArray(0);

    /////////////////开启深度测试
    glEnable(GL_DEPTH_TEST);

    // 循环渲染
    while (!glfwWindowShouldClose(window))
    {
        // 检查键盘输入
        processInput(window);

        // 进行渲染
        //设置清除颜色
        glClearColor(0.1f, 0.33f, 0.43f, 1.0f);
        //以及清除之前留下的深度测试数据
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 进行三角形的绘制

        //获取运行秒数
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;



        //绑定激活着色器程序对象
        shader.use();
        //获取uniform变量的位置
        int vertexColorLocation = glGetUniformLocation(shader.ID, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        ////////////////////////////////////////模型矩阵
        glm::mat4 model;
        model = glm::rotate(glm::mat4(1.0f),(float)glfwGetTime() *glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        ///////////////////观察矩阵
        glm::mat4 view;
        view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f+move));

        ////////////////////////////////传入uniform
        int modelLoc = glGetUniformLocation(shader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(shader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projectionLoc = glGetUniformLocation(shader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        //绑定VAO
        glBindVertexArray(VAO);
        //进行图片绘制
        //第一个参数：图形类型，第二个参数，要画几个顶点，第三个参数索引类型，第四个参数是EBO中的偏移量
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //第一个参数：图形类型，第二个参数，起始索引，第三个参数，顶点数目
        glDrawArrays(GL_TRIANGLES,0,36);


        // 交换缓冲区填色
        glfwSwapBuffers(window);
        glfwPollEvents();
        shader.unuse();
    }

    // 回收资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader.ID);

    // 释放资源，退出渲染程序
    glfwTerminate();
    return 0;
}

// 检查键盘的空格键是否被按下
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
               move+=0.2f;
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        move-=0.2f;
}

// 一个回调函数，如果窗口大小被改变时触发回调
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //视口变换函数
    glViewport(0, 0, width, height);
}