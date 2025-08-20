/*The aarena class holds all the types of battles it is like a stage to potray the battle,
it also display teh pets,their health, and battle logs*/
#include "Arena.h"
#include "Pet.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

Arena::Arena()
{
    logCount = 0;
    logCapacity = 100;
    currentBattleType = BATTLE_TYPE_1V1;
    activeEffectCount = 0;
    battleLog = new string[logCapacity];
    team1 = new BattleTeam(true);
    team2 = new BattleTeam(false);
    activeEffects = new ActiveItemEffect[MAX_ACTIVE_EFFECTS];

    for (int y = 0; y < ARENA_HEIGHT; ++y)
        for (int x = 0; x < ARENA_WIDTH; ++x)
            grid[y][x] = '.';
}

Arena::~Arena()
{
    delete[] battleLog;
    delete team1;
    delete team2;
    delete[] activeEffects;
}

void Arena::setupBattle(int type)
{
    currentBattleType = type;
    logCount = 0;
    delete team1;
    delete team2;
    team1 = new BattleTeam(true);
    team2 = new BattleTeam(false);
    for (int y = 0; y < ARENA_HEIGHT; ++y)
        for (int x = 0; x < ARENA_WIDTH; ++x)
            grid[y][x] = '.';
    placeObstacles();
}

void Arena::setupOneVsOne(Pet* playerPet, Pet* botPet)
{
    setupBattle(BATTLE_TYPE_1V1);
    playerPet->setPosition(0, 0);
    botPet->setPosition(ARENA_WIDTH - 1, ARENA_HEIGHT - 1);
    team1->addPet(playerPet);
    team2->addPet(botPet);
    updateLog("1v1 Battle started!");
}

void Arena::setupTwoVsTwo(Pet* playerPet1, Pet* playerPet2, Pet* botPet1, Pet* botPet2)
{
    setupBattle(BATTLE_TYPE_2V2);
    playerPet1->setPosition(0, 0);
    playerPet2->setPosition(0, ARENA_HEIGHT - 1);
    botPet1->setPosition(ARENA_WIDTH - 1, ARENA_HEIGHT - 1);
    botPet2->setPosition(ARENA_WIDTH - 1, 0);

    team1->addPet(playerPet1);
    team1->addPet(playerPet2);
    team2->addPet(botPet1);
    team2->addPet(botPet2);

    updateLog("2v2 Battle started!");
}

void Arena::setupGuildWar(Pet** playerPets, Pet** botPets)
{
    setupBattle(BATTLE_TYPE_GUILD_WAR);
    for (int i = 0; i < MAX_TEAM_SIZE; ++i)
    {
        playerPets[i]->setPosition(0, i);
        team1->addPet(playerPets[i]);
    }
    for (int i = 0; i < MAX_TEAM_SIZE; ++i)
    {
        botPets[i]->setPosition(ARENA_WIDTH - 1, i);
        team2->addPet(botPets[i]);
    }
    updateLog("Guild War has begun! Two guilds clash in an epic 5v5 battle!");
}

void Arena::showArena()
{
    clearScreen();
    showGrid();
    showLog();
}

void Arena::showPet(Pet* pet, bool isPlayer)
{
    if (pet == nullptr)
    {
        return;
    }

    string art;
    if (pet->getName() == "Dragon")
    {
        art = "  (o> <o)\n  /|_|\\\n   / \\";
    }
    else if (pet->getName() == "Phoenix")
    {
        art = "  (o> <o)\n  /|_|\\\n   / \\";
    }
    else if (pet->getName() == "Unicorn")
    {
        art = "  (o> <o)\n  /|_|\\\n   / \\";
    }
    else if (pet->getName() == "Griffin")
    {
        art = "  (o> <o)\n  /|_|\\\n   / \\";
    }

    int maxBar = 10;
    int barCount = (pet->getHealth() * maxBar) / 100;
    if (barCount < 0) barCount = 0;
    if (barCount > maxBar) barCount = maxBar;
    string healthBar(barCount, '#');
    healthBar += string(maxBar - barCount, ' ');
    string displayName = pet->getName();
    if (displayName.empty())
    {
        displayName = pet->getSpeciesName();
    }
    if (isPlayer)
    {
        cout << "Player's " << displayName << endl;
    }
    else
    {
        cout << "Bot's " << displayName << endl;
    }
    cout << "HP: [" << healthBar << "] " << pet->getHealth() << "/100" << endl;
    cout << art << endl;
}

