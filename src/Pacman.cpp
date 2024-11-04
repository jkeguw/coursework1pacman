//
// Created by XOS on 24-11-4.
//
#include "../include/Pacman.h"

Pacman::Pacman(int x, int y) : Character(x, y, GameConfig::PACMAN) {}

void Pacman::move(const std::vector<std::vector<char>>& map) {
    if (!_kbhit()) return;

    int input = _getch();
    Position newPos = pos;

    switch (input) {
        case 'w': case 'W': newPos.y--; break;
        case 's': case 'S': newPos.y++; break;
        case 'a': case 'A': newPos.x--; break;
        case 'd': case 'D': newPos.x++; break;
        default: return;
    }

    if (isValidMove(newPos, map)) {
        pos = newPos;
    }
}

bool Pacman::isValidMove(const Position& newPos, const std::vector<std::vector<char>>& map) const {
    if (newPos.y < 0 || newPos.y >= static_cast<int>(map.size())) return false;
    if (newPos.x < 0 || newPos.x >= static_cast<int>(map[0].size())) return false;
    return map[newPos.y][newPos.x] != GameConfig::WALL;
}