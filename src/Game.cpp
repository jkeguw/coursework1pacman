//
// Created by XOS on 24-11-4.
//
#include "../include/Game.h"

const int Game::MAP_WIDTH;
const int Game::MAP_HEIGHT;
const int Game::INFO_WIDTH;
const int Game::TOTAL_WIDTH;

Game::Game()
    : pacman(GameConfig::PACMAN_START_X, GameConfig::PACMAN_START_Y),
      score(0),
      gameOver(false),
      powerMode(false),
      level(1) {

    initializeMap();
    ghosts.push_back(Ghost(GameConfig::GHOST_HOME_X - 2, GameConfig::GHOST_HOME_Y));
    ghosts.push_back(Ghost(GameConfig::GHOST_HOME_X + 2, GameConfig::GHOST_HOME_Y));

    console = std::make_unique<ConsoleBuffer>(TOTAL_WIDTH, MAP_HEIGHT);
    remainingDots = countRemainingDots();
}

int Game::countRemainingDots() const {
    int count = 0;
    for (const auto& row : map) {
        for (char cell : row) {
            if (cell == GameConfig::DOT || cell == GameConfig::POWER_DOT) {
                count++;
            }
        }
    }
    return count;
}


std::string Game::centerText(const std::string& text, int width) const {
    int padding = (width - static_cast<int>(text.length())) / 2;
    return std::string(std::max(0, padding), ' ') + text;
}

void Game::drawInfoPanel() {
    // 绘制分隔线
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        console->draw(MAP_WIDTH, y, '|');
    }

    // 游戏标题
    console->drawString(MAP_WIDTH + 2, 1, centerText("PACMAN", INFO_WIDTH - 3));

    // 分数显示
    console->drawString(MAP_WIDTH + 2, 3, "SCORE:");
    console->drawString(MAP_WIDTH + 2, 4, centerText(std::to_string(score), INFO_WIDTH - 3));

    // 关卡显示
    console->drawString(MAP_WIDTH + 2, 6, "LEVEL:");
    console->drawString(MAP_WIDTH + 2, 7, centerText(std::to_string(level), INFO_WIDTH - 3));

    // 剩余豆子
    console->drawString(MAP_WIDTH + 2, 9, "DOTS LEFT:");
    console->drawString(MAP_WIDTH + 2, 10, centerText(std::to_string(remainingDots), INFO_WIDTH - 3));

    // 状态显示
    if (powerMode) {
        console->drawString(MAP_WIDTH + 2, 12, "POWER MODE!");
    }

    // 控制说明
    console->drawString(MAP_WIDTH + 2, 14, "CONTROLS:");
    console->drawString(MAP_WIDTH + 2, 15, " W - Up");
    console->drawString(MAP_WIDTH + 2, 16, " S - Down");
    console->drawString(MAP_WIDTH + 2, 17, " A - Left");
    console->drawString(MAP_WIDTH + 2, 18, " D - Right");

    // 如果游戏结束，显示游戏结束信息
    if (gameOver) {
        console->drawString(MAP_WIDTH + 2, MAP_HEIGHT - 3, "GAME OVER!");
        console->drawString(MAP_WIDTH + 2, MAP_HEIGHT - 2, "Press R to");
        console->drawString(MAP_WIDTH + 2, MAP_HEIGHT - 1, "restart");
    }
}

void Game::run() {
    while (true) {  // 主游戏循环
        if (!gameOver) {
            updateGame();
            displayGame();
            Sleep(GameConfig::GAME_SPEED);
        } else {
            handleGameOver();
            if (gameOver) {  // 如果用户选择退出而不是重启
                break;
            }
        }
    }

    // 显示最终告别屏幕
    console->clear();
    std::string thankYou = "Thanks for playing!";
    console->drawString((TOTAL_WIDTH - thankYou.length()) / 2, MAP_HEIGHT / 2, thankYou);
    console->swap();

    // 等待最后一次按键才退出
    while (!_kbhit()) Sleep(100);
}