void Arena::placeObstacles()
{
    srand(time(0));
    int wallCount = MAX_OBSTACLES / 2 + (MAX_OBSTACLES % 2);
    for (int i = 0; i < wallCount; ++i)
    {
        int x, y;
        do
        {
            x = rand() % ARENA_WIDTH;
            y = rand() % ARENA_HEIGHT;
        } while (grid[y][x] != '.' || (y == 0 && x == 0) || (y == ARENA_HEIGHT - 1 && x == ARENA_WIDTH - 1));
        grid[y][x] = '#';
    }
    int trapCount = MAX_OBSTACLES - wallCount;
    for (int i = 0; i < trapCount; ++i)
    {
        int x, y;
        do
        {
            x = rand() % ARENA_WIDTH;
            y = rand() % ARENA_HEIGHT;
        } while (grid[y][x] != '.' || (y == 0 && x == 0) || (y == ARENA_HEIGHT - 1 && x == ARENA_WIDTH - 1));
        grid[y][x] = '^';
    }
}

bool Arena::isWall(int y, int x) const
{
    return grid[y][x] == '#';
}

bool Arena::isTrap(int y, int x) const
{
    return grid[y][x] == '^';
}

void Arena::showGrid()
{
    char displayGrid[ARENA_HEIGHT][ARENA_WIDTH];
    for (int y = 0; y < ARENA_HEIGHT; ++y)
        for (int x = 0; x < ARENA_WIDTH; ++x)
            displayGrid[y][x] = grid[y][x];

    for (int i = 0; i < team1->getPetCount(); ++i)
    {
        Pet* pet = team1->getPet(i);
        if (pet) {
            int px = pet->getX();
            int py = pet->getY();
            if (displayGrid[py][px] == 'B')
            {
                displayGrid[py][px] = 'X'; /*if both player and bot are at the same position then
                                            X will be shown*/
            }
            else
            {
                displayGrid[py][px] = 'P';
            }
        }
    }
    for (int i = 0; i < team2->getPetCount(); ++i)
    {
        Pet* pet = team2->getPet(i);
        if (pet) {
            int bx = pet->getX();
            int by = pet->getY();
            if (displayGrid[by][bx] == 'P')
                displayGrid[by][bx] = 'X';
            else
                displayGrid[by][bx] = 'B';
        }
    }

    cout << "\n=== Battle Arena ===\n";
    for (int y = 0; y < ARENA_HEIGHT; ++y)
    {
        for (int x = 0; x < ARENA_WIDTH; ++x)
        {
            cout << displayGrid[y][x] << ' ';
        }
        cout << '\n';
    }

    cout << "\nTeam 1:\n";
    for (int i = 0; i < team1->getPetCount(); ++i)
    {
        showPet(team1->getPet(i), true);
    }

    cout << "\nTeam 2:\n";
    for (int i = 0; i < team2->getPetCount(); ++i)
    {
        showPet(team2->getPet(i), false);
    }
}

void Arena::updateLog(const string& logEntry)
{
    if (logCount < logCapacity)
    {
        battleLog[logCount++] = logEntry;
    }
}

void Arena::showLog() const
{
    cout << "\n=== Battle Log ===\n";
    for (int i = 0; i < logCount; ++i)
    {
        cout << battleLog[i] << "\n";
    }
}

void Arena::clearScreen() const
{
    system("cls");
}

