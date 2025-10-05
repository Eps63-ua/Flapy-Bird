#include "MainGameState.hpp"
#include <iostream>
#include "StateMachine.hpp" 
#include "GameOverState.hpp"

extern "C" {
    #include <raylib.h>
}

MainGameState::MainGameState(bool night)
{
    isNight = night;
}

MainGameState::~MainGameState() {
    //texturas
    UnloadTexture(birdSprite);
    UnloadTexture(pipeSprite);
    UnloadTexture(backgroundDay);
    UnloadTexture(backgroundNight);
    UnloadTexture(baseSprite);

    //numeros
    for (int i = 0; i < 10; i++) {
        UnloadTexture(numberSprites[i]);
    }

    //sonidos
    UnloadSound(flapSound);
    UnloadSound(scoreSound);
    UnloadSound(hitSound);
}


void MainGameState::init()
{
    //CARGAMOS ASSETS
    //texturas
    birdSprite = LoadTexture("assets/sprites/bluebird-downflap.png");
    pipeSprite = LoadTexture("assets/sprites/pipe-green.png");
    backgroundDay = LoadTexture("assets/sprites/background-day.png");
    backgroundNight = LoadTexture("assets/sprites/background-night.png");
    baseSprite = LoadTexture("assets/sprites/base.png");
    // numeros
    for (int i = 0; i < 10; i++) {
        numberSprites[i] = LoadTexture(("assets/sprites/" + std::to_string(i) + ".png").c_str());
    }
    
    //sonidos
    flapSound = LoadSound("assets/audio/wing.wav");
    scoreSound = LoadSound("assets/audio/point.wav");
    hitSound = LoadSound("assets/audio/hit.wav");

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
        PlaySound(flapSound);
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
            PlaySound(scoreSound);
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
            PlaySound(hitSound);
            WaitTime(0.2);
            this->state_machine->add_state(std::make_unique<GameOverState>(score, isNight), true);
            return;
        }
    }

    if(CheckCollisionRecs(bird_bb, base)){
        PlaySound(hitSound);
        WaitTime(0.2);
        this->state_machine->add_state(std::make_unique<GameOverState>(score, isNight), true);
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
    //DrawRectangleLinesEx(bird_bb, 2.0f, BLUE);

    //tuberias
    for (const auto& p : pipes) {
        DrawTextureEx(this->pipeSprite, {p.top.x + PIPE_W, p.top.y + PIPE_H}, 180.f, 1.0f, WHITE);
        DrawTextureEx(this->pipeSprite, {p.bot.x , p.bot.y}, 0.f, 1.0f, WHITE);
    }

    //base
    float altura = GetScreenHeight() - baseSprite.height;
    DrawTexture(baseSprite, 0, altura, WHITE);

    //puntuacion
    std::string scoreStr = std::to_string(score);
    int totalWidth = scoreStr.length() * numberSprites[0].width;
    int startX = (GetScreenWidth() - totalWidth) / 2;

    for (size_t i = 0; i < scoreStr.length(); i++) {
        int digit = scoreStr[i] - '0';
        DrawTexture(numberSprites[digit], startX + i * numberSprites[digit].width, 20, WHITE);
    }

    EndDrawing();
}