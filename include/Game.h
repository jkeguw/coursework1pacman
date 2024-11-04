//
// Created by XOS on 24-11-4.
//

#ifndef GAME_H
#define GAME_H

#pragma once
#include "Pacman.h"
#include "Ghost.h"
#include "ConsoleBuffer.h"
#include <vector>
#include <memory>

#pragma once
#include "Pacman.h"
#include "Ghost.h"
#include "ConsoleBuffer.h"
#include <vector>
#include <memory>

class Game {
public:
    // 使用constexpr来定义静态常量
    static constexpr int MAP_WIDTH = 28;      // 地图宽度
    static constexpr int MAP_HEIGHT = 31;     // 地图高度
    static constexpr int INFO_WIDTH = 20;     // 信息区域宽度
    static constexpr int TOTAL_WIDTH = MAP_WIDTH + INFO_WIDTH;  // 总宽度

private:
    std::vector<std::vector<char>> map;
    Pacman pacman;
    std::vector<Ghost> ghosts;
    std::unique_ptr<ConsoleBuffer> console;
    int score;
    bool gameOver;
    bool powerMode;
    int remainingDots;
    int level;

    void initializeMap();
    void updateGame();
    void displayGame();
    void drawInfoPanel();
    int countRemainingDots() const;
    std::string centerText(const std::string& text, int width) const;

    void handleGameOver();
    void displayGameOverScreen();

public:
    Game();
    void run();
};

#endif //GAME_H
