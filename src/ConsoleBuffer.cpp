//
// Created by XOS on 24-11-4.
//
#include "ConsoleBuffer.h"
#include <algorithm>
#include <stdexcept>

ConsoleBuffer::ConsoleBuffer(int gameWidth, int gameHeight)
    : gameWidth(gameWidth), gameHeight(gameHeight),
      displayWidth(gameWidth * 2), // 每个游戏字符占用两个显示字符
      displayHeight(gameHeight),
      currentBuffer(0) {

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
    COORD bufferSize = { static_cast<SHORT>(displayWidth), static_cast<SHORT>(displayHeight) };
    SMALL_RECT windowSize = { 0, 0, static_cast<SHORT>(displayWidth - 1), static_cast<SHORT>(displayHeight - 1) };

    // 设置字体
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 16;  // 字体宽度
    cfi.dwFontSize.Y = 16;  // 字体高度
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy(cfi.FaceName, L"Terminal");  // 使用Terminal字体
    SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);

    // 调整窗口大小
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    // 创建双缓冲
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

        SetConsoleScreenBufferSize(screenBuffer[i], bufferSize);
        SetConsoleCursorInfo(screenBuffer[i], &cursorInfo);
    }

    bufferData = new CHAR_INFO[displayWidth * displayHeight];
    clear();
}

WORD ConsoleBuffer::getCharacterAttributes(char ch) {
    WORD attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
    switch (ch) {
        case 'C': // Pacman
            attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case 'M': // Ghost
            attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
            break;
        case '.': // Dot
            attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
            break;
        case 'O': // Power dot
            attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
            break;
        case '#': // Wall
            attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
    }
    return attributes;
}

char ConsoleBuffer::getCharacterDisplay(char ch) {
    switch (ch) {
        case 'C': return 'O';  // 吃豆人用 O 表示
        case 'M': return 'M';  // 幽灵用 M 表示
        case '.': return '.';  // 小点保持不变
        case 'o': return 'O';  // 大能量豆用大写O
        case '#': return '#';  // 墙壁用 # 表示
        default: return ch;
    }
}

void ConsoleBuffer::draw(int x, int y, char ch) {
    if (x < 0 || x >= gameWidth || y < 0 || y >= gameHeight) {
        return;
    }

    WORD attributes = getCharacterAttributes(ch);
    char displayChar = getCharacterDisplay(ch);

    // 在显示缓冲区中占用两个字符位置
    int displayX = x * 2;
    int bufferIndex = y * displayWidth + displayX;

    // 设置第一个字符
    bufferData[bufferIndex].Char.AsciiChar = displayChar;
    bufferData[bufferIndex].Attributes = attributes;

    // 设置第二个字符（使用空格来扩展显示）
    bufferData[bufferIndex + 1].Char.AsciiChar = ' ';
    bufferData[bufferIndex + 1].Attributes = attributes;
}

void ConsoleBuffer::drawString(int x, int y, const std::string& str) {
    for (size_t i = 0; i < str.length(); ++i) {
        if (x + i >= gameWidth) break;
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
    SMALL_RECT writeRegion = {
        0,
        0,
        static_cast<SHORT>(displayWidth - 1),
        static_cast<SHORT>(displayHeight - 1)
    };
    COORD bufferSize = {
        static_cast<SHORT>(displayWidth),
        static_cast<SHORT>(displayHeight)
    };
    COORD bufferCoord = { 0, 0 };

    WriteConsoleOutput(
        screenBuffer[currentBuffer],
        bufferData,
        bufferSize,
        bufferCoord,
        &writeRegion
    );

    SetConsoleActiveScreenBuffer(screenBuffer[currentBuffer]);
    currentBuffer = 1 - currentBuffer;
}

void ConsoleBuffer::clear() {
    for (int i = 0; i < displayWidth * displayHeight; ++i) {
        bufferData[i].Char.AsciiChar = ' ';
        bufferData[i].Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
    }
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