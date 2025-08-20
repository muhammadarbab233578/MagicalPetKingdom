//Syeda Afnan hussain ,  SE-A
/*The purpose of this class is to show the leadership boards, which is basically the rankings of all the
playersbased ont the number of matches they have won and it displays top 5 players*/
#include "Leaderboard.h"

Leaderboard::Leaderboard(const string& fname) 
{
    filename = fname;
    playerCount = 0;
    loadFromFile();
}

void Leaderboard::updateScore(const string& playerName, bool won)
 {
    /* Finding the player in the array*/
    for (int i = 0; i < playerCount; i++) 
    {
        if (scores[i].getName() == playerName)
         {
            if (won)
            {
                scores[i].incrementWins();
            }
            return;
        }
    }
    /* If player not found and array not full add new player basically validation handling*/
    if (playerCount < MAX_PLAYERS)
     {
        scores[playerCount] = PlayerScore(playerName, won ? 1 : 0);
        playerCount++;
    }
    saveToFile();
}
void Leaderboard::displayTop5() 
{
    for (int i = 0; i < playerCount - 1; i++)
     {
        for (int j = 0; j < playerCount - i - 1; j++) 
        {
            if (scores[j].getWins() < scores[j + 1].getWins())
             {
                PlayerScore temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }
    cout<<endl;
    cout << "=== Top 5 Players ==="<<endl;
    int displayCount = (playerCount < 5) ? playerCount : 5;
    for (int i = 0; i < displayCount; i++) 
    {
        cout << i + 1 << ". " << scores[i].getName() << " - " << scores[i].getWins() << " wins"<<endl;
    }
    cout << "===================\n";
}
void Leaderboard::saveToFile()
{
    ofstream file(filename);
    if (file.is_open()) 
    {
        for (int i = 0; i < playerCount; i++)
         {
            file << scores[i].getName() << " " << scores[i].getWins() <<endl;
        }
        file.close();
    }
}
void Leaderboard::loadFromFile() 
{
    ifstream file(filename);
    if (file.is_open()) 
    {
        playerCount = 0;
        string name;
        int wins;
        
        while (playerCount < MAX_PLAYERS && file >> name >> wins)
        {
            scores[playerCount] = PlayerScore(name, wins);
            playerCount++;
        }
        file.close();
    }
} 