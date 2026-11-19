# Diary


# Development Log – AD Framework

**Developer:** Leonard Kämpf
**Project:** AD Framework
**Role:** Framework and Game Engine Development

This document records weekly development activities, technical decisions, problems, and upcoming tasks.

## Week 47 (16.11. – 20.11.2026)

### Activities

- Added five GoogleTest cases for REQ-F-002 to REQ-F-007. They check a wall cell, two-agent operation, DLL loading and unloading, the round counter, and what each agent sees when choosing a move.
- Updated the DLL adapter so the engine returns to WaitingForPlayers when either agent is unloaded. Reloading both agents lets the game resume.
- Four of the five new cases passed.
- Updated the Gantt-Charts according to the decisions.

### Decisions

- Allow one to two more weeks for core tests. Start the generator on 30 November if the tests pass, or on 7 December if fixes need another week.
- Generate asymmetric maps and use cost functions to assess fairness from the first version.

### Open Issues / Next Steps

- REQ-F-007 failed. When Agent 1 moves left from (4, 3), Agent 2 already sees that cell as empty. Build both views and collect both actions before applying either move.
- Test conflicting moves (REQ-F-008), sight boundaries, the protection radius and flag handling with two DLL agents. The representation of carried flags in the DLL snapshot is still open.

---

## Week 46 (09.11. – 13.11.2026)

### Activities

- Fixed the Windows build on 11 November by correcting errors in previously integrated LLM-generated code.
- Added GoogleTest-based engine tests with scripted agents and a dedicated test script.
- Passed the first three test cases for REQ-F-017–019: blocked movement with failure feedback, successful movement with success feedback, and deliberate waiting correctly replacing a previous failure.
- Implemented the missing one-cell protection radius around agents to prevent movement into directly adjacent cells, including diagonals.

### Decisions

- Link automated tests to requirement IDs and use scripted agents for reproducible scenarios.
- Keep the protection-radius fix pending verification until dedicated tests pass.

### Resolved Issues

- Resolved the failed Windows build carried over from Week 45.
- Established initial automated verification of movement handling and action feedback in the following round.

### Open Issues / Next Steps

- Test the protection radius, including diagonal adjacency, boundary distances, and movement by both agents.
- Complete verification of both renderer views and DLL integration under Windows, including flag pickup and placement with two DLL agents.
- Agree on how to distinguish carried flags from flags on the ground in the DLL snapshot.
- Extend automated test coverage beyond the first three movement and feedback scenarios.

---

## Week 45 (02.11. – 06.11.2026)

### Activities

- Reworked the map and 5×5 agent observation to use packed 5-bit cell codes.
- Updated flag handling, both renderer views, and the DLL snapshot conversion.

### Decisions

- Use 22 explicit cell states and reserve the remaining ten codes.
- Store flag ownership by team internally and translate it to own/enemy when building the DLL snapshot.

### Open Issues / Next Steps

- Resolve the failed Windows build, then verify both renderer views and DLL integration.
- Agree on how to distinguish carried flags from flags on the ground in the DLL snapshot.

---

## Week 44 (26.10. – 30.10.2026)

### Activities
- Updated the Gantt chart again to reflect the mathematical modeling and two-stage map generation milestones.
- Started with Literature Research for Fair Maze Generation. Paper planned of taking into account are listed below.

### Decisions
- Due to the research findings I decided to go with a two-step maze generator, as **Rupp, F., & Eckert, K. (2025)** stated, that this is the best way so the Optimization of the map will converge

### Open Issues / Next Steps
- Implement the graph-theoretic and entropy metrics ($S^{(r=2)}$, $\Omega_{ij}$, $H_{\text{deg}}$, $D_4$-asymmetry, and $|V_{\text{detect}}|$) in code.
- Implement the Stage 1 braided maze generator and Stage 2 Simulated Annealing optimization loop.
- Benchmark convergence times of the cost function $\mathcal{J}(G)$ across different grid sizes.
- Finalize the representation of flags, bases, and agent states and test the execution cycle with a reference agent.

### Literature & References

(I am far from finished reading and understanding)

