//
// Created by XOS on 24-11-4.
//
#include "PowerUp.h"

PowerUp::PowerUp(Type t, Position p, int displayTime)
    : type(t), pos(p), active(true), spawnTime(0), displayDuration(displayTime) {

    // 设置每种道具的效果
    switch (type) {
        case Type::SPEED_BOOST:
            effect = {5000, 2.0f, "SPEED UP!"};
        symbol = 'S';
        break;

        case Type::GHOST_FREEZER:
            effect = {7000, 1.0f, "FREEZE!"};
        symbol = 'F';
        break;

        case Type::POINT_MULTIPLIER:
            effect = {10000, 2.0f, "DOUBLE POINTS!"};
        symbol = 'P';
        break;

        case Type::WALL_PASS:
            effect = {4000, 1.0f, "WALL PASS!"};
        symbol = 'W';
        break;

        case Type::GHOST_VACUUM:
            effect = {8000, 1.0f, "GHOST VACUUM!"};
        symbol = 'V';
        break;

        case Type::TIME_SLOW:
            effect = {6000, 0.5f, "TIME SLOW!"};
        symbol = 'T';
        break;
    }
}

void PowerUp::update(int currentTime) {
    if (spawnTime == 0) {
        spawnTime = currentTime;
    }
}

bool PowerUp::shouldRemove(int currentTime) const {
    return currentTime - spawnTime > displayDuration;
}