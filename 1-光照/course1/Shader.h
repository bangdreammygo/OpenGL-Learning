#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class   Shader {
public:
	//着色器程序的id
	unsigned int ID;

	//构造函数,用于读取着色器,输入顶点着色器和片段着色器
	Shader(const char*vertexPath,const char*fragmentPath) {
	     //记录着色器源码
		std::string vertexCode, fragmentCode;
		//读取着色器文件
		std::ifstream vShaderFile, fShaderFile;
		//保证ifsream对象可以抛出异常
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			//打开文件
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			//读取文件的缓冲内容到数据流
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			//关闭文件处理器
			vShaderFile.close();
			fShaderFile.close();
			//转换数据（即源码）到string里
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch(std::ifstream::failure e){
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		//编译着色器
		unsigned int vertex, fragment;
		int success;
		char infoLog[512];//打印日志信息

		//顶点着色器
		//创建顶点着色器
		vertex = glCreateShader(GL_VERTEX_SHADER);
		//绑定源码
		glShaderSource(vertex,1,&vShaderCode,NULL);
		//编译
		glCompileShader(vertex);
		//打印错误
		checkCompileErrors(vertex, "VERTEX");
		//片段着色器
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		//绑定源码
		glShaderSource(fragment,1,&fShaderCode,NULL);
		//编译
		glCompileShader(fragment);
		//打印错误
		checkCompileErrors(fragment, "FRAGMENT");
		//着色器程序
		ID = glCreateProgram();
		glAttachShader(ID,vertex);
		glAttachShader(ID,fragment);
		//链接
		glLinkProgram(ID);
		//打印链接错误
		checkCompileErrors(ID, "PROGRAM");
		//删除着色器，已经编译好了程序了
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	};


	//激活程序
	void use() {
		//使用着色器程序
		glUseProgram(ID);
	};

	//结束程序
	void unuse() {
		glUseProgram(0);
	}

	//uniform工具函数

	//设置bool类型的uniform对象
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	//设置int类型的uniform对象
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	//设置float类型的uniform对象
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	//设置vec3
	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	//设置mat4
	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	//析构函数，结束着色程序的使用
	~Shader() {
		glDeleteProgram(ID);
	}


	//检查错误
	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};