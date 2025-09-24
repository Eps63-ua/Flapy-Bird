#include "MainGameState.hpp"
#include <iostream>

extern "C" {
    #include <raylib.h>
}

MainGameState::MainGameState()
{
}

void MainGameState::init()
{

}

void MainGameState::handleInput()
{
    if (IsKeyPressed(KEY_SPACE))
    {
        player.vy = JUMP_VY;
    }
}

void MainGameState::update(float deltaTime)
{
    //pajaro
    player.vy += GRAVITY * deltaTime;
    player.y += player.vy * deltaTime;
    
    //bounding box
    bird_bb = {player.x - RADIUS, player.y - RADIUS, RADIUS*2, RADIUS*2};

    //tuberias
    spawnTimer += deltaTime;
    if (spawnTimer >= spawnEvery) {
        spawnTimer = 0.f;

        int offset = GetRandomValue((int)(PIPE_H/2), GetScreenHeight()/2);
        float pipe_y_offset_top = (float)offset;

        PipePair p;
        float x_start = (float)GetScreenWidth();

        p.top = { x_start, -pipe_y_offset_top, PIPE_W, PIPE_H };
        p.bot = { x_start, (PIPE_H - pipe_y_offset_top) + GAP, PIPE_W, PIPE_H };

        pipes.push_back(p);
    }

    // mover tuberías
    for (auto &p : pipes) {
        p.top.x -= PIPE_SPEED * deltaTime;
        p.bot.x -= PIPE_SPEED * deltaTime;
    }

    // borrar tuberías fuera de pantalla
    if (!pipes.empty() && pipes.front().top.x + PIPE_W < 0) {
        pipes.pop_front();
    }
}

void MainGameState::render()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Flapy Bird", 10, 10, 20, LIGHTGRAY);
    //pajaro
    DrawCircleV(Vector2{player.x, player.y}, RADIUS, RED);
    //BB
    DrawRectangleLinesEx(bird_bb, 2.0f, BLUE);

    //tuberias
    for (const auto& p : pipes) {
        DrawRectangleRec(p.top, GREEN);
        DrawRectangleRec(p.bot, GREEN);
    }

    EndDrawing();
}