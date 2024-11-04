//
// Created by XOS on 24-11-4.
//

#ifndef POWERUP_H
#define POWERUP_H

#pragma once
#include "Position.h"
#include <string>

class PowerUp {
public:
    enum class Type {
        SPEED_BOOST,     // 加速
        GHOST_FREEZER,   // 冻结幽灵
        POINT_MULTIPLIER,// 分数加倍
        WALL_PASS,       // 穿墙能力
        GHOST_VACUUM,    // 吸引幽灵
        TIME_SLOW        // 减慢时间
    };

    struct Effect {
        int duration;         // 持续时间（毫秒）
        float magnitude;      // 效果强度
        std::string message; // 激活时显示的消息
    };

private:
    Type type;
    Position pos;
    bool active;
    int spawnTime;
    int displayDuration;  // 道具在地图上显示的时间
    char symbol;
    Effect effect;

public:
    PowerUp(Type t, Position p, int displayTime = 10000);

    Position getPosition() const { return pos; }
    bool isActive() const { return active; }
    void setActive(bool state) { active = state; }
    char getSymbol() const { return symbol; }
    Type getType() const { return type; }
    const Effect& getEffect() const { return effect; }

    void update(int currentTime);
    bool shouldRemove(int currentTime) const;
};

#endif //POWERUP_H
