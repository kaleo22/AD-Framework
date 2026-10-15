#pragma once


// ------------------------------------------------------------
// includes
// ------------------------------------------------------------

#include "cstdio"
#include <string>
#include "agent/agent_api.h"


// ------------------------------------------------------------
// classes (declaration)
// ------------------------------------------------------------


class AgentLibrary {
public:
    AgentLibrary() = default;
    ~AgentLibrary();

    AgentLibrary(const AgentLibrary&) = delete;
    AgentLibrary& operator=(const AgentLibrary&) = delete;
    AgentLibrary(AgentLibrary&& other) noexcept;
    AgentLibrary& operator=(AgentLibrary&& other) noexcept;

    bool Load(const char* filename);
    void Unload();
    bool IsLoaded() const;

    const char* GetName() const;
    AgentInstance* Create(uint32_t mapWidth, uint32_t mapHeight) const;
    void Destroy(AgentInstance* instance) const;
    void Reset(AgentInstance* instance) const;
    uint32_t Step(AgentInstance* instance, const WorldSnapshot* snapshot, uint32_t lastAction) const;

private:
    struct WindowsData;
    WindowsData* data_ = nullptr;

    using GetNameFunction = const char* (AGENT_CALL *)();
    using CreateFunction = AgentInstance* (AGENT_CALL *)(uint32_t, uint32_t);
    using DestroyFunction = void (AGENT_CALL *)(AgentInstance*);
    using ResetFunction = void (AGENT_CALL *)(AgentInstance*);
    using StepFunction = uint32_t(AGENT_CALL *)(AgentInstance*, const WorldSnapshot*, uint32_t);

    GetNameFunction getName_ = nullptr;
    CreateFunction create_ = nullptr;
    DestroyFunction destroy_ = nullptr;
    ResetFunction reset_ = nullptr;
    StepFunction step_ = nullptr;

    std::string name_;
};


struct PlayerInstance
{
    bool Load(const char* player_filename, uint32_t mapWidth, uint32_t mapHeight);
    void Unload();

    const char* GetName();
    void Reset();
    uint32_t Step(const WorldSnapshot* snapshot);

    AgentLibrary library;
    AgentInstance* instance = nullptr;
    uint32_t lastAction = ACTION_WAIT;
    std::string filename;
    bool ready = false;

    PlayerInstance() = default;
    ~PlayerInstance()
    {
        Unload();
    }
    PlayerInstance(const PlayerInstance&) = delete;
    PlayerInstance& operator=(const PlayerInstance&) = delete;
};
