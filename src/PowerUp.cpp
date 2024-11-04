//
// Created by XOS on 24-11-4.
//
#include "PowerUp.h"
#include "ConsoleBuffer.h"
#include "GameConfig.h"

PowerUp::PowerUp(Type t, Position p, int displayTime)
    : type(t), pos(p), active(true), spawnTime(GetTickCount()),
      displayDuration(GameConfig::POWERUP_PERSIST_TIME) {

    switch (type) {
        case Type::SPEED_BOOST:
            effect = {GameConfig::SPEED_BOOST_DURATION,
                     GameConfig::SPEED_BOOST_MULTIPLIER,
                     "SPEED UP!"};
        symbol = 'S';
        break;

        case Type::GHOST_FREEZER:
            effect = {GameConfig::GHOST_FREEZER_DURATION,
                     1.0f,
                     "FREEZE!"};
        symbol = 'F';
        break;

        case Type::POINT_MULTIPLIER:
            effect = {GameConfig::POINT_MULTIPLIER_DURATION,
                     GameConfig::POINT_MULTIPLIER_VALUE,
                     "DOUBLE POINTS!"};
        symbol = 'P';
        break;

        case Type::WALL_PASS:
            effect = {GameConfig::WALL_PASS_DURATION,
                     1.0f,
                     "WALL PASS!"};
        symbol = 'W';
        break;

        case Type::GHOST_VACUUM:
            effect = {GameConfig::GHOST_VACUUM_DURATION,
                     1.0f,
                     "GHOST VACUUM!"};
        symbol = 'V';
        break;

        case Type::TIME_SLOW:
            effect = {GameConfig::TIME_SLOW_DURATION,
                     GameConfig::TIME_SLOW_FACTOR,
                     "TIME SLOW!"};
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