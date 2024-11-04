//
// Created by XOS on 24-11-4.
//
#include "../include/Game.h"
#include <ctime>

Game::Game() : pacman(1, 1), score(0), gameOver(false) {
    srand(static_cast<unsigned int>(time(nullptr)));

    initializeMap();
    ghosts.push_back(Ghost(8, 1));
    ghosts.push_back(Ghost(8, 5));

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

void Game::initializeMap() {
    map = {
        {GameConfig::WALL, GameConfig::WALL, GameConfig::WALL, GameConfig::WALL, GameConfig::WALL, GameConfig::WALL, GameConfig::WALL, GameConfig::WALL, GameConfig::WALL, GameConfig::WALL},
        {GameConfig::WALL, GameConfig::DOT, GameConfig::DOT, GameConfig::DOT, GameConfig::DOT, GameConfig::DOT, GameConfig::DOT, GameConfig::DOT, GameConfig::DOT, GameConfig::WALL},
        {GameConfig::WALL, GameConfig::DOT, GameConfig::WALL, GameConfig::WALL, GameConfig::DOT, GameConfig::DOT, GameConfig::WALL, GameConfig::WALL, GameConfig::DOT, GameConfig::WALL},
        {GameConfig::WALL, GameConfig::DOT, GameConfig::DOT, GameConfig::DOT, GameConfig::DOT, GameConfig::DOT, GameConfig::DOT, GameConfig::DOT, GameConfig::DOT, GameConfig::WALL},
        {GameConfig::WALL, GameConfig::DOT, GameConfig::WALL, GameConfig::WALL, GameConfig::DOT, GameConfig::DOT, GameConfig::WALL, GameConfig::WALL, GameConfig::DOT, GameConfig::WALL},
        {GameConfig::WALL, GameConfig::DOT, GameConfig::DOT, GameConfig::DOT, GameConfig::DOT, GameConfig::DOT, GameConfig::DOT, GameConfig::DOT, GameConfig::DOT, GameConfig::WALL},
        {GameConfig::WALL, GameConfig::WALL, GameConfig::WALL, GameConfig::WALL, GameConfig::WALL, GameConfig::WALL, GameConfig::WALL, GameConfig::WALL, GameConfig::WALL, GameConfig::WALL}
    };
}

void Game::clearScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = {0, 0};
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, ' ', dwConSize, coordScreen, &cCharsWritten);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
    SetConsoleCursorPosition(hConsole, coordScreen);
}

void Game::run() {
    while (!gameOver) {
        clearScreen();
        updateGame();
        displayGame();
        Sleep(GameConfig::GAME_SPEED);
    }
    std::cout << "\nGame Over! Final Score: " << score << std::endl;
}

void Game::updateGame() {
    Position oldPos = pacman.getPosition();
    pacman.move(map);
    Position newPos = pacman.getPosition();

    if (map[newPos.y][newPos.x] == GameConfig::DOT) {
        score += 10;
        map[newPos.y][newPos.x] = GameConfig::EMPTY;
    }

    for (auto& ghost : ghosts) {
        ghost.move(map);
        if (ghost.getPosition().x == pacman.getPosition().x &&
            ghost.getPosition().y == pacman.getPosition().y) {
            gameOver = true;
            return;
        }
    }
}

void Game::displayGame() {
    std::vector<std::vector<char>> displayMap = map;

    Position pPos = pacman.getPosition();
    displayMap[pPos.y][pPos.x] = pacman.getSymbol();

    for (const auto& ghost : ghosts) {
        Position gPos = ghost.getPosition();
        displayMap[gPos.y][gPos.x] = ghost.getSymbol();
    }

    std::cout << "\n  PACMAN GAME\n\n";
    for (const auto& row : displayMap) {
        std::cout << "  ";
        for (char cell : row) {
            std::cout << cell << ' ';
        }
        std::cout << '\n';
    }
    std::cout << "\n  Score: " << score << "\n";
    std::cout << "  Use WASD keys to move\n";
}