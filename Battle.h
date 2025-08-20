//Syeda afnan hussain,  SE-A
#ifndef BATTLE_H
#define BATTLE_H
#include "Pet.h"
#include "Arena.h"
#include <string>

class Battle
 {
    Arena* arena;
    int arenaW, arenaH;
public:
    Battle(Arena* a, int w, int h);
    void startInteractiveBattle(Pet* playerPet, Pet* botPet);
};
#endif