#include "MainGameState.hpp"
#include <iostream>
#include "StateMachine.hpp" 
#include "GameOverState.hpp"

extern "C" {
    #include <raylib.h>
}

MainGameState::MainGameState()
{
}

MainGameState::~MainGameState() {
    UnloadTexture(birdSprite);
    UnloadTexture(pipeSprite);
    UnloadTexture(backgroundDay);
    UnloadTexture(backgroundNight);
    UnloadTexture(baseSprite);
}


void MainGameState::init()
{
    //texturas
    birdSprite = LoadTexture("assets/sprites/bluebird-downflap.png");
    pipeSprite = LoadTexture("assets/sprites/pipe-green.png");
    backgroundDay = LoadTexture("assets/sprites/background-day.png");
    backgroundNight = LoadTexture("assets/sprites/background-night.png");
    baseSprite = LoadTexture("assets/sprites/base.png");


    //dimension bird
    player.width = (float)birdSprite.width;
    player.height = (float)birdSprite.height;

    //tuberias
    PIPE_W = (float)pipeSprite.width;
    PIPE_H = (float)pipeSprite.height;

    //gap tuberias
    GAP = player.height * 4.5f;
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
    bird_bb = {player.x, player.y, player.width, player.height};   

    //base
    float baseY = GetScreenHeight() - baseSprite.height;
    base = {0.0f, baseY, (float)GetScreenWidth(), (float)baseSprite.height};

    
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

    //puntuacion
    for (auto& p : pipes) {
        if (!p.scored && (p.top.x + PIPE_W) < (player.x - RADIUS)) { // comparar borde dcho del tubo con el pájaro
            score++;
            p.scored = true;
        }
    }

    // borrar tuberías fuera de pantalla
    if (!pipes.empty() && pipes.front().top.x + PIPE_W < 0) {
        pipes.pop_front();
    }

    //colisiones
    colision = false;
    for (const auto& p : pipes) {
        if (CheckCollisionRecs(bird_bb, p.top) || CheckCollisionRecs(bird_bb, p.bot)) {
            colision = true;
            this->state_machine->add_state(std::make_unique<GameOverState>(score), true);
            return;
        }
    }

    if(CheckCollisionRecs(bird_bb, base)){
        this->state_machine->add_state(std::make_unique<GameOverState>(score), true);
        return;
    }

    //cambio fondo con degradado
    backgroundTimer += deltaTime;

    if (!fading && backgroundTimer >= backgroundSwapTime) {
        fading = true;
        fadeProgress = 0.0f;
        backgroundTimer = 0.0f;
    }
        
    if (fading) {
        fadeProgress += deltaTime / fadeTime;

        if (fadeProgress >= 1.0f) {
            fadeProgress = 1.0f;
            fading = false;
            isNight = !isNight; 
        }
    }

    //aumentar complejidad
    PIPE_SPEED += 2.0f * deltaTime;  // +2 px/s cada segundo
    
    if (spawnEvery > 0.8f) {
        spawnEvery -= 0.01f * deltaTime; // reducir tiempo entre spawns
    }

    if (GAP > 80.0f) {
        GAP -= deltaTime; 
    }



}

void MainGameState::render()
{
    BeginDrawing();
    
    //FONDO cambio con degradado
    Texture2D from = isNight ? backgroundNight : backgroundDay;
    Texture2D to   = isNight ? backgroundDay   : backgroundNight;

    if (fading) {
        // fase de mezcla
        DrawTexture(from, 0, 0, WHITE);

        // alpha crece de 0 → 255
        unsigned char alpha = (unsigned char)(fadeProgress * 255.0f);
        Color fadeColor = {255, 255, 255, alpha};
        DrawTexture(to, 0, 0, fadeColor);
    } else {
        DrawTexture(from, 0, 0, WHITE);
    }

    //texto
    DrawText("Flapy Bird", 10, 10, 20, LIGHTGRAY);
    
    //pajaro
    DrawTexture(birdSprite, player.x, player.y, WHITE);

    //BB
    DrawRectangleLinesEx(bird_bb, 2.0f, BLUE);

    //tuberias
    for (const auto& p : pipes) {
        DrawTextureEx(this->pipeSprite, {p.top.x + PIPE_W, p.top.y + PIPE_H}, 180.f, 1.0f, WHITE);
        DrawTextureEx(this->pipeSprite, {p.bot.x , p.bot.y}, 0.f, 1.0f, WHITE);
    }

    //base
    float altura = GetScreenHeight() - baseSprite.height;
    DrawTexture(baseSprite, 0, altura, WHITE);

    std::string s = std::to_string(score);
    int textW = MeasureText(s.c_str(), 30);
    int x = (GetScreenWidth() - textW) / 2;  // centrado horizontal
    int y = 20;                               // margen superior
    DrawText(s.c_str(), x, y, 30, BLACK);

    EndDrawing();
}