//Syeda afnan hussain , SE-A
/*The purpose of this class is to manage all the pets, their actions, abilities, healing, damage
all the stats, evolution, movement, distance to other pets, level ups and experiences*/
#include "Pet.h"
#include "Arena.h"
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

Pet::Pet(const string& n, int hp, int atk, int def, int spd, int abilityUses)
{
    name = n;
    health = hp;
    maxHealth = hp;
    attackPower = atk;
    defense = def;
    speed = spd;
    this->abilityUses = abilityUses;
    maxAbilityUses = abilityUses;
    x = 0;
    y = 0;
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        inventory[i] = nullptr;
    }
}

Pet::~Pet()
{
    for (int i = 0; i < INVENTORY_SIZE; ++i)
    {
        delete inventory[i];
    }
}

void Pet::takeDamage(int dmg)
{
    int actualDamage = dmg - defense;
    if (actualDamage < 1)
    {
        actualDamage = 1;
    }
    health -= actualDamage;
    if (health < 0)
    {
        health = 0;
    }
}

void Pet::heal(int amount)
{
    health += amount;
    if (health > maxHealth)
    {
        health = maxHealth;
    }
}

int Pet::getHealth() const
{
    return health;
}
int Pet::getAttackPower() const
{
    return attackPower;
}
int Pet::getDefense() const
{
    return defense;
}
int Pet::getSpeed() const
{
    return speed;
}
int Pet::getAbilityUses() const
{
    return abilityUses;
}
int Pet::getMaxAbilityUses() const
{
    return maxAbilityUses;
}
void Pet::setMaxAbilityUses(int amount)
{
    maxAbilityUses = amount;
}
std::string Pet::getName() const
{
    return name;
}

void Pet::setPosition(int nx, int ny)
{
    x = nx;
    y = ny;
}

void Pet::move(int dx, int dy, int arenaW, int arenaH, Arena* arena)
{
    int newX = x + dx;
    int newY = y + dy;
    if (newX < 0)
    {
        newX = 0;
    }
    if (newX >= arenaW)
    {
        newX = arenaW - 1;
    }
    if (newY < 0)
    {
        newY = 0;
    }
    if (newY >= arenaH)
    {
        newY = arenaH - 1;
    }
    /*Checking for obstacles*/
    if (arena && arena->isWall(newY, newX))
    {
        cout << "Blocked by wall!" << endl;
        return;
    }
    if (arena && arena->isTrap(newY, newX))
    {
        cout << "Stepped on a trap!" << endl;
        takeDamage(10);
    }
    /*Updating positions*/
    x = newX;
    y = newY;
}
int Pet::getX() const
{
    return x;
}
int Pet::getY() const
{
    return y;
}
int Pet::distanceTo(const Pet* other) const
{
    int dx = this->x - other->x;
    int dy = this->y - other->y;
    if (dx < 0)
    {
        dx = -dx;
    }
    if (dy < 0)
    {
        dy = -dy;
    }
    return dx + dy;
}

int Pet::getTrainingPoints() const
{
    return trainingPoints;
}
void Pet::increaseAttack(int amount)
{
    attackPower += amount;
}

void Pet::increaseDefense(int amount)
{
    defense += amount;
}

void Pet::increaseSpeed(int amount)
{
    speed += amount;
}

void Pet::setName(const std::string& newName)
{
    name = newName;
}

void Pet::setAttackPower(int amount)
{
    attackPower = amount;
}
void Pet::setDefense(int amount)
{
    defense = amount;
}
void Pet::setSpeed(int amount)
{
    speed = amount;
}

void Pet::restoreAbilityUses(int amount)
{
    abilityUses += amount;
    if (abilityUses > maxAbilityUses)
    {
        abilityUses = maxAbilityUses;
    }
}

Item** Pet::getInventory()
{
    return inventory;
}

void Pet::addItem(Item* item)
{
    for (int i = 0; i < INVENTORY_SIZE; ++i)
    {
        if (inventory[i] == nullptr)
        {
            inventory[i] = item;
            break;
        }
    }
}

void Pet::addExperience(int amount)
{
    experience += amount;
    while (experience >= experienceToNextLevel)
    {
        experience -= experienceToNextLevel;
        levelUp();
    }
}
const int Pet::EVOLUTION_LEVELS[3] = { 10, 20, 30 };

void Pet::levelUp()
{
    level++;
    maxHealth += 10;
    attackPower += 5;
    defense += 2;
    speed += 4;
    health = maxHealth;
    experienceToNextLevel += 50; /*This ncreases threshold for next level*/

    for (int i = 0; i < 3; i++)
    {
        if (level == EVOLUTION_LEVELS[i] && evolutionStage == i)
        {
            canEvolve = true;
            cout << endl << name << " can now evolve!" << endl;
            break;
        }
    }
    cout << name << " leveled up to " << level << endl;
}

