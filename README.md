# Diary


# Development Log – AD Framework

**Developer:** Leonard Kämpf
**Project:** AD Framework
**Role:** Framework and Game Engine Development

This document records weekly development activities, technical decisions, problems, and upcoming tasks.

---

## Week 42 (12.10. – 16.10.2026)

### Activities
- Reviewed the existing framework architecture and agent interfaces.
- Prepared the project development plan and documentation.
- Planned integration testing using a simple reference agent.
- Reviewed the requirements for host-agent communication.
- Integration of given Agent Interface

### Decisions
- Adjusting current version to use the provided Agent Interface.

### Open Issues / Next Steps
- Verify game-state synchronization.
- Test the complete execution cycle with a reference agent.
- Test possibilities to transfer more fields between Host and Agent in the same amount of Bits

---

## Week 41 (05.10. – 09.10.2026)

### Activities
- Continued development of the observation system.
- Reviewed movement handling and agent interaction.
- Investigated the separation between game logic and rendering.

### Decisions
- Agents receive information only through the defined observation interface.
- Game logic and graphical rendering remain independent.

### Open Issues / Next Steps
- Validate the observation window near map boundaries.
- Complete the agent-specific visualization.
- Prepare interface tests.

---

## Week 40 (28.09. – 02.10.2026)

### Activities
- Implemented a compact map representation using four bits per cell.
- Implemented bitwise encoding and decoding of map cells.
- Introduced a 5x5 observation area for agents.
- Integrated graphical visualization using SFML.
- Fixed compilation and linker errors.

### Decisions
- Store two map cells per byte using `uint8_t`.
- Use a fixed observation radius of two cells.

### Problems
- Incorrect number of visible cells in the initial observation view.
- Multiple-definition linker error in movement helper functions.

### Next Steps
- Validate observation extraction.
- Continue agent interface integration.

---

## Week 39 (21.09. – 25.09.2026)

### Activities
- Established the modular C++ project structure.
- Implemented initial DLL plugin loading.
- Introduced PluginManager and GameEngine components.
- Created a build script using MSYS2.
- Resolved compilation and initialization errors.

### Decisions
- Separate game engine, plugin system, and renderer.
- Use DLL-based plugins for agent integration.

### Problems
- Incorrect header include paths.
- GameEngine constructor failed due to missing Map initialization.

### Next Steps
- Integrate map and agent interfaces.
- Implement observation handling.

---

## Week 38 (14.09. – 18.09.2026)

### Activities
- Analyzed functional and non-functional requirements.
- Developed the initial software architecture.
- Investigated communication between framework and agents.
- Defined preliminary module responsibilities.

### Decisions
- Implement the framework in C++.
- Keep agent implementations independent of the host.
- Separate game logic from visualization.

### Next Steps
- Establish the project structure.
- Implement dynamic plugin loading.



## Project Timeline

The following Gantt chart shows the development progress and planned milestones of the AD Framework.

```mermaid
gantt
    title AD Framework – Development Timeline
    dateFormat  YYYY-MM-DD
    axisFormat  %d.%m.

    section Requirements & Architecture
    Requirements Analysis             :done, req,  2026-09-14, 7d
    Architecture Design               :done, arch, 2026-09-18, 8d
    Agent / Framework Interfaces      :done, intf, 2026-09-21, 10d

    section Framework Core
    Plugin System                     :done, plug, 2026-09-21, 7d
    Game Engine & Game States         :done, eng,  2026-09-24, 10d
    Packed Map Representation         :done, map,  2026-09-28, 8d
    Agent Observation (5x5 Vision)    :done, obs,  2026-09-30, 10d

    section Integration
    Agent Interaction Logic           :active, interact, 2026-10-06, 14d
    Agent View Renderer               :active, render, 2026-10-10, 10d
    Reference / Test Agent            :agent, 2026-10-15, 10d
    Game State Synchronization        :sync,  2026-10-18, 10d

    section Testing & Validation
    Interface Tests                   :tests, 2026-10-22, 10d
    Game Logic Tests                  :logic, 2026-10-25, 10d
    Plugin Integration Tests          :ptest, 2026-10-28, 8d

    section Finalization
    Documentation                     :docs, 2026-10-20, 21d
    Refactoring & Cleanup             :clean, 2026-11-02, 10d
    Framework Prototype Complete      :milestone, m1, 2026-11-12, 0d
```