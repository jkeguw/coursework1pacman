//
// Created by XOS on 24-11-4.
//
#include "PowerUpManager.h"
#include <algorithm>
#include "ConsoleBuffer.h"
#include "GameConfig.h"


PowerUpManager::PowerUpManager()
    : gen(rd()), lastSpawnTime(0),
      scoreMultiplier(1.0f), playerSpeed(1.0f), ghostSpeed(1.0f),
      wallPassEnabled(false) {}

void PowerUpManager::update(int currentTime, const std::vector<std::vector<char>>& map) {
    // 移除过期或被收集的道具
    powerUps.erase(
        std::remove_if(powerUps.begin(), powerUps.end(),
            [currentTime](const auto& powerUp) {
                return !powerUp->isActive() ||
                       (currentTime - powerUp->getSpawnTime() > GameConfig::POWERUP_PERSIST_TIME);
            }
        ),
        powerUps.end()
    );

    // 如果没有道具存在，且距离上次生成时间已超过间隔，则生成新道具
    if (powerUps.empty() && currentTime - lastSpawnTime > GameConfig::POWERUP_SPAWN_INTERVAL) {
        spawnPowerUp(map);
        lastSpawnTime = currentTime;
    }

    updateEffects(currentTime);
}

Position PowerUpManager::getRandomPosition(const std::vector<std::vector<char>>& map) {
    std::uniform_int_distribution<> distX(1, map[0].size() - 2);
    std::uniform_int_distribution<> distY(1, map.size() - 2);

    Position pos;
    do {
        pos.x = distX(gen);
        pos.y = distY(gen);
    } while (map[pos.y][pos.x] != ' ');

    return pos;
}

PowerUp::Type PowerUpManager::getRandomPowerUpType() {
    std::uniform_int_distribution<> dist(0, 5);
    return static_cast<PowerUp::Type>(dist(gen));
}

void PowerUpManager::spawnPowerUp(const std::vector<std::vector<char>>& map) {
    Position pos = getRandomPosition(map);
    PowerUp::Type type = getRandomPowerUpType();
    auto newPowerUp = std::make_unique<PowerUp>(type, pos, GameConfig::POWERUP_PERSIST_TIME);
    powerUps.push_back(std::move(newPowerUp));
}

bool PowerUpManager::checkCollision(const Position& playerPos) {
    for (auto& powerUp : powerUps) {
        if (powerUp->isActive() &&
            powerUp->getPosition().x == playerPos.x &&
            powerUp->getPosition().y == playerPos.y) {
            activatePowerUp(powerUp->getType());
            powerUp->setActive(false);
            return true;
        }
    }
    return false;
}

void PowerUpManager::activatePowerUp(PowerUp::Type type) {
    int currentTime = GetTickCount();
    const auto& effect = PowerUp(type, Position()).getEffect();
    int endTime = currentTime + effect.duration;

    // 添加新效果
    activeEffects.push_back({type, endTime});

    // 应用效果
    switch (type) {
        case PowerUp::Type::SPEED_BOOST:
            playerSpeed = effect.magnitude;
            break;
        case PowerUp::Type::GHOST_FREEZER:
            ghostSpeed = 0.0f;
            break;
        case PowerUp::Type::POINT_MULTIPLIER:
            scoreMultiplier = effect.magnitude;
            break;
        case PowerUp::Type::WALL_PASS:
            wallPassEnabled = true;
            break;
        case PowerUp::Type::TIME_SLOW:
            ghostSpeed = effect.magnitude;
            break;
    }
}

void PowerUpManager::updateEffects(int currentTime) {
    std::vector<PowerUp::Type> expiredEffects;

    // 检查效果是否过期
    for (auto it = activeEffects.begin(); it != activeEffects.end();) {
        if (currentTime > it->second) {
            expiredEffects.push_back(it->first);
            it = activeEffects.erase(it);
        } else {
            ++it;
        }
    }

    // 重置过期效果
    for (const auto& type : expiredEffects) {
        switch (type) {
            case PowerUp::Type::SPEED_BOOST:
                playerSpeed = 1.0f;
                break;
            case PowerUp::Type::GHOST_FREEZER:
                ghostSpeed = 1.0f;
                break;
            case PowerUp::Type::POINT_MULTIPLIER:
                scoreMultiplier = 1.0f;
                break;
            case PowerUp::Type::WALL_PASS:
                wallPassEnabled = false;
                break;
            case PowerUp::Type::TIME_SLOW:
                ghostSpeed = 1.0f;
                break;
        }
    }
}

void PowerUpManager::reset() {
    powerUps.clear();
    activeEffects.clear();
    scoreMultiplier = 1.0f;
    playerSpeed = 1.0f;
    ghostSpeed = 1.0f;
    wallPassEnabled = false;
    lastSpawnTime = 0;
}