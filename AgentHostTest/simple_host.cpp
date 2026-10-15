// Compilation and linking:
// g++ -c simple_host.cpp
// g++ simple_host.o agent_loader.o -o simple_host.exe
//
// Performing tests with one or two players:
// simple_host simple_agent.dll
// simple_host simple_agent.dll simple_agent.dll


#include <stdio.h>
#include "agent_loader.h"


#define MAP_WIDTH   80
#define MAP_HEIGHT  50


int main(int argc, char** argv)
{
    PlayerInstance players[2];

    if ((argc >= 2) && (players[0].Load(argv[1], MAP_WIDTH, MAP_HEIGHT))) {
        printf("Successfully loaded 1st player: %s\n", players[0].GetName());
    } else {
        printf("Could not load player DLL %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    if (argc >= 3) {
        if (players[1].Load(argv[2], MAP_WIDTH, MAP_HEIGHT)) {
            printf("Successfully loaded 2nd player: %s\n", players[1].GetName());
        } else {
            printf("Could not load player DLL %s\n", argv[2]);
            return EXIT_FAILURE;
        }
    }

    WorldSnapshot snapshot{};
    for (int y = 0; y < VIEW_SIZE; ++y) {
        for (int x = 0; x < VIEW_SIZE; ++x) {
            snapshot.cells[y][x] = CELLTYPE_FLOOR | CELLCONTENT_NONE;
        }
    }

    for (uint32_t k = 0; k < 5; ++k) {
        for (int p = 0; p < 2; ++p) {
            if (players[p].ready) {
                printf("HOST: Calling agent %d\n", p+1);
                uint32_t action = players[p].Step(&snapshot);
                printf("HOST: Return value of call was %u\n", action);
                players[p].lastAction = action;  // remember action that could actually be executed
            }
        }
        printf("\n");
    }

    printf("HOST: Calling agents' reset function\n");
    for (int p = 0; p < 2; ++p) {
        players[p].Reset();
    }
    printf("\n");

    for (uint32_t k = 0; k < 2; ++k) {
        for (int p = 0; p < 2; ++p) {
            if (players[p].ready) {
                printf("HOST: Calling agent %d\n", p+1);
                uint32_t action = players[p].Step(&snapshot);
                printf("HOST: Return value of call was %u\n", action);
                players[p].lastAction = action;  // remember action that could actually be executed
            }
        }
        printf("\n");
    }

    players[0].Unload();
    players[1].Unload();
}
