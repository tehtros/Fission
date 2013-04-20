#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <stack>

#include <Core/Manager.h>
#include <Core/State.h>

class StateManager : public Manager
{
    public:
        StateManager(Game *game);
        virtual ~StateManager();

        virtual bool update(float dt);

        void pushState(State *state);
        void popState();

        State *getCurrentState(){if (mStates.size() > 0) return mStates.top(); else return NULL;}

    protected:
        std::stack <State*> mStates; //stack of states
    private:
};

#endif // STATEMANAGER_H
