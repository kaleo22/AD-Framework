#pragma once


// ------------------------------------------------------------
// includes
// ------------------------------------------------------------


#include <stdint.h>


// ------------------------------------------------------------
// constants
// ------------------------------------------------------------


#define VIEW_SIZE                   5


#define CELLTYPE_BITMASK            0x00FF
#define CELLTYPE_UNKNOWN            0x0000
#define CELLTYPE_FLOOR              0x0001
#define CELLTYPE_WALL               0x0002
#define CELLTYPE_TARGET             0x0003


#define CELLCONTENT_BITMASK         0xFF00
#define CELLCONTENT_NONE            0x0000
#define CELLCONTENT_SELF            0x0100
#define CELLCONTENT_ENEMY           0x0200
#define CELLCONTENT_OWN_FLAG        0x0400
#define CELLCONTENT_ENEMY_FLAG      0x0800
#define CELLCONTENT_OWN_TARGET      0x1000
#define CELLCONTENT_ENEMY_TARGET    0x2000


#define ACTION_WAIT                 0x00
#define ACTION_MOVE_NORTH           0x11
#define ACTION_MOVE_NORTHEAST       0x12
#define ACTION_MOVE_EAST            0x13
#define ACTION_MOVE_SOUTHEAST       0x14
#define ACTION_MOVE_SOUTH           0x15
#define ACTION_MOVE_SOUTHWEST       0x16
#define ACTION_MOVE_WEST            0x17
#define ACTION_MOVE_NORTHWEST       0x18
#define ACTION_PICK_UP_FLAG         0x21
#define ACTION_PUT_DOWN_FLAG        0x22


// ------------------------------------------------------------
// data types
// ------------------------------------------------------------


struct WorldSnapshot {
    uint32_t cells[VIEW_SIZE][VIEW_SIZE];
};


struct AgentInstance;


// ------------------------------------------------------------
// DLL function interface
// ------------------------------------------------------------


#ifdef _WIN32
    #define AGENT_API __declspec(dllexport)
    #define AGENT_CALL __cdecl
#else
    #define AGENT_API
    #define AGENT_CALL
#endif


#if defined(__cplusplus)
extern "C" {
#endif
AGENT_API const char* AGENT_CALL Agent_GetName();
AGENT_API AgentInstance* AGENT_CALL Agent_Create(uint32_t mapWidth, uint32_t mapHeight);
AGENT_API void AGENT_CALL Agent_Destroy(AgentInstance* instance);
AGENT_API void AGENT_CALL Agent_Reset(AgentInstance* instance);
AGENT_API uint32_t AGENT_CALL Agent_Step(AgentInstance* instance, const WorldSnapshot* snapshot, uint32_t lastAction);

#if defined(__cplusplus)
}
#endif
