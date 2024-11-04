//
// Created by XOS on 24-11-4.
//
#include "../include/Ghost.h"

Ghost::Ghost(int x, int y) : Character(x, y, GameConfig::GHOST) {}

void Ghost::move(const std::vector<std::vector<char>>& map) {
    int direction = rand() % 4;
    Position newPos = pos;

    switch (direction) {
        case 0: newPos.y--; break;
        case 1: newPos.y++; break;
        case 2: newPos.x--; break;
        case 3: newPos.x++; break;
    }

    if (isValidMove(newPos, map)) {
        pos = newPos;
    }
}

bool Ghost::isValidMove(const Position& newPos, const std::vector<std::vector<char>>& map) const {
    if (newPos.y < 0 || newPos.y >= static_cast<int>(map.size())) return false;
    if (newPos.x < 0 || newPos.x >= static_cast<int>(map[0].size())) return false;
    return map[newPos.y][newPos.x] != GameConfig::WALL;
}