void Pet::addTrainingPoints(int amount)
{
    trainingPoints += amount;
}

bool Pet::unlockAbility(int abilityNumber, int cost)
{
    if (trainingPoints < cost)
    {
        return false;
    }
    trainingPoints -= cost;
    if (abilityNumber == 2)
    {
        ability2Unlocked = true;
    }
    /*I'll add more abilities later*/
    return true;
}

std::ostream& operator<<(ostream& COUT, const Pet& pet)
{
    COUT << "Pet: " << pet.name
        << " | HP: " << pet.health << "/" << pet.maxHealth
        << " | ATK: " << pet.attackPower
        << " | DEF: " << pet.defense
        << " | SPD: " << pet.speed
        << " | Level: " << pet.level;
    return COUT;
}

bool Pet::operator==(const Pet& other) const
{
    return (this->name == other.name);
}

bool Pet::operator<(const Pet& other) const
{
    return (this->name < other.name);
}

bool Pet::evolve()
{
    if (canEvolve == false)
    {
        return false;
    }
    evolutionStage++;
    canEvolve = false;
    maxHealth += 50;
    attackPower += 25;
    defense += 15;
    speed += 20;
    health = maxHealth;

    cout << endl << name << " evolved to " << getEvolutionName() << "!" << endl;
    cout << "Stats increased significantly!" << endl;
    return true;
}

string Pet::getEvolutionName() const
{
    return name;
}

Dragon::Dragon(const string& n) : Pet(n, 100, 20, 10, 5, 3)
{

}

void Dragon::attack(Pet* target)
{
    if (target && stunned == false)
    {
        target->takeDamage(attackPower);
        cout << name << " attacks " << target->getName() << " for " << attackPower << " damage!" << endl;
    }
}

void Dragon::fireballAttack(Pet* target)
{
    cout << name << " uses Fireball Attack!" << endl;
    target->takeDamage(attackPower * 2);
}

void Dragon::dragonsRoar(Pet* target)
{
    cout << name << " uses Dragon's Roar! " << target->getName() << " is stunned!" << endl;
    target->stunned = true;
}

void Dragon::useAbility(Pet* target, int abilityNumber)
{
    if (abilityUses > 0 && stunned == false)
    {
        if (abilityNumber == 1)
        {
            fireballAttack(target);
        }
        else if (abilityNumber == 2)
        {
            dragonsRoar(target);
        }
        else
        {
            cout << "Invalid ability number!\n";
            return;
        }
        abilityUses--;
    }
    else
    {
        cout << "No ability uses remaining or stunned!" << endl;
    }
}

void Dragon::takeDamage(int dmg)
{
    /*I am shhowing that dragons have natural resistance to damage*/
    int reducedDamage = static_cast<int>(dmg * 0.8); // 20% damage reduction
    Pet::takeDamage(reducedDamage);
}
bool Dragon::evolve()
{
    if (!Pet::evolve()) return false;

    switch (evolutionStage)
    {
    case 1:
        name = "Draacus";
        cout << "Dragon evolved into a Draacus!" << endl;
        break;
    case 2:
        name = "Ancient Giant";
        cout << "Draacus evolved into an Ancient Giant!" << endl;
        break;
    case 3:
        name = "Lord of the lords";
        cout << "Ancient Giant evolved into a Lord of the lords!" << endl;
        break;
    }
    return true;
}

Phoenix::Phoenix(const string& n) : Pet(n, 100, 70, 50, 80, 3)
{
    hasRebirth = true;
    rebirthUsed = false;
}

void Phoenix::attack(Pet* target)
{
    if (target && stunned == false)
    {
        target->takeDamage(attackPower);
        cout << name << " attacks " << target->getName() << " for " << attackPower << " damage!" << endl;
    }
}

void Phoenix::useAbility(Pet* target, int abilityNumber)
{
    if (abilityUses > 0 && stunned == false)
    {
        if (abilityNumber == 1)
        {
            flameHealing();
        }
        else if (abilityNumber == 2 && rebirthUsed == false)
        {
            rebirth();
        }
        else
        {
            cout << "Invalid ability number or rebirth already used!" << endl;
            return;
        }
        abilityUses--;
    }
    else
    {
        cout << "No ability uses remaining or stunned!" << endl;
    }
}

void Phoenix::flameHealing()
{
    int healAmount = attackPower;
    heal(healAmount);
    cout << name << " uses Flame Healing and heals itself for " << healAmount << " health!" << endl;
}

void Phoenix::rebirth()
{
    if (rebirthUsed == false)
    {
        health = maxHealth;
        rebirthUsed = true;
        cout << name << " uses Rebirth and is fully healed!" << endl;
    }
}

