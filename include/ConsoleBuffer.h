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
    ConsoleBuffer(int gameWidth, int gameHeight);
    ~ConsoleBuffer();

    ConsoleBuffer(const ConsoleBuffer&) = delete;
    ConsoleBuffer& operator=(const ConsoleBuffer&) = delete;

    void clear();
    void draw(int x, int y, char ch);
    void drawString(int x, int y, const std::string& str);
    void drawMatrix(const std::vector<std::vector<char>>& matrix);
    void swap();

private:
    HANDLE hConsole;
    HANDLE screenBuffer[2];
    int currentBuffer;
    int gameWidth;      // 游戏逻辑宽度
    int gameHeight;     // 游戏逻辑高度
    int displayWidth;   // 实际显示宽度（2倍gameWidth）
    int displayHeight;  // 实际显示高度
    CHAR_INFO* bufferData;

    // 添加字符映射表
    static const char CHAR_PACMAN = 'C';
    static const char CHAR_GHOST = 'M';
    static const char CHAR_DOT = '.';
    static const char CHAR_POWER_DOT = 'O';
    static const char CHAR_WALL = '#';
    static const char CHAR_EMPTY = ' ';

    // 获取字符的显示属性
    WORD getCharacterAttributes(char ch);
    // 获取字符的显示样式
    char getCharacterDisplay(char ch);
};

#endif //CONSOLEBUFFER_H
