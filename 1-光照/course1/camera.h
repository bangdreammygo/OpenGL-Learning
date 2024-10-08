#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// 定义了相机移动的几种可能选项。用作抽象概念，以避免使用特定于窗口系统的输入方法。
//前后左右
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// 默认值

///////欧拉角的偏航角和俯仰角
const float YAW = -90.0f;
const float PITCH = 0.0f;
//////移动速度
const float SPEED = 2.5f;
//////鼠标变化的灵敏度
const float SENSITIVITY = 0.1f;
//////视野范围
const float ZOOM = 45.0f;


// 一个处理输入并计算相应欧拉角、向量和矩阵以供OpenGL使用的抽象相机类。
class Camera
{
public:
    // 相机属性
    glm::vec3 Position;//相机位置
    glm::vec3 Front;///相机的方向向量
    glm::vec3 Up;//相机上向量
    glm::vec3 Right;//相机右向量
    glm::vec3 WorldUp;//世界坐标的上向量
    // 欧拉角（在这里不需要滚转角）
    float Yaw;///偏航角（分配xz）
    float Pitch;///俯仰角（定下来y轴以及xz的长度）
    // 相机选项
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // 参数1：相机位置，参数二：向上向量（世界坐标下）参数三偏航角 参数四俯仰角
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    //另一套构造函数
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // 获取相机的观测矩阵（view）
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // 键盘移动
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
        ///////////////将视角固定在地面上别乱飞
        Position.y = 0.4f;
    }

    //读取鼠标的操作
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        //给出欧拉角的限制范围
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // 更新方向向量
        updateCameraVectors();
    }

    // 鼠标滚轮的输入
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    // 从新给出的欧拉角来计算相机的方向向量
    void updateCameraVectors()
    {
        // 计算新的前方
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // 重新计算右向量
        Right = glm::normalize(glm::cross(Front, WorldUp));  // 标准化向量
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
#endif