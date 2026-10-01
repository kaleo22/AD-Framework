#pragma once

#include "game/game_state.h"
#include "game/build_map.h"
#include "game/game_engine.h"

class IRenderer
{
public:
    virtual ~IRenderer() = default;

    virtual bool initialize() = 0;
    virtual bool isOpen() const = 0;
    virtual void processEvents() = 0;

    virtual void render(
        const GameState& state,
        const Map& map
    ) = 0;
};