void Arena::startBattle()
{
    int turnCount = 0;
    const int MAX_TURNS = 40;

    srand(time(0)); /*This sets random seed for power-up generation*/

    while (!isBattleOver() && turnCount < MAX_TURNS)
    {
        showArena();

        checkForPowerUp();

        for (int i = 0; i < team1->getPetCount(); ++i)
        {
            Pet* pet = team1->getPet(i);
            if (pet && pet->getHealth() > 0)
            {
                string displayName = pet->getName();
                if (displayName.empty()) displayName = pet->getSpeciesName();
                cout << "\nTeam 1's turn - Pet " << (i + 1) << " (" << displayName << ")" << endl;
                cout << "Choose action (1: Move, 2: Attack, 3: Ability, 4: Item): ";
                int action;
                while (true)
                {
                    cin >> action;
                    if (cin.fail() || action < 0 || action > 4) {
                        cout << "Invalid input! Please enter 0-4: ";
                        cin.clear();
                        cin.ignore(10000, '\n');
                    }
                    else {
                        cin.ignore(10000, '\n');
                        break;
                    }
                }
                if (action == 0)
                    return;
                if (action == 1)
                {
                    cout << "Move direction: 1) Up 2) Down 3) Left 4) Right" << endl;
                    int dir;
                    while (true)
                    {
                        cin >> dir;
                        if (cin.fail() || dir < 1 || dir > 4)
                        {
                            cout << "Invalid input! Please enter 1-4: ";
                            cin.clear();
                            cin.ignore(10000, '\n');
                        }
                        else
                        {
                            cin.ignore(10000, '\n');
                            break;
                        }
                    }
                    int dx = 0, dy = 0;
                    if (dir == 1)
                    {
                        dy = -1;
                    }
                    else if (dir == 2)
                    {
                        dy = 1;
                    }
                    else if (dir == 3)
                    {
                        dx = -1;
                    }
                    else if (dir == 4)
                    {
                        dx = 1;
                    }
                    pet->move(dx, dy, ARENA_WIDTH, ARENA_HEIGHT, this);
                    updateLog(pet->getName() + " moved.");
                }
                else if (action == 2)
                {
                    int validTargets[MAX_TEAM_SIZE], numTargets = 0;
                    for (int j = 0; j < team2->getPetCount(); ++j)
                    {
                        Pet* targetPet = team2->getPet(j);
                        if (targetPet && targetPet->getHealth() > 0)
                        {
                            validTargets[numTargets++] = j;
                        }
                    }
                    if (numTargets == 0) continue;
                    int targetIdx = validTargets[rand() % numTargets];
                    Pet* targetPet = team2->getPet(targetIdx);
                    pet->attack(targetPet);
                    updateLog(pet->getName() + " attacks " + targetPet->getName());
                }
                else if (action == 3)
                {
                    if (pet->getAbilityUses() <= 0)
                    {
                        cout << "You have used all your ability uses. Please select another action." << endl;
                        --i;
                        continue;
                    }
                    cout << "Choose ability:\n";
                    cout << "1. " << pet->getAbility1Name() << endl;
                    if (pet->ability2Unlocked)
                        cout << "2. " << pet->getAbility2Name() << endl;
                    int abilityChoice = 1;
                    while (true)
                    {
                        cout << "Enter ability number: ";
                        cin >> abilityChoice;
                        if (cin.fail() || (abilityChoice != 1 && abilityChoice != 2))
                        {
                            cout << "Invalid input! Please enter 1 or 2: ";
                            cin.clear();
                            cin.ignore(10000, '\n');
                            continue;
                        }
                        cin.ignore(10000, '\n');
                        if (abilityChoice == 2 && !pet->ability2Unlocked)
                        {
                            cout << "Ability 2 is locked! Please select another ability" << endl;
                            continue;
                        }
                        break;
                    }
                    int targetIdx = -1;
                    int tries = 0;
                    while (tries < 10)
                    {
                        int idx = rand() % team2->getPetCount();
                        Pet* targetPet = team2->getPet(idx);
                        if (targetPet && targetPet->getHealth() > 0)
                        {
                            targetIdx = idx;
                            break;
                        }
                        tries++;
                    }
                    if (targetIdx == -1)
                    {
                        continue;
                    }
                    Pet* abilityTarget = team2->getPet(targetIdx);

                    bool isPhoenixFlameHealing = (pet->getName() == "Phoenix" && abilityChoice == 1);
                    if (isPhoenixFlameHealing)
                    {
                        abilityTarget = pet;
                    }
                    pet->useAbility(abilityTarget, abilityChoice);
                    if (isPhoenixFlameHealing)
                    {
                        updateLog(pet->getName() + " used Flame Healing on itself");
                    }
                    else
                    {
                        string abilityName = (abilityChoice == 1) ? pet->getAbility1Name() : pet->getAbility2Name();
                        updateLog(pet->getName() + " uses " + abilityName + " on " + abilityTarget->getName());
                    }
                }
                else if (action == 4)
                {
                    Item** inv = pet->getInventory();
                    bool hasItem = false;
                    cout << "Inventory:" << endl;
                    for (int k = 0; k < INVENTORY_SIZE; ++k)
                    {
                        if (inv[k])
                        {
                            cout << k + 1 << ". " << inv[k]->getName() << endl;
                            hasItem = true;
                        }
                    }
                    if (hasItem == false)
                    {
                        cout << "Inventory is empty\n";

                        --i;
                        continue;
                    }
                    cout << "Select item index: ";
                    int idx;
                    while (true)
                    {
                        cin >> idx;
                        if (cin.fail() || idx < 1 || idx > INVENTORY_SIZE || !inv[idx - 1])
                        {
                            cout << "Invalid input! Please select a valid item index: ";
                            cin.clear();
                            cin.ignore(10000, '\n');
                        }
                        else
                        {
                            cin.ignore(10000, '\n');
                            break;
                        }
                    }
                    inv[idx - 1]->use(pet);
                    addItemEffect(inv[idx - 1], pet);
                    updateLog(pet->getName() + " used " + inv[idx - 1]->getName());
                    delete inv[idx - 1];
                    inv[idx - 1] = nullptr;
                }
                if (isBattleOver()) break;
            }
        }
        if (isBattleOver()) break;

        for (int i = 0; i < team2->getPetCount(); ++i)
        {
            Pet* pet = team2->getPet(i);
            if (pet && pet->getHealth() > 0)
            {
                int action = rand() % 4 + 1;
                if (action == 0) return;
                if (action == 1)
                {
                    /* Move randomly*/
                    int dir = rand() % 4;
                    int dx = 0, dy = 0;
                    if (dir == 0) dy = -1;
                    else if (dir == 1) dy = 1;
                    else if (dir == 2) dx = -1;
                    else if (dir == 3) dx = 1;
                    pet->move(dx, dy, ARENA_WIDTH, ARENA_HEIGHT, this);
                    updateLog("Bot moved.");
                }
                else if (action == 2)
                {
                    int validTargets[MAX_TEAM_SIZE], numTargets = 0;
                    for (int j = 0; j < team1->getPetCount(); ++j)
                    {
                        Pet* targetPet = team1->getPet(j);
                        if (targetPet && targetPet->getHealth() > 0)
                        {
                            validTargets[numTargets++] = j;
                        }
                    }
                    if (numTargets == 0)
                    {
                        continue;
                    }
                    int targetIdx = validTargets[rand() % numTargets];
                    Pet* targetPet = team1->getPet(targetIdx);
                    pet->attack(targetPet);
                    updateLog("Team 2's " + pet->getName() + " attacks " + targetPet->getName());
                }
                else if (action == 3)
                {
                    if (pet->getAbilityUses() <= 0)
                    {
                        continue;
                    }
                    int abilityChoice = 1;
                    if (pet->ability2Unlocked)
                    {
                        abilityChoice = (rand() % 2) + 1;
                    }
                    int validTarget = -1;
                    int tries = 0;
                    while (tries < 10)
                    {
                        int idx = rand() % team1->getPetCount();
                        Pet* targetPet = team1->getPet(idx);
                        if (targetPet && targetPet->getHealth() > 0)
                        {
                            validTarget = idx;
                            break;
                        }
                        tries++;
                    }
                    if (validTarget == -1)
                    {
                        continue;
                    }

                    Pet* abilityTarget = team1->getPet(validTarget);
                    bool isPhoenixFlameHealing = (pet->getName() == "Phoenix" && abilityChoice == 1);
                    if (isPhoenixFlameHealing)
                    {
                        abilityTarget = pet;
                    }
                    pet->useAbility(abilityTarget, abilityChoice);
                    if (isPhoenixFlameHealing)
                    {
                        updateLog(pet->getName() + " used Flame Healing on itself");
                    }
                    else
                    {
                        string abilityName = (abilityChoice == 1) ? pet->getAbility1Name() : pet->getAbility2Name();
                        updateLog("Team 2's " + pet->getName() + " uses " + abilityName + " on " + abilityTarget->getName());
                    }
                }
                else if (action == 4)
                {
                    Item** inv = pet->getInventory();
                    for (int k = 0; k < INVENTORY_SIZE; ++k)
                    {
                        if (inv[k])
                        {
                            inv[k]->use(pet);
                            addItemEffect(inv[k], pet);
                            updateLog("Team 2's " + pet->getName() + " used " + inv[k]->getName());
                            delete inv[k];
                            inv[k] = nullptr;
                            break;
                        }
                    }
                }
                if (isBattleOver())
                {
                    break;
                }
            }
        }
        if (isBattleOver())
        {
            break;
        }
        updatePowerUps();
        updateItemEffects();

        if (isBattleOver())
        {
            break;
        }
        turnCount++;
    }

    showArena();
    if (isBattleOver())
    {
        BattleTeam* winner = getWinningTeam();
        if (winner)
        {
            BattleTeam* loser = (winner == team1) ? team2 : team1;
            for (int i = 0; i < winner->getPetCount(); ++i)
            {
                if (winner->getPet(i)) {
                    winner->getPet(i)->addExperience(50);
                }
            }
            for (int i = 0; i < loser->getPetCount(); ++i)
            {
                if (loser->getPet(i)) {
                    loser->getPet(i)->addExperience(10);
                }
            }
            updateLog((winner->isPlayerTeam ? "Team 1" : "Team 2") + string(" wins the battle!"));
        }
        else
        {
            updateLog("The battle ends in a draw!");
        }
    }
    else if (turnCount >= MAX_TURNS)
    {
        updateLog("The battle ends in a draw due to turn limit!");
        cout << "The battle ends in a draw due to turn limit!" << endl;
    }
}

