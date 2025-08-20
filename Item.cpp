//Syeda afnan hussain,  SE-A
#include "Item.h"
#include "Pet.h"
#include <iostream>
using namespace std;

Item::Item(const string& n, int p, int d)
 {
    name = n;
    price = p;
    duration = d;
}

string Item::getName() const
 { 
    return name;
}

int Item::getPrice() const 
{ 
    return price;
}

int Item::getDuration() const
 {
    return duration;
}

HealingPotion::HealingPotion() : Item("Healing Potion", 50, 0) 
{
    healAmount = 12;
}

void HealingPotion::use(Pet* user)
 {
    cout << "Used Healing Potion on " << user->getName() << endl;
    user->heal(healAmount);
}

void HealingPotion::removeEffect(Pet* user)
{
    /*Healing potion is not removed from the pet becuz
     the pet's health increase is permanant*/
}

ManaPotion::ManaPotion() : Item("Mana Potion", 30, 0)
{
    abilityAmount = 2;
}

void ManaPotion::use(Pet* user)
{
    cout << "Used Mana Potion on " << user->getName() << "! Restored " << abilityAmount << " ability uses" << endl;
    user->restoreAbilityUses(abilityAmount);
}

void ManaPotion::removeEffect(Pet* user)
{
    /*Manapotion is not removed from the pet becuz
     the pet's ability uses increase is permanant*/
}

BuffItem::BuffItem() : Item("Buff Item", 60, 3) 
{
    buffAmount = 10;
    buffType = 0;/*So that we know which stat is increased adn is to be restrored*/
    originalStat = 0;
}

void BuffItem::use(Pet* user) 
{
    cout << "Choose stat to buff:"<<endl;
    cout<<"1) Attack"<<endl;
    cout<<"2) Speed"<<endl;
    int choice;
    cin >> choice;
    while(!(cin>>choice))
    {
        cin.clear();
        cout<<"Invalid input! Please enter a valid number"<<endl;
        cin.ignore(100000,'\n');
    }
    if (choice == 1)
     {
        originalStat = user->getAttackPower();
        user->increaseAttack(buffAmount);
        buffType = 0;
        cout<<"Attack increased by "<<buffAmount<< "for "<<duration<<" turns"<<endl;
    } 
    else if (choice == 2) 
    {
        originalStat = user->getSpeed();
        user->increaseSpeed(buffAmount);
        buffType = 1;
        cout << "Speed increased by " << buffAmount << " for " << duration << " turns." << endl;
    } 
    else 
    {
        cout << "Invalid choice! No buff applied"<<endl;
        buffType = -1;
    }
}
void BuffItem::removeEffect(Pet* user) 
{
    if (buffType == 0)
     {
        user->setAttackPower(originalStat);
        cout<<"Attack buff wore off!"<<endl;
    } 
    else if (buffType == 1) 
    {
        user->setSpeed(originalStat);
        cout<<"Speed buff wore off!"<<endl;
    }
}
Shield::Shield() : Item("Shield", 20, 3)
 {
    shieldAmount = 5;
    originalDefense = 0;
}
void Shield::use(Pet* user)
 {
    cout<<"Used Shield on "<<user->getName()<<"! Defense increased by "<<shieldAmount<<" for "<<duration<<" turns"<<endl;
    originalDefense = user->getDefense();
    user->increaseDefense(shieldAmount);
}
void Shield::removeEffect(Pet* user)
 {
    cout<<"Shield effect wore off on "<<user->getName()<<endl;
    user->setDefense(originalDefense);
}

std::ostream& operator<<(ostream& COUT, const Item& item)
 {
    COUT << "Item: " << item.getName() << " | Price: " << item.getPrice() << " | Duration: " << item.getDuration();
    return COUT;
}
bool Item::operator==(const Item& other) const 
{
    return (this-> name == other.name);
}
bool Item::operator<(const Item& other) const
 {
    return (this-> name < other.name);
} 