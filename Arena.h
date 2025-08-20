//Syeda afnan hussain, SE-A
#ifndef ARENA_H
#define ARENA_H
#include "Pet.h"
#include "Item.h"
#include <string>
#include <iostream>
using namespace std;

class PowerUp
{
private:
    int type;       
    int value;
    int duration; /*number of turns for which it lasts*/
    bool active;    
    
public:
    PowerUp(int t, int v, int d) : type(t), value(v), duration(d), active(false) {}
    
    int getType() const 
    {
         return type;
          }
    int getValue() const 
    {
         return value;
          }
    int getDuration() const
     {
         return duration;
          }
    bool isActive() const 
    {
         return active; 
         }
    
    void setActive(bool isActive) 
    {
         active = isActive; 
         }
    void decrementDuration() 
    {
         if (duration > 0) duration--;
          }
};

class Arena;

class BattleTeam 
{
private:
    static const int MAX_TEAM_SIZE = 5;
    Pet** pets;
    int petCount;
    bool isPlayerTeam;
    PowerUp* currentPowerUp;

public:
    BattleTeam(bool isPlayer) : petCount(0), isPlayerTeam(isPlayer), currentPowerUp(nullptr)
    {
        pets = new Pet*[MAX_TEAM_SIZE];
        for (int i = 0; i < MAX_TEAM_SIZE; ++i)
        {
            pets[i] = nullptr;
        }
    }

    ~BattleTeam()
    {
        delete[] pets;
        delete currentPowerUp;
    }
    
    bool addPet(Pet* pet) 
    {
        if (petCount >= MAX_TEAM_SIZE) return false;
        pets[petCount++] = pet;
        return true;
    }
    
    int getPetCount() const 
    {
        return petCount; 
    }
    
    Pet* getPet(int index) const 
    {
        if(index >= 0 && index < petCount)
        {
            return pets[index];
        }
        else
        {
            return nullptr;
        }
    }
    
    bool isPlayer() const 
    {
        return isPlayerTeam; 
    }
    
    PowerUp* getCurrentPowerUp() const 
    {
        return currentPowerUp;
    }
    
    void setPowerUp(PowerUp* powerUp)
    { 
        currentPowerUp = powerUp; 
    }
    
    void setCurrentPowerUp(PowerUp* powerUp)
    {
        currentPowerUp = powerUp;
    }
    
    Pet** getPets() const { return pets; }
    
    bool hasPet(int index) const
    {
        return (index >= 0 && index < petCount && pets[index] != nullptr);
    }
    
    int getMaxTeamSize() const { return MAX_TEAM_SIZE; }
    
    friend class Arena;
};

class ActiveItemEffect 
{
private:
    Item* item;
    Pet* target;
    int remainingTurns;

public:
    ActiveItemEffect() 
    {
        item = nullptr;
        target = nullptr;
        remainingTurns = 0;
    }
    
    ActiveItemEffect(Item* i, Pet* t, int turns) 
    {
        item = i;
        target = t;
        remainingTurns = turns;
    }
    
    Item* getItem() const 
    {
        return item;
    }
    
    Pet* getTarget() const 
    {
        return target;
    }
    
    int getRemainingTurns() const
    {
        return remainingTurns;
    }
    
    void decrementTurns()
    {
        if (remainingTurns > 0) 
        {
            remainingTurns--; 
        }
    }
    
    friend class Arena;
};

class Arena 
{
private:
    static const int ARENA_WIDTH = 9;  
    static const int ARENA_HEIGHT = 7;  
    static const int MAX_TEAM_SIZE = 5;  
    static const int POWER_UP_CHANCE = 20; /*There is 20% chance that powerups might appear*/
    static const int MAX_OBSTACLES = 10;  
    static const int MAX_ACTIVE_EFFECTS = 10;  /*Decreased for performance*/
    static const int BATTLE_TYPE_1V1 = 1;     
    static const int BATTLE_TYPE_2V2 = 2;      
    static const int BATTLE_TYPE_GUILD_WAR = 3;  

    static const int POWER_UP_HEALTH = 1;
    static const int POWER_UP_ATTACK = 2;
    static const int POWER_UP_DEFENSE = 3;
    static const int POWER_UP_SPEED = 4;
    static const int MAX_POWER_UP_TYPES = 4;
    
    string* battleLog;
    int logCount;
    int logCapacity;
    int currentBattleType;
    BattleTeam* team1;
    BattleTeam* team2;
    ActiveItemEffect* activeEffects;
    int activeEffectCount;
    char grid[ARENA_HEIGHT][ARENA_WIDTH];

public:
    Arena();
    ~Arena();
    void setupBattle(int type);
    void setupOneVsOne(Pet* playerPet, Pet* botPet);
    void setupTwoVsTwo(Pet* playerPet1, Pet* playerPet2, Pet* botPet1, Pet* botPet2);
    void setupGuildWar(Pet** playerPets, Pet** botPets);
    
    void showArena();
    void showPet(Pet* pet, bool isPlayer);
    void showGrid();
    void updateLog(const string& logEntry);
    void showLog() const;
    void clearScreen() const;
    
    void startBattle();
    void processTurn(int teamIndex, int petIndex, int action, int targetIndex);
    bool isBattleOver() const;
    BattleTeam* getWinningTeam() const;
    
    void checkForPowerUp();
    void applyPowerUp(BattleTeam* team);
    void updatePowerUps();
    PowerUp* generateRandomPowerUp();
   
    void addItemEffect(Item* item, Pet* target);
    void updateItemEffects();
    void removeExpiredEffects();
    
    void placeObstacles();
    bool isWall(int y, int x) const;
    bool isTrap(int y, int x) const;
    
    int getCurrentBattleType() const
    {
        return currentBattleType;
    }
    
    BattleTeam* getTeam1() const 
    {
        return team1; 
    }
    
    BattleTeam* getTeam2() const 
    {
        return team2;
    }
};
#endif 