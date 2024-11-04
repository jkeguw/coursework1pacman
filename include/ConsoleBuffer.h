//
// Created by XOS on 24-11-4.
//

#ifndef CONSOLEBUFFER_H
#define CONSOLEBUFFER_H

#pragma once
#include <windows.h>
#include <string>
#include <vector>

class ConsoleBuffer {
public:
    ConsoleBuffer(int width, int height);
    ~ConsoleBuffer();

    // 禁用拷贝
    ConsoleBuffer(const ConsoleBuffer&) = delete;
    ConsoleBuffer& operator=(const ConsoleBuffer&) = delete;

    // 缓冲区操作
    void clear();
    void draw(int x, int y, char ch);
    void drawString(int x, int y, const std::string& str);
    void drawMatrix(const std::vector<std::vector<char>>& matrix);
    void swap();

private:
    HANDLE hConsole;              // 控制台句柄
    HANDLE screenBuffer[2];       // 双缓冲区
    int currentBuffer;            // 当前活动缓冲区
    int width;                    // 缓冲区宽度
    int height;                   // 缓冲区高度
    CHAR_INFO* bufferData;        // 缓冲区数据

    void initializeScreenBuffer();
    void cleanupScreenBuffer();
};

#endif //CONSOLEBUFFER_H
