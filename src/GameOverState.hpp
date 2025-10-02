#pragma once
#include "GameState.hpp"
#include <deque> 


extern "C" {
    #include <raylib.h>
}


class GameOverState : public GameState
{

    public:
        GameOverState(int score, bool night);
        ~GameOverState() override;

        void init() override;
        void handleInput() override;
        void update(float dt) override;
        void render() override;

        void pause() override {}
        void resume() override {}

    private:
        int finalScore = 0;
        Texture2D gameOverText;
        Texture2D background;
        bool isNight = false;
};