#include "Core/StateManager.h"

#include "Core/State.h"

StateManager *StateManager::Instance = NULL;

StateManager::StateManager()
{
    Instance = this;
}

StateManager::~StateManager()
{
    //dtor
}

bool StateManager::update(float dt)
{
    if (mStates.size()>0 && !mStates.top()->update(dt))
    {
        delete mStates.top();
        mStates.pop();
    }
    return mStates.size()>0;
}

void StateManager::pushState(State *state)
{
    mStates.push(state);
    state->initialize();
}

void StateManager::popState()
{
    mStates.top()->kill();
}
