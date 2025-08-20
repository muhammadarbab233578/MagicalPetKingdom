//Syeda afnan hussain,  SE-A
#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <iostream>
using namespace std;
class Pet;

class Item
 {
protected:
    string name;
    int price;
    int duration;  /*Duration in turns for temporary effects*/

public:
    Item(const string& name, int price, int duration = 0);
    virtual ~Item() = default; 

    string getName() const;
    int getPrice() const;
    int getDuration() const;
    virtual void use(Pet* target) = 0; 
    virtual void removeEffect(Pet* target) = 0;  /*Remove temporary effects*/
    friend std::ostream& operator<<(std::ostream& os, const Item& item);
    bool operator==(const Item& other) const;
    bool operator<(const Item& other) const;
};

class HealingPotion : public Item
 {
private:
    int healAmount;

public:
    HealingPotion();
    void use(Pet* target) override;
    void removeEffect(Pet* target) override;
};

class ManaPotion : public Item 
{
private:
    int abilityAmount;
    int originalAbilityUses;

public:
    ManaPotion();
    void use(Pet* target) override;
    void removeEffect(Pet* target) override;
};

class BuffItem : public Item 
{
private:
    int buffAmount;
    int buffType; /*0 = attack, 1 = speed*/
    int originalStat;

public:
    BuffItem();
    void use(Pet* user) override;
    void removeEffect(Pet* user) override;
};
class Shield : public Item 
{
private:
    int shieldAmount;
    int originalDefense;

public:
    Shield();
    void use(Pet* target) override;
    void removeEffect(Pet* target) override;
};
#endif 