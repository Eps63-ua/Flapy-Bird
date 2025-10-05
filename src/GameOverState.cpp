 #include "GameOverState.hpp"
#include <iostream>
#include "StateMachine.hpp"
#include "MainGameState.hpp"

extern "C" {
    #include <raylib.h>
}

GameOverState::GameOverState(int score, bool night) 
{
    finalScore = score;
    isNight = night;
}

GameOverState::~GameOverState() {
    UnloadTexture(gameOverText);
    UnloadSound(dieSound);
}

void GameOverState::init() {
    gameOverText = LoadTexture("assets/sprites/gameover.png");
    dieSound = LoadSound("assets/audio/die.wav");
    
    if(isNight){

        background = LoadTexture("assets/sprites/background-night.png");
    }else{
        background = LoadTexture("assets/sprites/background-day.png");
    }
    PlaySound(dieSound);
}

void GameOverState::handleInput() {
    if (IsKeyPressed(KEY_SPACE)) {
        this->state_machine->add_state(std::make_unique<MainGameState>(isNight), true);
    }
}

void GameOverState::update(float) {
}

void GameOverState::render()
{
    BeginDrawing();
    DrawTexture(background, 0, 0, WHITE);

    // Mostrar la puntuación final
    std::string s = "Score: " + std::to_string(finalScore);
    int textW = MeasureText(s.c_str(), 30);
    int x = (GetScreenWidth() - textW) / 2;
    int y = GetScreenHeight() / 2;
    DrawText(s.c_str(), x, y, 30, BLACK);

    // Mostrar el texto de "Game Over"
    x = (GetScreenWidth() - gameOverText.width) / 2;
    y -= gameOverText.height * 2;
    DrawTexture(gameOverText, x, y, WHITE);


    EndDrawing();
}