void Game::updateGame() {
    if (gameOver) return;

    // 更新吃豆人位置
    Position oldPos = pacman.getPosition();
    pacman.move(map);
    Position newPos = pacman.getPosition();

    // 检查豆子收集
    if (map[newPos.y][newPos.x] == GameConfig::DOT) {
        score += GameConfig::NORMAL_DOT_SCORE;
        map[newPos.y][newPos.x] = GameConfig::EMPTY;
        remainingDots--;
    }
    else if (map[newPos.y][newPos.x] == GameConfig::POWER_DOT) {
        score += GameConfig::POWER_DOT_SCORE;
        map[newPos.y][newPos.x] = GameConfig::EMPTY;
        powerMode = true;
        remainingDots--;
    }

    // 更新幽灵位置
    for (auto& ghost : ghosts) {
        ghost.move(map);
        if (ghost.getPosition().x == pacman.getPosition().x &&
            ghost.getPosition().y == pacman.getPosition().y) {
            if (powerMode) {
                score += GameConfig::GHOST_SCORE;
                // 重置幽灵位置
                ghost = Ghost(GameConfig::GHOST_HOME_X, GameConfig::GHOST_HOME_Y);
            } else {
                gameOver = true;
                return;
            }
            }
    }

    // 检查是否完成关卡
    if (remainingDots == 0) {
        level++;
        initializeMap();
        remainingDots = countRemainingDots();
    }
}

void Game::displayGame() {
    // 清除缓冲区
    console->clear();

    // 创建用于显示的地图副本
    std::vector<std::vector<char>> displayMap = map;

    // 放置吃豆人
    Position pPos = pacman.getPosition();
    displayMap[pPos.y][pPos.x] = pacman.getSymbol();

    // 放置幽灵
    for (const auto& ghost : ghosts) {
        Position gPos = ghost.getPosition();
        displayMap[gPos.y][gPos.x] = ghost.getSymbol();
    }

    // 绘制地图
    console->drawMatrix(displayMap);

    // 绘制信息面板
    drawInfoPanel();

    // 交换缓冲区
    console->swap();
}

