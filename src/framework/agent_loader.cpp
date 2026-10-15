// Compilation:
// g++ -c agent_loader.cpp


// ------------------------------------------------------------
// includes
// ------------------------------------------------------------


#include <utility>
#include "cstdio"
#include "framework/agent_loader.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


// ------------------------------------------------------------
// class AgentLibrary (implementation)
// ------------------------------------------------------------


struct AgentLibrary::WindowsData
{
    HMODULE module = nullptr;
};


AgentLibrary::~AgentLibrary()
{
    Unload();
}


AgentLibrary::AgentLibrary(AgentLibrary&& other) noexcept
{
    data_ = other.data_;

    getName_ = other.getName_;
    create_ = other.create_;
    destroy_ = other.destroy_;
    reset_ = other.reset_;
    step_ = other.step_;

    name_ = std::move(other.name_);

    other.data_ = nullptr;
    other.getName_ = nullptr;
    other.create_ = nullptr;
    other.destroy_ = nullptr;
    other.reset_ = nullptr;
    other.step_ = nullptr;
}


AgentLibrary& AgentLibrary::operator=(AgentLibrary&& other) noexcept
{
    if (this == &other) {
        return *this;
    }

    Unload();

    data_ = other.data_;
    getName_ = other.getName_;
    create_ = other.create_;
    destroy_ = other.destroy_;
    reset_ = other.reset_;
    step_ = other.step_;
    name_ = std::move(other.name_);

    other.data_ = nullptr;
    other.getName_ = nullptr;
    other.create_ = nullptr;
    other.destroy_ = nullptr;
    other.reset_ = nullptr;
    other.step_ = nullptr;

    return *this;
}


bool AgentLibrary::Load(const char* filename)
{
    Unload();

    data_ = new WindowsData;
    data_->module = LoadLibraryA(filename);
    if (!data_->module) {
        delete data_;
        data_ = nullptr;
        return false;
    }

    getName_ = reinterpret_cast<GetNameFunction>(GetProcAddress(data_->module, "Agent_GetName"));
    create_  = reinterpret_cast<CreateFunction>( GetProcAddress(data_->module, "Agent_Create"));
    destroy_ = reinterpret_cast<DestroyFunction>(GetProcAddress(data_->module, "Agent_Destroy"));
    reset_   = reinterpret_cast<ResetFunction>(  GetProcAddress(data_->module, "Agent_Reset"));
    step_    = reinterpret_cast<StepFunction>(   GetProcAddress(data_->module, "Agent_Step"));

    if (!getName_ || !create_ || !destroy_ || !step_) {
        Unload();
        return false;
    }

    const char* name = getName_();
    name_ =  (name != nullptr)? name : filename;

    return true;
}


void AgentLibrary::Unload()
{
    if (data_) {
        if (data_->module) {
            FreeLibrary(data_->module);
            data_->module = nullptr;
        }
        delete data_;
        data_ = nullptr;
    }

    getName_ = nullptr;
    create_ = nullptr;
    destroy_ = nullptr;
    reset_ = nullptr;
    step_ = nullptr;

    name_.clear();
}


bool AgentLibrary::IsLoaded() const
{
    return (data_ != nullptr) && (data_->module != nullptr);
}


const char* AgentLibrary::GetName() const
{
    return name_.c_str();
}


AgentInstance* AgentLibrary::Create(uint32_t mapWidth, uint32_t mapHeight) const
{
    if (!create_) {
        return nullptr;
    }

    return create_(mapWidth, mapHeight);
}


void AgentLibrary::Destroy(AgentInstance* instance) const
{
    if (destroy_) {
        destroy_(instance);
    }
}


void AgentLibrary::Reset(AgentInstance* instance) const
{
    if (reset_) {
        reset_(instance);
    }
}


uint32_t AgentLibrary::Step(AgentInstance* instance, const WorldSnapshot* snapshot, uint32_t lastAction) const
{
    if (!step_) {
        return 0;
    }

    return step_(instance, snapshot, lastAction);
}


// ------------------------------------------------------------
// struct PlayerInstance (implementation)
// ------------------------------------------------------------


bool PlayerInstance::Load(const char* player_filename, uint32_t mapWidth, uint32_t mapHeight)
{
    Unload();

    AgentLibrary newLibrary;
    if (!newLibrary.Load(player_filename)) {
        printf("Unable to load agent DLL: %s\n", player_filename);
        return false;
    }

    AgentInstance* newInstance = newLibrary.Create(mapWidth, mapHeight);
    if (!newInstance) {
        printf("Agent_Create failed: %s\n", player_filename);
        newLibrary.Unload();
        return false;
    }

    this->library = std::move(newLibrary);
    this->instance = newInstance;
    this->filename = player_filename;
    this->lastAction = 0;
    this->ready = true;

    return true;
}


void PlayerInstance::Unload()
{
    if (instance) {
        library.Destroy(instance);
        instance = nullptr;
    }

    library.Unload();
    filename.clear();
    lastAction = ACTION_WAIT;
    ready = false;
}


const char* PlayerInstance::GetName()
{
    return library.GetName();
}


void PlayerInstance::Reset()
{
    library.Reset(instance);
    lastAction = ACTION_WAIT;
}


uint32_t PlayerInstance::Step(const WorldSnapshot* snapshot)
{
    return library.Step(instance, snapshot, lastAction);
}
