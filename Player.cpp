//Syeda Afnan Hussain ,  SE-A
/*The purpose of this class is to manage inventory and players as well as bot's actiosn
like adding items to invenotry, selecting pets, performing actions and many mroe*/
#include "Player.h"
#include <iostream>
#include <cstdlib>
using namespace std;

Player::Player(const string& n)
{
    name = n;
    pet = nullptr;
    currency = 100;
    for (int i = 0; i < INVENTORY_SIZE; ++i)
    {
        inventory[i] = nullptr;
    }
}

Player::~Player()
{
    if (pet)
    {
        delete pet;
    }
    for (int i = 0; i < INVENTORY_SIZE; ++i)
    {
        if (inventory[i])
        {
            delete inventory[i];
        }
    }
}

void Player::selectPet(Pet* availablePets[], int numPets)
{
    cout << "Select your pet:" << endl;
    for (int i = 0; i < numPets; ++i)
    {
        cout << i + 1 << ". " << availablePets[i]->getName() << endl;
    }
    int choice;
    while (!(cin >> choice) || choice < 1 || choice > numPets)
    {
        cin.clear();
        cout << "Invalid input! Please enter a valid number" << endl;
        cin.ignore(100000, '\n');
    }
    pet = availablePets[choice - 1];
    cin.ignore(100000, '\n'); // clear buffer

    int nameChoice;
    cout << "Would you like to name your pet? (1 for Yes, 0 for No): ";
    while (!(cin >> nameChoice) || (nameChoice != 0 && nameChoice != 1))
    {
        cin.clear();
        cout << "Invalid input! Please enter 1 for Yes or 0 for No" << endl;
        cin.ignore(100000, '\n');
    }
    cin.ignore(100000, '\n'); // clear buffer

    if (nameChoice == 1)
    {
        cout << "Enter the pet's name: ";
        string petName;
        getline(cin, petName);
        pet->setName(petName);
    }
}

void Player::addItem(Item* item)
{
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        if (inventory[i] == nullptr) /*The loop finds the first empty slot and adds the item there*/
        {
            inventory[i] = item;
            return;
        }
    }
    cout << "Inventory full!" << endl;
}

void Player::useItem(int idx)
{
    if (idx < 0 || idx >= INVENTORY_SIZE || inventory[idx] == nullptr)
    {
        cout << "Invalid item!" << endl;
        return;
    }
    inventory[idx]->use(pet);
    delete inventory[idx];
    inventory[idx] = nullptr;
}

Pet* Player::getPet()
{
    return pet;
}
string Player::getName() const
{
    return name;
}
int Player::getCurrency() const
{
    return currency;
}
void Player::earnCurrency(int amount)
{
    currency += amount;
}

void Player::showInventory() const
{
    cout << "Inventory:" << endl;
    bool hasItem = false;
    for (int i = 0; i < INVENTORY_SIZE; ++i)
    {
        if (inventory[i] != nullptr)
        {
            cout << i + 1 << ". " << inventory[i]->getName() << endl;
            hasItem = true;
        }
    }
    if (hasItem == false)
    {
        cout << "Inventory empty" << endl;
    }
}

void Player::performAction(Player* opponent)
{
    cout << "Choose action: " << endl;
    cout << " 1) Attack 2) Use Ability 3) Use Item" << endl;
    int act;
    while (!(cin >> act) || act < 1 || act > 3)
    {
        cin.clear();
        cout << "Invalid input! Please enter 1, 2, or 3" << endl;
        cin.ignore(100000, '\n');
    }
    cin.ignore(100000, '\n'); // clear buffer
    if (act == 1)
    {
        pet->attack(opponent->getPet());
    }
    else if (act == 2)
    {
        int abilityNumber = 1;
        cout << "Choose ability:" << endl;
        cout << " 1) First Ability 2) Second Ability" << endl;
        while (!(cin >> abilityNumber) || (abilityNumber != 1 && abilityNumber != 2))
        {
            cin.clear();
            cout << "Invalid input! Please enter 1 or 2" << endl;
            cin.ignore(100000, '\n');
        }
        cin.ignore(100000, '\n'); // clear buffer
        pet->useAbility(opponent->getPet(), abilityNumber);
    }
    else if (act == 3)
    {
        showInventory();
        cout << "Select item index: ";
        int idx;
        while (!(cin >> idx) || idx < 1 || idx > INVENTORY_SIZE)
        {
            cin.clear();
            cout << "Invalid input! Please enter a valid item index" << endl;
            cin.ignore(100000, '\n');
        }
        cin.ignore(100000, '\n'); // clear buffer
        useItem(idx - 1);
    }
    else cout << "Invalid action!" << endl;
}

/*Bot*/
Bot::Bot(const string& n) : Player(n)
{
    /*empty becuz all required setup is being done in the player constructor*/
}

void Bot::performAction(Player* opponent)
{
    /*Randomly select an action  1 = Attack, 2 = Ability, 3 = Item*/
    int tries = 0;/*This is basically a tracker for the bot's invalid actions , the bot can make only
    3 times invalid actions then after by default automatically attack will be selected and bot will attack*/
    while (tries < 3)
    {
        int action = rand() % 3 + 1;
        if (action == 1)
        {
            pet->attack(opponent->getPet());
            return;
        }
        else if (action == 2)
        {
            int botAbility = 1;
            if (pet->ability2Unlocked) botAbility = (rand() % 2) + 1;
            pet->useAbility(opponent->getPet(), botAbility);
            return;
        }
        else if (action == 3)
        {
            for (int i = 0; i < INVENTORY_SIZE; i++)
            {
                if (inventory[i] != nullptr)
                {
                    inventory[i]->use(pet);
                    delete inventory[i];
                    inventory[i] = nullptr;
                    return;
                }
            }
        }
        tries++;
    }
    /*If no items available then, default is set to attack*/
    pet->attack(opponent->getPet());
}