void Game::initializeMap() {
    // 28x31的经典吃豆人地图布局
    map = {
        {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
        {'#','.','.','.','.','.','.','.','.','.','.','.','.','.','#','#','.','.','.','.','.','.','.','.','.','.','.','.'},
        {'#','.','#','#','#','#','.','#','#','#','#','#','.','.','.','.','.','#','#','#','#','#','#','.','#','#','#','.'},
        {'#','o','#','#','#','#','.','#','#','#','#','#','.','#','#','#','#','.','#','#','#','#','#','.','#','#','#','o'},
        {'#','.','#','#','#','#','.','#','#','#','#','#','.','#','#','#','#','.','#','#','#','#','#','.','#','#','#','.'},
        {'#','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.'},
        {'#','.','#','#','#','#','.','#','#','.','#','#','#','#','#','#','#','#','#','#','.','#','#','.','#','#','#','.'},
        {'#','.','#','#','#','#','.','#','#','.','#','#','#','#','#','#','#','#','#','#','.','#','#','.','#','#','#','.'},
        {'#','.','.','.','.','.','.','.','.','.','.','.','.','.','#','#','.','.','.','.','.','.','.','.','.','.','.','.'},
        {'#','#','#','#','#','#','.','#','#','#','#','#','.','.','.','.','.','#','#','#','#','#','.','#','#','#','#','#'},
        {'#','#','#','#','#','#','.','#','#','#','#','#','.','#','#','#','#','.','#','#','#','#','.','#','#','#','#','#'},
        {'#','#','#','#','#','#','.','#','#','.','.','.','.','.','.','.','.','.','.','.','#','#','.','#','#','#','#','#'},
        {'#','#','#','#','#','#','.','#','#','.','#','#','#',' ',' ','#','#','#','.','.','.','.','.','#','#','#','#','#'},
        {'#','#','#','#','#','#','.','#','#','.','#',' ',' ',' ',' ',' ',' ','#','.','#','#','.','#','#','#','#','#','#'},
        {' ',' ',' ',' ',' ',' ','.','.','.','.','.','#',' ',' ',' ',' ',' ','#','.','#','#','.','#','#','#','#','#','#'},
        {'#','#','#','#','#','#','.','#','#','.','#',' ',' ',' ',' ',' ',' ','#','.','#','#','.','#','#','#','#','#','#'},
        {'#','#','#','#','#','#','.','#','#','.','#','#','#','#','#','#','#','#','.','#','#','.','#','#','#','#','#','#'},
        {'#','#','#','#','#','#','.','#','#','.','.','.','.','.','.','.','.','.','.','.','.','.','.','#','#','#','#','#'},
        {'#','#','#','#','#','#','.','#','#','.','#','#','#','#','#','#','#','#','#','#','#','.','#','#','#','#','#','#'},
        {'#','.','.','.','.','.','.','.','.','.','.','.','.','.','#','#','.','.','.','.','.','.','.','.','.','.','.','.'},
        {'#','.','#','#','#','#','.','#','#','#','#','#','.','.','.','.','.','#','#','#','#','#','.','#','#','#','#','.'},
        {'#','.','#','#','#','#','.','#','#','#','#','#','.','#','#','#','#','.','#','#','#','#','.','#','#','#','#','.'},
        {'#','o','.','.','#','#','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.'},
        {'#','#','#','.','#','#','.','#','#','.','#','#','#','#','#','#','#','#','#','#','.','#','#','.','#','#','#','#'},
        {'#','#','#','.','#','#','.','#','#','.','#','#','#','#','#','#','#','#','#','#','.','#','#','.','#','#','#','#'},
        {'#','.','.','.','.','.','.','.','.','.','.','.','.','.','#','#','.','.','.','.','.','.','.','.','.','.','.','.'},
        {'#','.','#','#','#','#','#','#','#','#','#','#','.','.','.','.','.','#','#','#','#','#','#','#','#','#','#','.'},
        {'#','.','#','#','#','#','#','#','#','#','#','#','.','#','#','#','#','.','#','#','#','#','#','#','#','#','#','.'},
        {'#','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.'},
        {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
    };
}

void Game::handleGameOver() {
    displayGameOverScreen();

    // 清空键盘缓冲区
    while (_kbhit()) _getch();

    bool exitGame = false;
    while (!exitGame) {
        if (_kbhit()) {
            char key = _getch();
            switch (key) {
                case 'r':
                case 'R':
                    // 重置游戏状态
                    score = 0;
                level = 1;
                powerMode = false;
                gameOver = false;
                initializeMap();
                pacman = Pacman(GameConfig::PACMAN_START_X, GameConfig::PACMAN_START_Y);
                ghosts.clear();
                ghosts.push_back(Ghost(GameConfig::GHOST_HOME_X - 2, GameConfig::GHOST_HOME_Y));
                ghosts.push_back(Ghost(GameConfig::GHOST_HOME_X + 2, GameConfig::GHOST_HOME_Y));
                remainingDots = countRemainingDots();
                return;

                case 'q':
                case 'Q':
                case 27:  // ESC键
                    exitGame = true;
                break;
            }
        }
        Sleep(100);  // 减少CPU使用
    }
}

void Game::displayGameOverScreen() {
    console->clear();

    // 显示最终游戏状态
    std::vector<std::string> gameOverMessages = {
        "GAME OVER!",
        "",
        "Final Score: " + std::to_string(score),
        "Level Reached: " + std::to_string(level),
        "",
        "Press 'R' to Restart",
        "Press 'Q' or ESC to Quit"
    };

    // 计算开始位置以居中显示消息
    int startY = (MAP_HEIGHT - gameOverMessages.size()) / 2;

    // 显示所有消息
    for (size_t i = 0; i < gameOverMessages.size(); ++i) {
        std::string centeredText = centerText(gameOverMessages[i], TOTAL_WIDTH);
        console->drawString(0, startY + i, centeredText);
    }

    // 添加装饰边框
    for (int x = 0; x < TOTAL_WIDTH; ++x) {
        console->draw(x, startY - 2, '=');
        console->draw(x, startY + gameOverMessages.size() + 1, '=');
    }

    console->swap();
}