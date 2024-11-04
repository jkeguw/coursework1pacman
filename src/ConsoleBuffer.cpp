//
// Created by XOS on 24-11-4.
//
#include "ConsoleBuffer.h"
#include <algorithm>
#include <stdexcept>

ConsoleBuffer::ConsoleBuffer(int width, int height)
    : width(width), height(height), currentBuffer(0) {
    // 获取标准输出句柄
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Failed to get console handle");
    }

    // 隐藏光标
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    // 设置控制台窗口和缓冲区大小
    COORD bufferSize = { static_cast<SHORT>(width), static_cast<SHORT>(height) };
    SMALL_RECT windowSize = { 0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(height - 1) };

    // 调整窗口大小
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    // 创建两个屏幕缓冲区
    for (int i = 0; i < 2; ++i) {
        screenBuffer[i] = CreateConsoleScreenBuffer(
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            CONSOLE_TEXTMODE_BUFFER,
            NULL
        );

        if (screenBuffer[i] == INVALID_HANDLE_VALUE) {
            throw std::runtime_error("Failed to create screen buffer");
        }

        // 设置缓冲区大小
        SetConsoleScreenBufferSize(screenBuffer[i], bufferSize);

        // 隐藏光标
        SetConsoleCursorInfo(screenBuffer[i], &cursorInfo);
    }

    // 分配缓冲区内存
    bufferData = new CHAR_INFO[width * height];
    clear();
}

ConsoleBuffer::~ConsoleBuffer() {
    // 恢复原始缓冲区
    SetConsoleActiveScreenBuffer(hConsole);

    // 关闭缓冲区句柄
    for (int i = 0; i < 2; ++i) {
        if (screenBuffer[i] != INVALID_HANDLE_VALUE) {
            CloseHandle(screenBuffer[i]);
        }
    }

    // 释放内存
    delete[] bufferData;
}

void ConsoleBuffer::clear() {
    for (int i = 0; i < width * height; ++i) {
        bufferData[i].Char.AsciiChar = ' ';
        bufferData[i].Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
    }
}

void ConsoleBuffer::draw(int x, int y, char ch) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return;
    }

    WORD attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
    // 设置不同字符的颜色
    switch (ch) {
        case 'C': // Pacman
            attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case 'G': // Ghost
            attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
            break;
        case '.': // Dot
            attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
            break;
        case 'o': // Power dot
            attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
            break;
        case '#': // Wall
            attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
    }

    bufferData[y * width + x].Char.AsciiChar = ch;
    bufferData[y * width + x].Attributes = attributes;
}

void ConsoleBuffer::drawString(int x, int y, const std::string& str) {
    for (size_t i = 0; i < str.length(); ++i) {
        if (x + i >= width) break;
        draw(x + i, y, str[i]);
    }
}

void ConsoleBuffer::drawMatrix(const std::vector<std::vector<char>>& matrix) {
    for (size_t y = 0; y < matrix.size(); ++y) {
        for (size_t x = 0; x < matrix[y].size(); ++x) {
            draw(x, y, matrix[y][x]);
        }
    }
}

void ConsoleBuffer::swap() {
    SMALL_RECT writeRegion = { 0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(height - 1) };
    COORD bufferSize = { static_cast<SHORT>(width), static_cast<SHORT>(height) };
    COORD bufferCoord = { 0, 0 };

    // 写入缓冲区
    WriteConsoleOutput(
        screenBuffer[currentBuffer],
        bufferData,
        bufferSize,
        bufferCoord,
        &writeRegion
    );

    // 切换活动缓冲区
    SetConsoleActiveScreenBuffer(screenBuffer[currentBuffer]);
    currentBuffer = 1 - currentBuffer;
}