//
// Created by XOS on 24-11-4.
//

#ifndef CHARACTER_H
#define CHARACTER_H

#pragma once
#include "Position.h"
#include <vector>

class Character {
protected:
    Position pos;
    char symbol;
public:
    Character(int x, int y, char sym);
    virtual ~Character();

    Position getPosition() const;
    char getSymbol() const;
    virtual void move(const std::vector<std::vector<char>>& map) = 0;
};

#endif //CHARACTER_H
