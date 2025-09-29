#pragma once
#include "GameState.hpp"


class GameOverState : public GameState
{

    public:
        GameOverState(int score);
        ~GameOverState() override = default;

        void init() override;
        void handleInput() override;
        void update(float dt) override;
        void render() override;

        void pause() override {}
        void resume() override {}

    private:
        int finalScore = 0;
};