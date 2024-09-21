#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>



//检测窗口大小改变的函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//检查键盘输入的函数
void processInput(GLFWwindow* window);

// 画面配置相关
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//顶点着色器源码
//location数据提供给shader去vao的第n个属性中去寻找数据
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 3) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//片段着色器源码（表示最终颜色）
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.82f, 0.65f, 0.42f, 0.8f);\n"
"}\n\0";

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


    // 着色器配置
    //存储顶点着色器
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //绑定着色器并编译它
    //glShaderSource函数把要编译的着色器对象作为第一个参数。第二参数指定了传递的源码字符串数量，这里只有一个。第三个参数是顶点着色器真正的源码
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // 检查着色器编译成功与否
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 片段着色器相关配置和编译
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // 检查片段着色器编译成功与否
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    

    //将着色器链接为一个着色器程序对象
    unsigned int shaderProgram = glCreateProgram();
    //链接顶点
    glAttachShader(shaderProgram, vertexShader);
    //链接片段
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // 查看是否链接成功
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    //删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 准备要渲染的三角形的三个顶点
    float vertices[] = {
         0.5f,  0.5f, 0.0f, 
         0.5f, -0.5f, 0.0f,  
         0.0f, -0.5f, 0.0f,
         0.0f, -0.5f, 0.0f,
         -0.5f, 0.5f, 0.0f,
    };

    //顺序绘制
    unsigned int indices[] = {  
        0, 1, 2 ,2,1,4
    };
    unsigned int VBO, VAO, EBO;
    //创建opengl内置对象，第一个参数表示生成的对象数量
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

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

    //告诉opengl怎么解析数据
    //第一个参数 0 表示只需要位置值0的这个属性
    //第二个参数指定顶点属性的大小，vec3有三个值，所以大小为3
    //第三个参数指定数据类型
    //第四个参数定义是否希望被标准化
    //第五个参数时步长，告诉我们连续的顶点属性间的间隔，由于下个数据在3个float之后（只包含3个顶点坐标的数据嘛）
    //最后一个参数时表示数据在缓冲中的起始偏移量
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

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
        //激活程序对象
        glUseProgram(shaderProgram);
        //绑定VAO
        glBindVertexArray(VAO);
        //进行图片绘制
        //第一个参数：图形类型，第二个参数，要画几个顶点，第三个参数索引类型，第四个参数是EBO中的偏移量
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        //另一种绘制方式
        //glDrawArrays(GL_TRIANGLES,0,3);



        // 交换缓冲区填色
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 回收资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

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