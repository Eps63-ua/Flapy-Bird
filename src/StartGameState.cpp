#include "StartGameState.hpp"
#include "StateMachine.hpp"

extern "C" {
    #include <raylib.h>
}

StartGameState::StartGameState() = default;

StartGameState::~StartGameState() {
    UnloadTexture(background);
    UnloadTexture(getReadySprite);
}

void StartGameState::init() {
    background = LoadTexture("assets/sprites/background-day.png");
    getReadySprite = LoadTexture("assets/sprites/message.png"); 
}

void StartGameState::handleInput() {
    if (IsKeyPressed(KEY_SPACE)) {
        this->state_machine->add_state(std::make_unique<MainGameState>(), true);
    }
}

void StartGameState::update(float) {
}

void StartGameState::render() {
    BeginDrawing();

    DrawTexture(background, 0, 0, WHITE);

    // centrar el mensaje
    int x = (GetScreenWidth() - getReadySprite.width) / 2;
    int y = GetScreenHeight() / 2 - getReadySprite.height / 2;
    DrawTexture(getReadySprite, x, y, WHITE);

    EndDrawing();
}