void Arena::checkForPowerUp()
{
    if (rand() % 100 < POWER_UP_CHANCE)
    {
        PowerUp* powerUp = generateRandomPowerUp();

        if (rand() % 2 == 0)
        {
            if (team1->getCurrentPowerUp())
            {
                delete team1->getCurrentPowerUp();
            }
            team1->setCurrentPowerUp(powerUp);
            applyPowerUp(team1);
            updateLog("Team 1 found a power-up!");
        }
        else
        {
            if (team2->getCurrentPowerUp())
            {
                delete team2->getCurrentPowerUp();
            }
            team2->setCurrentPowerUp(powerUp);
            applyPowerUp(team2);
            updateLog("Team 2 found a power-up!");
        }
    }
}

void Arena::updatePowerUps()
{
    if (team1->getCurrentPowerUp() && team1->getCurrentPowerUp()->isActive())
    {
        team1->getCurrentPowerUp()->decrementDuration();
        if (team1->getCurrentPowerUp()->getDuration() <= 0)
        {
            /* removing  power-up effect*/
            for (int i = 0; i < team1->getPetCount(); ++i)
            {
                Pet* pet = team1->getPet(i);
                if (pet && pet->getHealth() > 0)
                {
                    switch (team1->getCurrentPowerUp()->getType())
                    {
                    case POWER_UP_ATTACK:
                        pet->increaseAttack(-team1->getCurrentPowerUp()->getValue());
                        break;
                    case POWER_UP_DEFENSE:
                        pet->increaseDefense(-team1->getCurrentPowerUp()->getValue());
                        break;
                    case POWER_UP_SPEED:
                        pet->increaseSpeed(-team1->getCurrentPowerUp()->getValue());
                        break;
                        /* No need to undo health boosts*/
                    }
                }
            }
            delete team1->getCurrentPowerUp();
            team1->setCurrentPowerUp(nullptr);
            updateLog("Team 1's power-up has worn off.");
        }
    }
    if (team2->getCurrentPowerUp() && team2->getCurrentPowerUp()->isActive())
    {
        team2->getCurrentPowerUp()->decrementDuration();
        if (team2->getCurrentPowerUp()->getDuration() <= 0)
        {
            for (int i = 0; i < team2->getPetCount(); ++i)
            {
                Pet* pet = team2->getPet(i);
                if (pet && pet->getHealth() > 0)
                {
                    switch (team2->getCurrentPowerUp()->getType())
                    {
                    case POWER_UP_ATTACK:
                        pet->increaseAttack(-team2->getCurrentPowerUp()->getValue());
                        break;
                    case POWER_UP_DEFENSE:
                        pet->increaseDefense(-team2->getCurrentPowerUp()->getValue());
                        break;
                    case POWER_UP_SPEED:
                        pet->increaseSpeed(-team2->getCurrentPowerUp()->getValue());
                        break;
                    }
                }
            }
            delete team2->getCurrentPowerUp();
            team2->setCurrentPowerUp(nullptr);
            updateLog("Team 2's power-up has worn off.");
        }
    }
}

