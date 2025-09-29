 #include "GameOverState.hpp"
#include <iostream>

extern "C" {
    #include <raylib.h>
}

GameOverState::GameOverState(int score) : GameState(), finalScore(score) {}

void GameOverState::init() {
}

void GameOverState::handleInput() {
}

void GameOverState::update(float) {
}

void GameOverState::render()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Game Over", 10, 10, 20, LIGHTGRAY);

    // Mostrar la puntuación final
    std::string s = "Score: " + std::to_string(finalScore);
    int textW = MeasureText(s.c_str(), 30);
    int x = (GetScreenWidth() - textW) / 2;
    int y = GetScreenHeight() / 2;
    DrawText(s.c_str(), x, y, 30, BLACK);

    EndDrawing();
}