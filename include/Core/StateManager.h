#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <stack>

#include <Core/Manager.h>
#include <Core/State.h>

class StateManager : public Manager
{
    public:
        StateManager();
        virtual ~StateManager();

        virtual bool update(float dt);

        void pushState(State *state);
        void popState();

        State *getCurrentState(){if (mStates.size() > 0) return mStates.top(); else return NULL;}

        static StateManager *get(){return Instance;}

    protected:
        std::stack <State*> mStates; //stack of states
    private:
        static StateManager *Instance;
};

#endif // STATEMANAGER_H
