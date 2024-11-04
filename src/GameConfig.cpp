//
// Created by XOS on 24-11-4.
//
#include "../include/GameConfig.h"

#include "GameConfig.h"

namespace GameConfig {
    const char WALL = '#';
    const char DOT = '.';
    const char POWER_DOT = 'o';
    const char EMPTY = ' ';
    const char PACMAN = 'C';
    const char GHOST = 'G';
    const char DOOR = '-';

    const int NORMAL_DOT_SCORE = 10;
    const int POWER_DOT_SCORE = 50;
    const int GHOST_SCORE = 200;

    const int GAME_SPEED = 100;
    const int GHOST_SPEED = 2;
    const int POWER_MODE_DURATION = 10000; // 10秒

    // 吃豆人初始位置（底部中间）
    const int PACMAN_START_X = 14;
    const int PACMAN_START_Y = 23;

    // 幽灵初始位置（中心）
    const int GHOST_HOME_X = 14;
    const int GHOST_HOME_Y = 14;
}