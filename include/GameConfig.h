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

    // 道具系统配置
    const int POWERUP_PERSIST_TIME = 30000;    // 道具持续显示时间（毫秒）
    const int POWERUP_SPAWN_INTERVAL = 15000;  // 道具生成间隔（毫秒）

    // 道具效果持续时间（毫秒）
    const int SPEED_BOOST_DURATION = 5000;
    const int GHOST_FREEZER_DURATION = 7000;
    const int POINT_MULTIPLIER_DURATION = 10000;
    const int WALL_PASS_DURATION = 4000;
    const int GHOST_VACUUM_DURATION = 8000;
    const int TIME_SLOW_DURATION = 6000;

    // 道具效果强度
    const float SPEED_BOOST_MULTIPLIER = 2.0f;
    const float POINT_MULTIPLIER_VALUE = 2.0f;
    const float TIME_SLOW_FACTOR = 0.5f;
}

#endif //GAMECONFIG_H
