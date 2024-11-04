//
// Created by XOS on 24-11-4.
//

#ifndef POWERUPMANAGER_H
#define POWERUPMANAGER_H

#pragma once
#include "PowerUp.h"
#include <vector>
#include <memory>
#include <random>

class PowerUpManager {
private:
    std::vector<std::unique_ptr<PowerUp>> powerUps;
    std::vector<std::pair<PowerUp::Type, int>> activeEffects;  // 类型和结束时间
    std::random_device rd;
    std::mt19937 gen;
    int lastSpawnTime;
    int spawnInterval;
    float scoreMultiplier;
    float playerSpeed;
    float ghostSpeed;
    bool wallPassEnabled;

    Position getRandomPosition(const std::vector<std::vector<char>>& map);
    PowerUp::Type getRandomPowerUpType();

public:
    PowerUpManager();

    void update(int currentTime, const std::vector<std::vector<char>>& map);
    void spawnPowerUp(const std::vector<std::vector<char>>& map);
    bool checkCollision(const Position& playerPos);
    void activatePowerUp(PowerUp::Type type);
    void updateEffects(int currentTime);

    // 状态查询方法
    float getScoreMultiplier() const { return scoreMultiplier; }
    float getPlayerSpeed() const { return playerSpeed; }
    float getGhostSpeed() const { return ghostSpeed; }
    bool isWallPassEnabled() const { return wallPassEnabled; }
    const std::vector<std::unique_ptr<PowerUp>>& getPowerUps() const { return powerUps; }

    void reset();
};

#endif //POWERUPMANAGER_H
