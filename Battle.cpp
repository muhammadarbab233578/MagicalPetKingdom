//Syeda afnan hussain,  SE-A
/*This is the class where all the types of battles take place ,
basically this calass handles the battles, and its log and stats*/

#include "Battle.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
using namespace std;

Battle::Battle(Arena* a, int w, int h)
{
    arena = a;
    arenaW = w;
    arenaH = h;
}
void Battle::startInteractiveBattle(Pet* playerPet, Pet* botPet)
{
    /*Here we are placing the pets oooposite to eachother*/
    playerPet->setPosition(0, 0);
    botPet->setPosition(arenaW - 1, arenaH - 1);
    srand(static_cast<unsigned int>(time(0)));
    string lastAction = "Battle begins!";
    while (playerPet->getHealth() > 0 && botPet->getHealth() > 0)
    {
        arena->updateLog(lastAction);
        arena->showArena();
        cout << "\nChoose your action:" << endl;
        cout << "1) Move" << endl;
        cout << "2) Attack" << endl;
        cout << "3) Use Ability" << endl;
        cout << "4) Defend" << endl;
        int choice;
        while (!(cin >> choice) || choice < 1 || choice > 4)
        {
            cin.clear();
            cout << "Invalid input! Please enter a number between 1 and 4" << endl;
            cin.ignore(100000, '\n');
        }
        int pdx = 0, pdy = 0;

        if (choice == 1)
        {
            cout << "Move direction: 1) Up 2) Down 3) Left 4) Right" << endl;
            int dir;
            cin >> dir;
            if (dir == 1)
            {
                pdy = -1;
            }
            else if (dir == 2)
            {
                pdy = 1;
            }
            else if (dir == 3)
                pdx = -1;
            else if (dir == 4)
            {
                pdx = 1;
            }
            playerPet->move(pdx, pdy, arenaW, arenaH, arena);
            lastAction = "Player moved. ";
        }
        else if (choice == 2)
        {
            if (playerPet->distanceTo(botPet) == 1)
            {
                playerPet->attack(botPet);
                lastAction = "Player attacked! ";
            }
            else
            {
                lastAction = "Too far to attack! ";
            }
        }
        else if (choice == 3)
        {
            int abilityNumber = 1;
            cout << "Choose ability: 1) First Ability 2) Second Ability" << endl;
            cin >> abilityNumber;
            if (cin.fail() || (abilityNumber != 1 && abilityNumber != 2))
            {
                abilityNumber = 1;
            }
            playerPet->useAbility(botPet, abilityNumber);
            lastAction = "Player used ability! ";
        }
        else if (choice == 4)
        {
            lastAction = "Player defends! ";
        }
        else
        {
            lastAction = "Invalid action! ";
        }
        int botAct = rand() % 4;
        int bdx = 0, bdy = 0;
        if (botAct == 0)
        {
            int dir = rand() % 4;
            if (dir == 0)
            {
                bdy = -1;
            }
            else if (dir == 1)
            {
                bdy = 1;
            }
            else if (dir == 2)
            {
                bdx = -1;
            }
            else if (dir == 3)
            {
                bdx = 1;
            }
            botPet->move(bdx, bdy, arenaW, arenaH, arena);
            lastAction += "Bot moved.";
        }
        else if (botAct == 1)
        {
            if (botPet->distanceTo(playerPet) == 1)
            {
                botPet->attack(playerPet);
                lastAction += "Bot attacked!";
            }
            else
            {
                lastAction += "Bot too far to attack!";
            }
        }
        else if (botAct == 2)
        {
            int botAbility = 1;
            if (botPet->ability2Unlocked)
            {
                botAbility = (rand() % 2) + 1;
            }
            botPet->useAbility(playerPet, botAbility);
            lastAction += "Bot used ability!";
        }
        else if (botAct == 3)
        {
            lastAction += "Bot defends!";
        }
    }
    arena->updateLog(playerPet->getHealth() > 0 ? "You win!" : "Bot wins!");
    arena->showArena();
    if (playerPet->getHealth() > 0)
    {
        cout << "\nYou win!" << endl;
    }
    else
    {
        cout << "\nBot wins!" << endl;
    }
}