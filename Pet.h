//Syeda afnan hussain,  SE-A
#ifndef PET_H
#define PET_H
#include <string>
#include "Item.h"
#include <iostream>
using namespace std;

class Arena;
static const int INVENTORY_SIZE = 5;

class Pet
{
protected:
    std::string name;
    int health, maxHealth, attackPower, defense, speed;
    int abilityUses;
    int maxAbilityUses;
    int x, y; /*positions in arena*/
    Item* inventory[INVENTORY_SIZE];

public:
    int speedProgress = 0;
    int attackProgress = 0;
    int defenseProgress = 0;
    int fatigue = 0;
    bool stunned = false;

    bool hasShield = false;
    bool shieldActive = false;
    bool hasSpeedBurst = false;
    int speedBurstTurns = 0;
    bool hasRebirth = false;
    bool rebirthUsed = false;

    int experience = 0;
    int level = 1;
    int experienceToNextLevel = 100;
    int trainingPoints = 0;
    bool ability1Unlocked = true; /*First ability is always unlocked*/
    bool ability2Unlocked = false; /*Second ability must be unlocked using Training points*/

    /*these are the evolution properties*/
    int evolutionStage = 0;
    static const int EVOLUTION_LEVELS[3]; /*Levels at which evolution occurs*/
    bool canEvolve = false;

    Pet(const string& n, int hp, int atk, int def, int spd, int abilityUses = 3);
    virtual ~Pet();

    virtual void attack(Pet* target) = 0;
    virtual void useAbility(Pet* target, int abilityNumber) = 0;
    virtual void takeDamage(int dmg);
    virtual void heal(int amount);
    virtual bool evolve();

    virtual void rebirth() {}

    virtual string getAbility1Name() const = 0;
    virtual string getAbility2Name() const = 0;

    int getHealth() const;
    int getAttackPower() const;
    int getDefense() const;
    int getSpeed() const;
    int getAbilityUses() const;
    int getMaxAbilityUses() const;
    void setMaxAbilityUses(int amount);
    string getName() const;
    void setName(const string& newName);
    int getTrainingPoints() const;

    void increaseAttack(int amount);
    void increaseDefense(int amount);
    void increaseSpeed(int amount);
    void setAttackPower(int amount);
    void setDefense(int amount);
    void restoreAbilityUses(int amount);
    void setSpeed(int amount);

    Item** getInventory();
    void addItem(Item* item);

    void setPosition(int nx, int ny);
    void move(int dx, int dy, int arenaW, int arenaH, Arena* arena);
    int getX() const;
    int getY() const;
    int distanceTo(const Pet* other) const;
    void addExperience(int amount);
    void levelUp();
    void addTrainingPoints(int amount);
    bool unlockAbility(int abilityNumber, int cost);

    friend ostream& operator<<(ostream& os, const Pet& pet);
    bool operator==(const Pet& other) const;
    bool operator<(const Pet& other) const;


    int getEvolutionStage() const
    {
        return evolutionStage;
    }
    bool getCanEvolve() const
    {
        return canEvolve;
    }
    string getEvolutionName() const;
    virtual string getSpeciesName() const = 0;
};

class Dragon : public Pet
{
public:
    Dragon(const string& n);
    void attack(Pet* target) override;
    void useAbility(Pet* target, int abilityNumber) override;
    void takeDamage(int dmg) override;
    void fireballAttack(Pet* target);
    void dragonsRoar(Pet* target);
    bool evolve() override;
    string getAbility1Name() const override
    {
        return "Fireball Attack";
    }
    string getAbility2Name() const override
    {
        return "Dragon's Roar";
    }
   string getSpeciesName() const override
     {
         return "Dragon";
          }
};

class Phoenix : public Pet
{
public:
    Phoenix(const std::string& n);
    bool rebirthUsed = false;

    void attack(Pet* target) override;
    void useAbility(Pet* target, int abilityNumber) override;
    void flameHealing();
    void rebirth();
    void takeDamage(int dmg) override;
    bool evolve() override;
    string getAbility1Name() const override
    {
        return "Flame Healing";
    }
    string getAbility2Name() const override
    {
        return "Rebirth";
    }
    string getSpeciesName() const override
     {
         return "Phoenix"; 
         }
};

class Unicorn : public Pet
{
public:
    Unicorn(const string& n);
    bool shieldActive = false;
    int speedBurstTurns = 0;

    void attack(Pet* target) override;
    void useAbility(Pet* target, int abilityNumber) override;
    void speedBurst();
    void magicShield();
    void takeDamage(int dmg) override;
    bool evolve() override;
    string getAbility1Name() const override
    {
        return "Magic Shield";
    }
    string getAbility2Name() const override
    {
        return "Speed Burst";
    }
    std::string getSpeciesName() const override 
    {
         return "Unicorn";
         }
};
class Griffin : public Pet
{
public:
    Griffin(const string& n);
    bool diveBombStun = false;

    void attack(Pet* target) override;
    void useAbility(Pet* target, int abilityNumber) override;
    void takeDamage(int dmg) override;
    void clawStrike(Pet* target);
    void diveBomb(Pet* target);
    bool evolve() override;
    string getAbility1Name() const override
    {
        return "Claw Strike";
    }
    string getAbility2Name() const override
    {
        return "Dive Bomb";
    }
    std::string getSpeciesName() const override 
    {
         return "Griffin"; 
         }
};

#endif 