void Phoenix::takeDamage(int dmg)
{
    if (health <= dmg && rebirthUsed == false)
    {
        rebirth();
    }
    else
    {
        Pet::takeDamage(dmg);
    }
}
bool Phoenix::evolve()
{
    if (Pet::evolve() == false)
    {
        return false;
    }

    switch (evolutionStage)
    {
    case 1:
        name = "Flame Phoenix";
        cout << "Phoenix evolved into a Flame Phoenix!" << endl;
        break;
    case 2:
        name = "Solar Phoenix";
        cout << "Flame Phoenix evolved into a Solar Phoenix!" << endl;
        break;
    case 3:
        name = "Divine Phoenix";
        cout << "Solar Phoenix evolved into an Divine Phoenix!" << endl;
        break;
    }

    return true;
}

Unicorn::Unicorn(const string& n) : Pet(n, 100, 60, 80, 90, 3)
{
    hasShield = true;
    shieldActive = false;
    hasSpeedBurst = true;
    speedBurstTurns = 0;
}

void Unicorn::attack(Pet* target)
{
    target->takeDamage(attackPower);
}

void Unicorn::speedBurst()
{
    cout << name << " uses Speed Burst!" << endl;
    speed += 5;
    speedBurstTurns = 3;
}

void Unicorn::magicShield()
{
    cout << name << " uses Magic Shield!" << endl;
    shieldActive = true;
}

void Unicorn::useAbility(Pet* target, int abilityNumber)
{
    if (abilityUses > 0 && stunned == false)
    {
        if (abilityNumber == 1)
        {
            speedBurst();
        }
        else if (abilityNumber == 2)
        {
            magicShield();
        }
        else
        {
            cout << "Invalid ability number!\n";
            return;
        }
        abilityUses--;
    }
    else {
        cout << "No ability uses remaining or stunned!" << endl;
    }
}

void Unicorn::takeDamage(int dmg)
{
    if (shieldActive)
    {
        cout << name << "'s Magic Shield absorbs the damage!" << endl;
        shieldActive = false;
        return;
    }
    Pet::takeDamage(dmg);
}

bool Unicorn::evolve()
{
    if (Pet::evolve() == false)
    {
        return false;
    }

    switch (evolutionStage)
    {
    case 1:
        name = "Crystal Unicorn";
        cout << "Unicorn evolved into a Crystal Unicorn!" << endl;
        break;
    case 2:
        name = "Starlith Unicorn";
        cout << "Crystal Unicorn evolved into a Starlith Unicorn!" << endl;
        break;
    case 3:
        name = "Mythical Unicorn";
        cout << "Starlith Unicorn evolved into a Mythical Unicorn!" << endl;
        break;
    }
    return true;
}

Griffin::Griffin(const string& n) : Pet(n, 100, 18, 7, 10, 3)
{

}

void Griffin::attack(Pet* target)
{
    if (target && stunned == false)
    {
        target->takeDamage(attackPower);
        cout << name << " attacks " << target->getName() << " for " << attackPower << " damage!" << endl;
    }
}

void Griffin::clawStrike(Pet* target)
{
    cout << name << " uses Claw Strike!" << endl;
    target->takeDamage(attackPower * 2);
}

void Griffin::diveBomb(Pet* target)
{
    cout << name << " uses Dive Bomb! " << target->getName() << " is stunned!" << endl;
    target->stunned = true;
}

void Griffin::useAbility(Pet* target, int abilityNumber)
{
    if (abilityUses > 0 && stunned == false)
    {
        if (abilityNumber == 1)
        {
            clawStrike(target);
        }
        else if (abilityNumber == 2)
        {
            diveBomb(target);
        }
        else
        {
            cout << "Invalid ability number!\n";
            return;
        }
        abilityUses--;
    }
    else
    {
        cout << "No ability uses remaining or stunned!" << endl;
    }
}

void Griffin::takeDamage(int dmg)
{
    /*I have included a twist,griffins have a 20% chance to dodge attacks*/
    if (rand() % 100 < 20)
    {
        cout << name << " dodged the attack!" << endl;
        return;
    }
    Pet::takeDamage(dmg);
}

bool Griffin::evolve()
{
    if (Pet::evolve() == false)
    {
        return false;
    }

    switch (evolutionStage)
    {
    case 1:
        name = "Thunder Griffin";
        cout << "Griffin evolved into a Thunder Griffin!" << endl;
        break;
    case 2:
        name = "Storm Griffin";
        cout << "Thunder Griffin evolved into a Storm Griffin!" << endl;
        break;
    case 3:
        name = "Legendary Griffin";
        cout << "Storm Griffin evolved into a Legendary Griffin!" << endl;
        break;
    }

    return true;
}
