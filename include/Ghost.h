//
// Created by XOS on 24-11-4.
//

#ifndef GHOST_H
#define GHOST_H

#pragma once
#include "Character.h"
#include "GameConfig.h"
#include <cstdlib>

class Ghost : public Character {
public:
    Ghost(int x, int y);
    void move(const std::vector<std::vector<char>>& map) override;

private:
    bool isValidMove(const Position& newPos, const std::vector<std::vector<char>>& map) const;
};

#endif //GHOST_H
