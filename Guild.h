//Syeda afnan hussain,  SE-A
#ifndef GUILD_H
#define GUILD_H
#include <string>
#include "Pet.h"
#include <iostream>
using namespace std;

class Guild 
{
public:
    static const int MAX_GUILD_PETS = 5; 
    static const int MAX_GUILDS = 4;
    static const int MAX_BATTLE_LOG = 100;
private:
    string name;
    Pet* pets[MAX_GUILD_PETS];
    string roles[MAX_GUILD_PETS];
    int petCount;/*Pets currently in the guilds*/
    int wins;
    int losses;
    int ranking;
    string battleLog[MAX_BATTLE_LOG]; /*It is like the commentary, it holds the comentaries
     of the battle between the two guilds, records all the battles  the guild has participated*/
    int logCount;

public:
    Guild(const string& n);
    ~Guild();
    bool addPet(Pet* pet, const string& role);
    void showGuild() const;
    string getName() const;
    int getPetCount() const 
    {
         return petCount; 
         }
    Pet* getPet(int index) const 
    {
         if( index < petCount)
         {
          return pets[index] ;
         }
         else
         {
          return nullptr; 
         }
    }
    string getRole(int index) const 
    {
         if( index < petCount)
         {
          return roles[index] ;
         }
         else
         {
          return ""; 
         }
    }
    void recordWin() 
    { 
        wins++; 
        updateRanking(); 
    }
    void recordLoss() 
    { 
        losses++; 
        updateRanking(); 
    }
    int getRanking() const
     {
         return ranking; 
         }
    int getWins() const
     {
         return wins; 
         }
    int getLosses() const
     {
         return losses;
          }
    void updateRanking();
    void recordBattle(const string& action);
    void showBattleLog() const;
    friend ostream& operator<<(ostream& os, const Guild& guild);
    bool operator==(const Guild& other) const;
    bool operator<(const Guild& other) const;
};

class GuildManager 
{
private:
    Guild** guilds; 
    int guildCount;
    int guildCapacity;

public:
    GuildManager();
    ~GuildManager();
    bool createGuild(const string& name);
    Guild* getGuild(const string& name);
    Guild* getGuild(int index)
     {
         if(index >= 0 && index < guildCount) 
         {
            return guilds[index];
         }
         else
         {
            return  nullptr; 
         }
     }
    int getGuildCount() const 
    {
         return guildCount; 
    }
    void showAllGuilds() const;
    void showGuildRankings() const;
    bool initiateGuildWar(Guild* guild1, Guild* guild2);
    Guild** getTopRankedGuilds(int& count) const;
};
class GuildWar 
{
private:
    Guild* guild1;
    Guild* guild2;
    string* battleLog;  /*Records only the current war events basically the current war commentary*/
    int logCount;
    int logCapacity;
    bool isActive;
public:
    GuildWar(Guild* g1, Guild* g2);
    ~GuildWar();
    void startWar();
    void recordBattle(const string& action);
    void showBattleLog() const;
    Guild* determineWinner();
    bool isWarActive() const
     {
         return isActive; 
         }
    void endWar();
};
#endif 