//
// Created by XOS on 24-11-4.
//

#ifndef POWERUPEFFECT_H
#define POWERUPEFFECT_H

#pragma once
#include <string>
#include <vector>
#include <windows.h>

class PowerUpEffect {
public:
    struct VisualEffect {
        int duration;
        WORD textAttribute;
        std::string animation;
    };

    static VisualEffect getEffectForPowerUp(PowerUp::Type type);
    static void renderEffect(const VisualEffect& effect, ConsoleBuffer& console, int x, int y);
};

#endif //POWERUPEFFECT_H
