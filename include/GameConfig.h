//
// Created by XOS on 24-11-4.
//

#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#pragma once

namespace GameConfig {
    extern const char WALL;       // 墙壁
    extern const char DOT;        // 普通豆子
    extern const char POWER_DOT;  // 能量豆
    extern const char EMPTY;      // 空地
    extern const char PACMAN;     // 吃豆人
    extern const char GHOST;      // 幽灵
    extern const char DOOR;       // 幽灵门

    extern const int NORMAL_DOT_SCORE;    // 普通豆子分数
    extern const int POWER_DOT_SCORE;     // 能量豆分数
    extern const int GHOST_SCORE;         // 吃幽灵分数

    extern const int GAME_SPEED;          // 游戏速度
    extern const int GHOST_SPEED;         // 幽灵速度
    extern const int POWER_MODE_DURATION; // 能量模式持续时间

    // 初始位置配置
    extern const int PACMAN_START_X;
    extern const int PACMAN_START_Y;
    extern const int GHOST_HOME_X;
    extern const int GHOST_HOME_Y;
    //地图尺寸
    extern const int MAP_WIDTH;
    extern const int MAP_HEIGHT;
}

#endif //GAMECONFIG_H
