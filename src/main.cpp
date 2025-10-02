#include "StateMachine.hpp"
#include "StartGameState.hpp"
#include <memory>
#include <chrono>

extern "C" {
    #include <raylib.h>
}


int main()
{ 
    float delta_time = 0.0f;
    
    InitWindow(288, 512, "Flapy Bird");

    StateMachine state_machine = StateMachine();
    state_machine.add_state(std::make_unique<StartGameState>(), false);
    state_machine.handle_state_changes(delta_time);
        
    while (!WindowShouldClose())
    {
        delta_time = GetFrameTime();      
        state_machine.handle_state_changes(delta_time);
        state_machine.getCurrentState()->handleInput();
        state_machine.getCurrentState()->update(delta_time);
        state_machine.getCurrentState()->render(); 
    }

    CloseWindow(); 
    return 0;
}