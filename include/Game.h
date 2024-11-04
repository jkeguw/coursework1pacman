//
// Created by XOS on 24-11-4.
//

#ifndef GAME_H
#define GAME_H

#pragma once
#include "Pacman.h"
#include "Ghost.h"
#include <vector>
#include <windows.h>
#include <iostream>

class Game {
private:
    std::vector<std::vector<char>> map;
    Pacman pacman;
    std::vector<Ghost> ghosts;
    int score;
    bool gameOver;

    void initializeMap();
    void clearScreen();
    void updateGame();
    void displayGame();

public:
    Game();
    void run();
};

#endif //GAME_H
