//Syeda afnan hussain, SE-A
#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include "Pet.h"
#include "Item.h"
using namespace std;

class Player 
{
protected:
    static const int INVENTORY_SIZE = 5;
    std::string name;
    Pet* pet;
    Item* inventory[INVENTORY_SIZE];
    int currency;
public:
    Player(const std::string& n);
    virtual ~Player();
    void selectPet(Pet* availablePets[], int numPets);
    void addItem(Item* item);
    void useItem(int idx);
    Pet* getPet();
    string getName() const;
    int getCurrency() const;
    void earnCurrency(int amount);
    void showInventory() const;
    virtual void performAction(Player* opponent);
};

class Bot : public Player 
{
public:
    Bot(const string& n);
    void performAction(Player* opponent) override;
};

#endif 