PowerUp* Arena::generateRandomPowerUp()
{
    static const struct
    {
        int type;
        int value;
        int duration;
    }
    powerUpConfigs[MAX_POWER_UP_TYPES] = {
        {POWER_UP_HEALTH, 20, 3},
        {POWER_UP_ATTACK, 10, 3},
        {POWER_UP_DEFENSE, 10, 3},
        {POWER_UP_SPEED, 5, 3}
    };

    int typeIndex = rand() % MAX_POWER_UP_TYPES;
    return new PowerUp(powerUpConfigs[typeIndex].type, powerUpConfigs[typeIndex].value, powerUpConfigs[typeIndex].duration);
}

void Arena::applyPowerUp(BattleTeam* team)
{
    if (!team->getCurrentPowerUp()) return;

    PowerUp* powerUp = team->getCurrentPowerUp();
    for (int i = 0; i < team->getPetCount(); ++i)
    {
        Pet* pet = team->getPet(i);
        if (pet && pet->getHealth() > 0)
        {
            switch (powerUp->getType())
            {
            case POWER_UP_HEALTH:
                pet->heal(powerUp->getValue());
                break;
            case POWER_UP_ATTACK:
                pet->increaseAttack(powerUp->getValue());
                break;
            case POWER_UP_DEFENSE:
                pet->increaseDefense(powerUp->getValue());
                break;
            case POWER_UP_SPEED:
                pet->increaseSpeed(powerUp->getValue());
                break;
            }
        }
    }
    powerUp->setActive(true);
    updateLog(string("Applied power-up to ") + (team->isPlayerTeam ? "Team 1" : "Team 2"));
}

