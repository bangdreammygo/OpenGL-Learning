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
	//��ɫ�������id
	unsigned int ID;

	//���캯��,���ڶ�ȡ��ɫ��,���붥����ɫ����Ƭ����ɫ��
	Shader(const char*vertexPath,const char*fragmentPath) {
	     //��¼��ɫ��Դ��
		std::string vertexCode, fragmentCode;
		//��ȡ��ɫ���ļ�
		std::ifstream vShaderFile, fShaderFile;
		//��֤ifsream��������׳��쳣
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			//���ļ�
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			//��ȡ�ļ��Ļ������ݵ�������
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			//�ر��ļ�������
			vShaderFile.close();
			fShaderFile.close();
			//ת�����ݣ���Դ�룩��string��
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch(std::ifstream::failure e){
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		//������ɫ��
		unsigned int vertex, fragment;
		int success;
		char infoLog[512];//��ӡ��־��Ϣ

		//������ɫ��
		//����������ɫ��
		vertex = glCreateShader(GL_VERTEX_SHADER);
		//��Դ��
		glShaderSource(vertex,1,&vShaderCode,NULL);
		//����
		glCompileShader(vertex);
		//��ӡ����
		checkCompileErrors(vertex, "VERTEX");
		//Ƭ����ɫ��
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		//��Դ��
		glShaderSource(fragment,1,&fShaderCode,NULL);
		//����
		glCompileShader(fragment);
		//��ӡ����
		checkCompileErrors(fragment, "FRAGMENT");
		//��ɫ������
		ID = glCreateProgram();
		glAttachShader(ID,vertex);
		glAttachShader(ID,fragment);
		//����
		glLinkProgram(ID);
		//��ӡ���Ӵ���
		checkCompileErrors(ID, "PROGRAM");
		//ɾ����ɫ�����Ѿ�������˳�����
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	};


	//�������
	void use() {
		//ʹ����ɫ������
		glUseProgram(ID);
	};

	//��������
	void unuse() {
		glUseProgram(0);
	}

	//uniform���ߺ���

	//����bool���͵�uniform����
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	//����int���͵�uniform����
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	//����float���͵�uniform����
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	//����vec3
	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	//����mat4
	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	//����������������ɫ�����ʹ��
	~Shader() {
		glDeleteProgram(ID);
	}


	//������
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