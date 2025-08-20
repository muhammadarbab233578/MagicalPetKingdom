//Syeda afnan hussain,  SE-A
#ifndef LEADERBOARD_H
#define LEADERBOARD_H
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

// Separate PlayerScore class
class PlayerScore 
{
private:
    string name;
    int wins;

public:
    PlayerScore() 
     {
        name="";
        wins=0;
     }
    PlayerScore(const string& playerName, int playerWins = 0) 
    {
        name=playerName;
        wins=playerWins;
    } 
    
    string getName() const 
    {
         return name;
     }
    int getWins() const
     { 
        return wins;
      }
    void setName(const string& playerName) 
    {
         name = playerName; 
         }
    void setWins(int playerWins) 
    {
         wins = playerWins; 
         }
    void incrementWins() 
    { 
        wins++;
         }
    
    bool operator<(const PlayerScore& other) const
     {
         return (this->wins < other.wins); 
     }
    bool operator>(const PlayerScore& other) const 
    { 
        return (this->wins > other.wins); 
    }
    bool operator==(const PlayerScore& other) const 
    { 
        return (this->name == other.name); 
    }
};

class Leaderboard 
{
private:
    static const int MAX_PLAYERS = 10;
    PlayerScore scores[MAX_PLAYERS];
    int playerCount;
    string filename;
public:
    Leaderboard(const string& fname = "leaderboard.txt");
    void updateScore(const string& playerName, bool won);
    void displayTop5();
    void saveToFile();
    void loadFromFile();
};
#endif 