void Arena::addItemEffect(Item* item, Pet* target)
{
    if (item->getDuration() > 0 && activeEffectCount < MAX_ACTIVE_EFFECTS)
    {
        activeEffects[activeEffectCount++] = ActiveItemEffect(item, target, item->getDuration());
    }
}

void Arena::updateItemEffects()
{
    for (int i = 0; i < activeEffectCount; ++i)
    {
        activeEffects[i].decrementTurns();
        if (activeEffects[i].getRemainingTurns() <= 0)
        {
            activeEffects[i].getItem()->removeEffect(activeEffects[i].getTarget());
        }
    }
    removeExpiredEffects();
}

void Arena::removeExpiredEffects()
{
    int writeIndex = 0;
    for (int i = 0; i < activeEffectCount; ++i)
    {
        if (activeEffects[i].getRemainingTurns() > 0)
        {
            if (writeIndex != i)
            {
                activeEffects[writeIndex] = activeEffects[i];
            }
            writeIndex++;
        }
    }
    activeEffectCount = writeIndex;
}

void Arena::processTurn(int teamIndex, int petIndex, int action, int targetIndex)
{
    BattleTeam* currentTeam = (teamIndex == 0) ? team1 : team2;
    BattleTeam* opponentTeam = (teamIndex == 0) ? team2 : team1;
    Pet* currentPet = currentTeam->getPet(petIndex);
    if (currentPet == nullptr || currentPet->getHealth() <= 0)
    {
        return;
    }
    Pet* target = nullptr;
    if (targetIndex >= 0 && targetIndex < opponentTeam->getPetCount())
    {
        target = opponentTeam->getPet(targetIndex);
        if (!target || target->getHealth() <= 0) return;
    }
    else
    {
        for (int i = 0; i < opponentTeam->getPetCount(); ++i)
        {
            if (opponentTeam->getPet(i)->getHealth() > 0)
            {
                target = opponentTeam->getPet(i);
                break;
            }
        }
        if (target == nullptr) return;
    }
    if (currentPet->stunned)
    {
        cout << currentPet->getName() << " is stunned and skips the turn!" << endl;
        currentPet->stunned = false;
        return;
    }
    if (currentPet->getName() == "Unicorn")
    {

        if (currentPet->hasShield && currentPet->shieldActive)
        {
            cout << currentPet->getName() << " is protected by Magic Shield!" << endl;
            currentPet->shieldActive = false;
        }

        if (currentPet->hasSpeedBurst && currentPet->speedBurstTurns > 0)
        {
            currentPet->speedBurstTurns--;
            if (currentPet->speedBurstTurns == 0)
            {
                cout << currentPet->getName() << "'s Speed Burst wore off!" << endl;
                currentPet->increaseSpeed(-5);
            }
        }
    }
    else if (currentPet->getName() == "Phoenix")
    {
        if (currentPet->getHealth() <= 0 && currentPet->rebirthUsed == false && currentPet->hasRebirth)
        {
            currentPet->rebirth();
            if (currentPet->getHealth() > 0)
            {
                cout << currentPet->getName() << " is reborn and continues the fight!" << endl;
            }
        }
    }

    switch (action)
    {
    case 1:
        currentPet->attack(target);
        updateLog(currentPet->getName() + " attacks " + target->getName());
        break;
    case 2:
    {
        int abilityNumber = 1;
        if (!currentTeam->isPlayerTeam && currentPet->ability2Unlocked)
        {
            abilityNumber = (rand() % 2) + 1;
        }
        currentPet->useAbility(target, abilityNumber);
        updateLog(currentPet->getName() + " uses ability on " + target->getName());
        break;
    }
    case 3:
        if (currentTeam->isPlayerTeam)
        {

            cout << "\nSelect item to use:\n";
            for (int i = 0; i < INVENTORY_SIZE; ++i)
            {
                if (currentTeam->getPet(petIndex)->getInventory()[i])
                {
                    cout << i + 1 << ". " << currentTeam->getPet(petIndex)->getInventory()[i]->getName() << "\n";
                }
            }
            int itemChoice;
            while (true) {
                cin >> itemChoice;
                if (cin.fail() || itemChoice < 1 || itemChoice > INVENTORY_SIZE) 
                {
                    cout << "Invalid input! Please enter a valid item number: ";
                    cin.clear();
                    cin.ignore(10000, '\n');
                }
                else
                 {
                    cin.ignore(10000, '\n');
                    break;
                }
            }

            if (itemChoice > 0 && itemChoice <= INVENTORY_SIZE)
            {
                Item* item = currentTeam->getPet(petIndex)->getInventory()[itemChoice - 1];
                if (item)
                {
                    item->use(currentPet);
                    addItemEffect(item, currentPet);
                    updateLog(currentPet->getName() + " used " + item->getName());
                }
            }
        }
        else
        {

            for (int i = 0; i < INVENTORY_SIZE; ++i)
            {
                Item* item = currentTeam->getPet(petIndex)->getInventory()[i];
                if (item)
                 {
                    item->use(currentPet);
                    addItemEffect(item, currentPet);
                    updateLog(currentPet->getName() + " used " + item->getName());
                    break;
                }
            }
        }
        break;
    }
    updateItemEffects();
}

