//
// Created by XOS on 24-11-4.
//

#ifndef PACMAN_H
#define PACMAN_H

#pragma once
#include "Character.h"
#include "GameConfig.h"
#include <conio.h>

class Pacman : public Character {
public:
    Pacman(int x, int y);
    void move(const std::vector<std::vector<char>>& map) override;

private:
    bool isValidMove(const Position& newPos, const std::vector<std::vector<char>>& map) const;
};

#endif //PACMAN_H
