#pragma once
#include "GameState.hpp"
#include <deque> 

extern "C" {
    #include <raylib.h>
}

struct Bird {
    float x, y, vy;
};

struct PipePair { 
    Rectangle top, bot; 
    bool scored=false; 
};

class MainGameState : public GameState
{
    public:
        MainGameState();
        ~MainGameState() = default;

        void init() override;
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;

        void pause(){};
        void resume(){};

    
    private:
        char entered_key;
        
        //pajaro
        Bird player{200.f, 200.f, 0.f};
        const float GRAVITY = 900.f;
        const float JUMP_VY = -300.f;
        const float RADIUS  = 17.f;
        Rectangle bird_bb; // bounding box del pájaro

        //tuberias
        std::deque<PipePair> pipes;
        const float PIPE_W = 32.f;
        const float PIPE_H = 320.f;
        const float PIPE_SPEED = 100.f; // píxeles por segundo
        const float GAP = 120.f;        // separación entre tuberías
       
        float spawnTimer = 0.f;
        const float spawnEvery = 2.0f;  // cada 2 segundos aparece un par de tuberías


};