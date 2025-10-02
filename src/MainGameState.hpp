#pragma once
#include "GameState.hpp"
#include <deque> 

extern "C" {
    #include <raylib.h>
}

struct Bird {
    float x, y, vy, height, width;
};

struct PipePair { 
    Rectangle top, bot; 
    bool scored=false; 
};

class MainGameState : public GameState
{
    public:
        MainGameState(bool night = bool{});
        ~MainGameState() override;

        void init() override;
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;

        void pause(){};
        void resume(){};

    
    private:
        char entered_key;
        
        //pajaro
        Bird player{144.f, 144.f, 0.f};
        const float GRAVITY = 900.f;
        const float JUMP_VY = -300.f;
        float RADIUS  = 17.f;

        //bounding box
        Rectangle bird_bb; 
        
        //base
        Rectangle base;
        bool colision = false; //colision con tuberias o base

        //tuberias
        std::deque<PipePair> pipes;
        float PIPE_W = 32.f;
        float PIPE_H = 320.f;
        float PIPE_SPEED = 100.f; // píxeles por segundo
        float GAP = 120.f;        // separación entre tuberías
       
        float spawnTimer = 0.f;
        float spawnEvery = 2.0f;  // cada 2 segundos aparece un par de tuberías

        //puntuacion
        int score = 0;

        //FONDO
        //temporizador cambio
        float backgroundTimer = 0.0f;
        bool isNight = false;
        const float backgroundSwapTime = 10.0f; 
        //degradado cambio
        bool fading = false;             
        float fadeTime = 5.0f;           
        float fadeProgress = 0.0f;       


        //texturas
        Texture2D birdSprite;
        Texture2D pipeSprite;
        Texture2D backgroundDay;
        Texture2D backgroundNight;
        Texture2D startingBackground{};
        Texture2D baseSprite;

};