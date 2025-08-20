//Syeda afnan hussain, SE-A
#ifndef GAME_H
#define GAME_H
#include "Player.h"
#include "Item.h"
#include "Arena.h"
#include "Guild.h"
#include "TrainingCamp.h"
#include "Shop.h"
#include "Leaderboard.h"

class Game 
{
    Player* player;
    Bot* bot;
    Pet* availablePets[4];
    Item* shopItems[4];
    Arena* arena;
    GuildManager* guildManager;
    TrainingCamp* trainingCamp;
    Shop* shop;
    Leaderboard* leaderboard;
public:
    Game();
    ~Game();
    void run();
    void showShop();
    void saveProgress();
    void loadProgress();
private:
    void battleLoop();
    void guildMenu();
    void trainingMenu();
    void shopMenu();
    void battleMenu();
};
#endif 