bool Arena::isBattleOver() const
{
    bool team1Defeated = true;
    for (int i = 0; i < team1->getPetCount(); ++i)
    {
        if (team1->getPet(i)->getHealth() > 0)
        {
            team1Defeated = false;
            break;
        }
    }
    bool team2Defeated = true;
    for (int i = 0; i < team2->getPetCount(); ++i)
    {
        if (team2->getPet(i)->getHealth() > 0)
        {
            team2Defeated = false;
            break;
        }
    }
    return team1Defeated || team2Defeated;
}

BattleTeam* Arena::getWinningTeam() const
{
    if (isBattleOver() == false)
    {
        return nullptr;
    }

    bool team1Alive = false;
    for (int i = 0; i < team1->getPetCount(); ++i)
    {
        if (team1->getPet(i)->getHealth() > 0)
        {
            team1Alive = true;
            break;
        }
    }
    bool team2Alive = false;
    for (int i = 0; i < team2->getPetCount(); ++i)
    {
        if (team2->getPet(i)->getHealth() > 0)
        {
            team2Alive = true;
            break;
        }
    }

    if (team1Alive == true && team2Alive == false)
    {
        return team1;
    }
    if (team1Alive == false && team2Alive == true)
    {
        return team2;
    }
    return nullptr; /*draw*/
}