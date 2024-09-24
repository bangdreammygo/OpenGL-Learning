#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"Shader.h"
#include <iostream>



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

    // 准备要渲染的三角形的三个顶点
    float vertices[] = {
         0.5f,  0.5f, 0.0f, 
         0.5f, -0.5f, 0.0f,  
         0.0f, -0.5f, 0.0f,
    };

    //顶点颜色
    float colors[] = {
        1.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,
        0.0f,0.0f,1.0f
    };

    //顺序绘制
    unsigned int indices[] = {  
        0, 1, 2 
    };
    unsigned int VBO,CVBO, VAO, EBO;
    //创建opengl内置对象，第一个参数表示生成的对象数量
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);  
    glGenBuffers(1, &CVBO);

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

    //解绑VAO
    glBindVertexArray(0);


    // 循环渲染
    while (!glfwWindowShouldClose(window))
    {
        // 检查键盘输入
        processInput(window);

        // 进行渲染
        //设置清除颜色
        glClearColor(0.1f, 0.33f, 0.43f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 进行三角形的绘制

        //获取运行秒数
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        
        //绑定激活着色器程序对象
        shader.use();
        //获取uniform变量的位置
        int vertexColorLocation = glGetUniformLocation(shader.ID, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        //绑定VAO
        glBindVertexArray(VAO);
        //进行图片绘制
        //第一个参数：图形类型，第二个参数，要画几个顶点，第三个参数索引类型，第四个参数是EBO中的偏移量
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);


        //另一种绘制方式
        //glDrawArrays(GL_TRIANGLES,0,3);

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
}

// 一个回调函数，如果窗口大小被改变时触发回调
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //视口变换函数
    glViewport(0, 0, width, height);
}