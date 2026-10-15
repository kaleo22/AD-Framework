// Compilation and linking:
// g++ -c simple_agent.cpp
// g++ -o simple_agent.dll simple_agent.o -shared -Wl,--subsystem,windows


// ------------------------------------------------------------
// includes
// ------------------------------------------------------------


#include <stdlib.h>
#include <stdio.h>
#include "agent_api.h"


// ------------------------------------------------------------
// constants
// ------------------------------------------------------------


#define AGENT_NAME "SimpleAgent"


// ------------------------------------------------------------
// data types
// ------------------------------------------------------------


struct AgentInstance
{
    int rounds = 0;
    unsigned int mapWidth = 0;
    unsigned int mapHeight = 0;
};


// ------------------------------------------------------------
// functions
// ------------------------------------------------------------


AgentInstance* Create(uint32_t mapWidth, uint32_t mapHeight)
{
    AgentInstance* instance = new AgentInstance();
    instance->mapWidth = mapWidth;
    instance->mapHeight = mapHeight;

    printf("AGENT: initialized data structures, map dimensions %u x %u\n", mapWidth, mapHeight);

    return instance;
}


void Destroy(AgentInstance* instance)
{
    printf("AGENT: cleaning up memory\n");

    delete instance;
}


void Reset(AgentInstance* instance)
{
    printf("AGENT: performing reset of internal states\n");

    instance->rounds = 0;
}


uint32_t Step(AgentInstance* instance, const WorldSnapshot* snapshot, uint32_t lastAction)
{
    ++instance->rounds;
    uint32_t action = ACTION_MOVE_NORTH + (rand() % 8);

    printf("AGENT: I have been called %u times, last action %u, returning new action: %u\n", instance->rounds, lastAction, action);

    return action;
}


// ------------------------------------------------------------
// DLL function interface
// ------------------------------------------------------------


extern "C" {

AGENT_API const char* AGENT_CALL Agent_GetName()
{
    return AGENT_NAME;
}


AGENT_API AgentInstance* AGENT_CALL Agent_Create(uint32_t mapWidth, uint32_t mapHeight)
{
    return Create(mapWidth, mapHeight);
}


AGENT_API void AGENT_CALL Agent_Destroy(AgentInstance* instance)
{
    if (instance) {
        Destroy(instance);
    }
}


AGENT_API void AGENT_CALL Agent_Reset(AgentInstance* instance)
{
    if (instance) {
        Reset(instance);
    }
}


AGENT_API uint32_t AGENT_CALL Agent_Step(AgentInstance* instance, const WorldSnapshot* snapshot, uint32_t lastAction)
{
    if ((instance) && (snapshot)) {
        return Step(instance, snapshot, lastAction);
    } else {
        return ACTION_WAIT;
    }
}

} // extern "C"