#### 1. Search Information & Path Entropy
- **Rosvall, M., Trusina, A., Minnhagen, P., & Sneppen, K. (2005).** *Networks and cities: An information perspective.* Physical Review Letters, 94(2), 028701.  
  DOI: [10.1103/PhysRevLett.94.028701](https://doi.org/10.1103/PhysRevLett.94.028701)
- **Sneppen, K., Trusina, A., & Rosvall, M. (2005).** *Hide-and-seek on complex networks.* Europhysics Letters (EPL), 69(5), 853–859.  
  DOI: [10.1209/epl/i2004-10422-0](https://doi.org/10.1209/epl/i2004-10422-0)

#### 2. Effective Resistance Distance, Graph Laplacian & Random Walks
- **Chandra, A. K., Raghavan, P., Ruzzo, W. L., Smolensky, R., & Tiwari, P. (1989).** *The electrical resistance of a graph captures its commute and cover times.* Proceedings of the 21st Annual ACM Symposium on Theory of Computing (STOC), 574–586.  
  DOI: [10.1145/73007.73062](https://doi.org/10.1145/73007.73062)


#### 3. Graph Spanners & Local Stretch Factor (Sensor vs. Walking Metric)
- **Narasimhan, G., & Smid, M. (2007).** *Geometric Spanner Networks.* Cambridge University Press.  
  DOI: [10.1017/CBO9780511546884](https://doi.org/10.1017/CBO9780511546884)

#### 4. Occupancy Grid Shannon Entropy
- **Gabriely, Y., & Rimon, E. (2003).** *Competitive on-line coverage of grid environments by a mobile robot.* Computational Geometry, 24(3), 197–224.
  DOI: [10.1016/S0925-7721(02)00110-4](https://doi.org/10.1016/S0925-7721(02)00110-4)

#### 5. Structural Graph Entropy & Symmetry Orbits
- **Dehmer, M., & Mowshowitz, A. (2011).** *A history of graph entropy measures.* Information Sciences, 181(1), 57–78.  
  DOI: [10.1016/j.ins.2010.08.041](https://doi.org/10.1016/j.ins.2010.08.041)

#### 6. Two-Stage SBPCG & Asymmetric Map Balancing
- **Togelius, J., Yannakakis, G. N., Stanley, K. O., & Browne, C. (2011).** *Search-based procedural content generation: A taxonomy and survey.* IEEE Transactions on Computational Intelligence and AI in Games, 3(3), 172–186.  
  DOI: [10.1109/TCIAIG.2011.2148116](https://doi.org/10.1109/TCIAIG.2011.2148116)
- **Ashlock, D., Lee, C., & McGuinness, C. (2011).** *Search-based procedural generation of maze-like levels.* IEEE Transactions on Computational Intelligence and AI in Games, 3(3), 260–273.  
  DOI: [10.1109/TCIAIG.2011.2138707](https://doi.org/10.1109/TCIAIG.2011.2138707)

- **Rupp, F., & Eckert, K. (2025).** *Level the Level: Balancing game levels for asymmetric player archetypes with reinforcement learning.* Proceedings of the 20th International Conference on the Foundations of Digital Games (FDG).  
  DOI: [10.1145/3723498.3723747](https://doi.org/10.1145/3723498.3723747)

---

## Week 43 (19.10. – 23.10.2026)

### Activities
- Updated the project development plan and Gantt chart.
- Reworked the linear development phases into iterative, risk-driven cycles.
- Prepared the project status presentation.

### Decisions
- Use a spiral approach while retaining the existing milestones.

### Open Issues / Next Steps
- Complete the representation of flags, bases, and agent states.
- Test the execution cycle with a reference agent.
- Develop a map generator with fairness constraints.
- Improve the graphical user interface.

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


## Development Roadmap

The project estimate is 300 hours.

| When | Work | Review outcome or target |
|---|---|---|
| September – 23 October | Requirements, architecture and project planning | Project plan and agent interface prepared for the October review |
| October – 19 November | Framework prototype, DLL integration and first engine tests | Two DLL agents can leave and rejoin. Seven of eight recorded test cases pass. Round ordering is open. |
| 23–27 November, with 30 November–4 December if needed | Complete the core engine tests and rerun the affected cases | Both agents decide from the same round state. Conflicting actions, sight limits, the protection radius and DLL flag interactions have recorded results. |
| 7–18 December in the two-week plan | Generate asymmetric candidates, enforce connectivity and score fairness from the first version | Reproducible maps and a fairness score before and after optimisation for each seed |
| 4–29 January | Integrate generated maps, run benchmarks and regressions, complete the evaluation and developer guide | Evaluated generator by **29 January 2027** with recorded seeds, costs and runtimes |
| 1 February | Package the framework and hand it over | **1 February 2027** release target |

## Software Engineering

```mermaid
gantt
    title Software Engineering
    dateFormat YYYY-MM-DD
    axisFormat %d %b
    tickInterval 2week
    weekday monday
    todayMarker off

    section Framework
    Requirements and architecture        :done, se1, 2026-09-16, 30d
    Engine and agent API                 :done, se2, after se1, 27d

    section Tests
    Engine and DLL tests                 :active, se3, after se2, 18d
    Retests if needed                    :se4, after se3, 7d

    section Integration
    Map input and seed handling          :se5, after se4, 14d
    Year-end reserve                     :se6, after se5, 14d
    Map integration and replay           :se7, after se6, 14d

    section Release
    Regression tests and CI              :se8, after se7, 7d
    Developer guide and handover         :se9, after se8, 7d
    Framework release                    :milestone, se9m, after se9, 0d
```

## Algorithms

```mermaid
gantt
    title Algorithms
    dateFormat YYYY-MM-DD
    axisFormat %d %b
    tickInterval 2week
    weekday monday
    todayMarker off

    section Preparation
    Literature and initial fairness criteria :done, a1, 2026-10-01, 32d

    section Dependency
    Framework work and tests             :a2, after a1, 35d

    section Generator
    Asymmetric maps and fairness costs   :a3, after a2, 7d
    Optimise and check connectivity      :a4, after a3, 7d
    Year-end reserve                     :a5, after a4, 14d

    section Evaluation
    Runs with fixed seeds                :a6, after a5, 7d
    Benchmark costs and runtime          :a7, after a6, 14d
    Evaluate results                     :a8, after a7, 4d
    Evaluation complete                  :milestone, a8m, after a8, 0d
```

Structural fairness does not imply identical win rates.
Advanced agent development is reserved for future student cohorts.
