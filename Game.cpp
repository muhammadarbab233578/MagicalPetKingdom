//Syeda afnan hussain, , SE-A
/*The purpose of this class is, well basically this iss the class where all the function calls and everthing is happening
main calls the game class and it handles everything after that*/
#include "Game.h"
#include "Battle.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Game::Game()
{
    availablePets[0] = new Dragon("Dragon");
    availablePets[1] = new Phoenix("Phoenix");
    availablePets[2] = new Unicorn("Unicorn");
    availablePets[3] = new Griffin("Griffin");
    shopItems[0] = new HealingPotion();
    shopItems[1] = new ManaPotion();
    shopItems[2] = new BuffItem();
    shopItems[3] = new Shield();
    player = new Player("Player");
    bot = new Bot("Bot");
    arena = new Arena();
    guildManager = new GuildManager();
    trainingCamp = new TrainingCamp("Main Camp");
    shop = new Shop();
    leaderboard = new Leaderboard();
}
Game::~Game()
{
    for (int i = 0; i < 4; ++i)
    {
        delete availablePets[i];
        delete shopItems[i];
    }
    delete player;
    delete bot;
    delete arena;
    delete guildManager;
    delete trainingCamp;
    delete shop;
    delete leaderboard;
}
void Game::showShop()
{
    cout << "Shop Items (prices in rupees):" << endl;
    for (int i = 0; i < 4; ++i)
    {
        cout << i + 1 << ". " << shopItems[i]->getName() << " - " << shopItems[i]->getPrice() << " rupees" << endl;
    }
    cout << "Enter item number to buy or 0 to exit: ";
    int choice;
    while (true)
    {
        cin >> choice;
        if (cin.fail())
        {
            cout << "Invalid input! Please enter a valid number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        else
        {
            cin.ignore(10000, '\n');
            break;
        }
    }
    if (choice >= 1 && choice <= 4)
    {
        int price = shopItems[choice - 1]->getPrice();
        if (player->getCurrency() >= price)
        {
            Item* newItem = nullptr;
            if (shopItems[choice - 1]->getName() == "Healing Potion")
            {
                newItem = new HealingPotion();
            }
            else if (shopItems[choice - 1]->getName() == "Mana Potion")
            {
                newItem = new ManaPotion();
            }
            else if (shopItems[choice - 1]->getName() == "Buff Item")
            {
                newItem = new BuffItem();
            }
            else if (shopItems[choice - 1]->getName() == "Shield")
            {
                newItem = new Shield();
            }
            if (newItem)
            {
                player->addItem(newItem);
                player->earnCurrency(-price);
                cout << "Item bought for " << price << " rupees!" << endl;
            }
        }
        else
        {
            cout << "Not enough rupees!\n";
        }
    }
    else if (choice != 0)
    {
        cout << "Invalid choice!\n";
    }
}

void Game::saveProgress()
{
    // Check if player has a pet selected
    if (player->getPet() == nullptr) {
        cout << "\nYou need to select a pet before saving your progress!" << endl;
        cout << "Press Enter to continue...";
        cin.clear();
        cin.ignore(10000, '\n');
        cin.get();
        return;
    }

    ofstream fout("save.txt");

    // Save player information
    fout << player->getName() << endl;
    fout << player->getCurrency() << endl;

    // Save player's selected pet index
    int selectedPetIndex = -1;
    for (int i = 0; i < 4; ++i) {
        if (player->getPet() == availablePets[i]) {
            selectedPetIndex = i;
            break;
        }
    }
    fout << selectedPetIndex << endl;

    // Save all available pets
    for (int i = 0; i < 4; ++i) {
        Pet* pet = availablePets[i];
        fout << pet->getName() << " "
            << pet->getHealth() << " "
            << pet->getAttackPower() << " "
            << pet->getDefense() << " "
            << pet->getSpeed() << " "
            << pet->getAbilityUses() << " "
            << pet->getMaxAbilityUses() << " "
            << pet->level << " "
            << pet->experience << " "
            << pet->experienceToNextLevel << " "
            << pet->trainingPoints << " "
            << pet->ability1Unlocked << " "
            << pet->ability2Unlocked << " "
            << pet->getEvolutionStage() << " "  // Add evolution stage
            << pet->getCanEvolve() << endl;     // Add can evolve status
    }

    // Save inventory - write "EMPTY" for all slots if no pet exists or inventory is empty
    if (player->getPet() != nullptr) {
        for (int i = 0; i < INVENTORY_SIZE; ++i) {
            Item* item = player->getPet()->getInventory()[i];
            if (item) fout << item->getName() << " ";
            else fout << "EMPTY ";
        }
    }
    else {
        for (int i = 0; i < INVENTORY_SIZE; ++i) {
            fout << "EMPTY ";
        }
    }
    fout << endl;

    // Save guilds
    fout << "GUILDS" << endl;
    for (int i = 0; i < guildManager->getGuildCount(); ++i) {
        Guild* guild = guildManager->getGuild(i);
        fout << guild->getName() << " "
            << guild->getWins() << " "
            << guild->getLosses() << " "
            << guild->getRanking() << endl;

        for (int j = 0; j < guild->getPetCount(); ++j) {
            Pet* pet = guild->getPet(j);
            fout << pet->getName() << " " << guild->getRole(j) << endl;
        }
        fout << "END_GUILD" << endl;
    }
    fout << "END_GUILDS" << endl;

    // Save leaderboard
    fout << "LEADERBOARD" << endl;
    // Add leaderboard saving code here if needed
    fout << "END_LEADERBOARD" << endl;

    fout.close();

    cout << "\nProgress saved successfully!" << endl;
    cout << "Press Enter to continue...";
    cin.clear();
    cin.ignore(10000, '\n');
    cin.get();
}
void Game::loadProgress()
{
    ifstream fin("save.txt");
    if (!fin) {
        cout << "\nNo save file found.\n";
        cout << "Press Enter to return to the main menu...";
        cin.clear();
        cin.ignore(10000, '\n');
        cin.get();
        return;
    }
    string pname;
    int cur;
    fin >> pname >> cur;
    delete player;
    player = new Player(pname);
    player->earnCurrency(cur - player->getCurrency());

    // Load player's selected pet index
    int selectedPetIndex;
    fin >> selectedPetIndex;

    for (int i = 0; i < 4; ++i)/*Loading all pets*/
    {
        string petname;
        int hp, atk, def, spd, abilityUses, maxAbilityUses, level, xp, xpNext, tp, a1, a2;
        int evolutionStage = 0, canEvolve = 0;
        fin >> petname >> hp >> atk >> def >> spd >> abilityUses >> maxAbilityUses >> level >> xp >> xpNext >> tp >> a1 >> a2 >> evolutionStage >> canEvolve;
        Pet* pet = availablePets[i];
        pet->setName(petname);
        pet->heal(hp - pet->getHealth());
        pet->setAttackPower(atk);
        pet->setDefense(def);
        pet->increaseSpeed(spd - pet->getSpeed());
        pet->restoreAbilityUses(abilityUses - pet->getAbilityUses());
        pet->setMaxAbilityUses(maxAbilityUses);
        pet->level = level;
        pet->experience = xp;
        pet->experienceToNextLevel = xpNext;
        pet->trainingPoints = tp;
        pet->ability1Unlocked = a1;
        pet->ability2Unlocked = a2;
        pet->evolutionStage = evolutionStage;
        pet->canEvolve = canEvolve;
    }

    // Manually select the player's pet based on saved index
    if (selectedPetIndex >= 0 && selectedPetIndex < 4) {
        // Simulate selecting the pet by calling selectPet and passing in the index+1
        // First, save the cin state
        std::streambuf* orig_cin = cin.rdbuf();

        // Create a stringstream with the pet index and a 0 for no name change
        std::stringstream temp_cin;
        temp_cin << (selectedPetIndex + 1) << std::endl << 0 << std::endl;

        // Redirect cin to use our stringstream
        cin.rdbuf(temp_cin.rdbuf());

        // Call selectPet which will read from our stringstream
        player->selectPet(availablePets, 4);

        // Restore the original cin
        cin.rdbuf(orig_cin);
    }

    for (int i = 0; i < INVENTORY_SIZE; ++i) /*Loading inventory*/
    {
        string itemName;
        fin >> itemName;

        // Only add items to inventory if a pet is selected
        if (player->getPet() != nullptr && itemName != "EMPTY")
        {
            Item* newItem = nullptr;
            if (itemName == "Healing Potion")
            {
                newItem = new HealingPotion();
            }
            else if (itemName == "Mana Potion")
            {
                newItem = new ManaPotion();
            }
            else if (itemName == "Buff Item")
            {
                newItem = new BuffItem();
            }
            else if (itemName == "Shield")
            {
                newItem = new Shield();
            }
            if (newItem)
            {
                player->getPet()->getInventory()[i] = newItem;
            }
        }
        else
        {
            if (player->getPet() != nullptr) {
                player->getPet()->getInventory()[i] = nullptr;
            }
        }
    }
    string section;
    fin >> section;      /*Loading guilds*/
    if (section == "GUILDS")
    {
        while (true)
        {
            string guildName;
            int wins, losses, ranking;
            fin >> guildName;
            if (guildName == "END_GUILDS") break;
            fin >> wins >> losses >> ranking;
            Guild* guild = guildManager->getGuild(guildName);
            if (guild == nullptr)
            {
                guildManager->createGuild(guildName);
                guild = guildManager->getGuild(guildName);
            }
            guild->recordWin();
            guild->recordLoss();
            while (true)
            {
                string petName, role;
                fin >> petName;
                if (petName == "END_GUILD") break;
                fin >> role;
                Pet* pet = nullptr;
                for (int i = 0; i < 4; ++i)
                {
                    if (availablePets[i]->getName() == petName)
                    {
                        pet = availablePets[i];
                        break;
                    }
                }
                if (pet) guild->addPet(pet, role);
            }
        }
    }
    fin.close();
    cout << "\nProgress loaded successfully!\n";
    cout << "Press Enter to return to the main menu...";
    cin.clear();
    cin.ignore(10000, '\n');
    cin.get();
}

void Game::battleLoop()
{
    string lastAction = "Battle begins!";
    while (player->getPet()->getHealth() > 0 && bot->getPet()->getHealth() > 0)
    {
        arena->updateLog(lastAction);
        arena->showArena();
        cout << "\n1) Action 2) Shop 3) Save 4) Load" << endl;
        int choice;
        while (true)
        {
            cin >> choice;
            if (cin.fail())
            {
                cout << "Invalid input! Please enter a valid number.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }
            else
            {
                cin.ignore(10000, '\n');
                break;
            }
        }
        if (choice == 1)
        {
            /*Player's turn*/
            cout << "Choose action: 1) Attack 2) Use Ability 3) Use Item\n";
            int act;
            while (true)
            {
                cin >> act;
                if (cin.fail())
                {
                    cout << "Invalid input! Please enter a valid number.\n";
                    cin.clear();
                    cin.ignore(10000, '\n');
                }
                else {
                    cin.ignore(10000, '\n');
                    break;
                }
            }
            if (act == 1)
            {
                player->getPet()->attack(bot->getPet());
                lastAction = "Player's " + player->getPet()->getName() + " attacked! ";
            }
            else if (act == 2)
            {
                int abilityNumber = 1;
                cout << "Choose ability: 1) First Ability 2) Second Ability\n";
                cin >> abilityNumber;
                if (cin.fail() || (abilityNumber != 1 && abilityNumber != 2)) abilityNumber = 1;
                player->getPet()->useAbility(bot->getPet(), abilityNumber);
                lastAction = "Player's " + player->getPet()->getName() + " used ability! ";
            }
            else if (act == 3)
            {
                player->showInventory();
                cout << "Select item index: ";
                int idx;
                while (true)
                {
                    cin >> idx;
                    if (cin.fail())
                    {
                        cout << "Invalid input! Please enter a valid number." << endl;
                        cin.clear();
                        cin.ignore(10000, '\n');
                    }
                    else {
                        cin.ignore(10000, '\n');
                        break;
                    }
                }
                player->useItem(idx - 1);
                lastAction = "Player used an item! ";
            }
            else
            {
                lastAction = "Invalid action! ";
            }
            // Bot's turn
            int botAct = rand() % 3;
            if (bot->getPet()->getHealth() > 0)
            {
                if (botAct == 0)
                {
                    bot->getPet()->attack(player->getPet());
                    lastAction += "Bot's " + bot->getPet()->getName() + " attacked!";
                }
                else if (botAct == 1)
                {
                    int botAbility = 1;
                    if (bot->getPet()->ability2Unlocked) botAbility = (rand() % 2) + 1;
                    bot->getPet()->useAbility(player->getPet(), botAbility);
                    lastAction += "Bot's " + bot->getPet()->getName() + " used ability!";
                }
                else
                {
                    bot->useItem(0);
                    lastAction += "Bot used an item!";
                }
            }
        }
        else if (choice == 2)
        {
            showShop();
            lastAction = "Visited shop.";
        }
        else if (choice == 3)
        {
            saveProgress();
            cout << "Press Enter to return to the main menu...";
            cin.ignore(10000, '\n');
            cin.get();
            break;
        }
        else if (choice == 4)
        {
            loadProgress();
            cout << "Press Enter to return to the main menu...";
            cin.ignore(10000, '\n');
            cin.get();
            break;
        }
        else
        {
            lastAction = "Invalid menu choice.";
        }
    }
    arena->updateLog(player->getPet()->getHealth() > 0 ? "You win!" : "Bot wins!");
    arena->showArena();
    cout << (player->getPet()->getHealth() > 0 ? "\nYou win!\n" : "\nBot wins!\n");
}

void Game::run()
{
    cout << "\n==========================================\n";
    cout << "    Welcome to Afnan's Magical Pets Kingdom!      " << endl;
    cout << "==========================================" << endl << endl;

    while (true)
    {
        cout << "\n==========================================" << endl;
        cout << "              Main Menu                   " << endl;
        cout << "==========================================" << endl;
        cout << "1) Battle Arena - Fight other pets!" << endl;
        cout << "2) Shop - Buy items and potions" << endl;
        cout << "3) Training Camp - Improve your pet" << endl;
        cout << "4) Guilds - Join or create a guild" << endl;
        cout << "5) Leaderboard - View top players" << endl;
        cout << "6) Show Inventory" << endl;
        cout << "7) Save Progress" << endl;
        cout << "8) Load Progress" << endl;
        cout << "9) Exit Game" << endl;
        cout << "==========================================<<" << endl;
        int choice;
        while (true) {
            cout << "Your choice: ";
            cin >> choice;
            if (cin.fail())
            {
                cout << "Invalid input! Please enter a valid number" << endl;
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }
            if (choice >= 1 && choice <= 9)
            {
                break;
            }
            cout << "Invalid choice! Please enter a number between 1 and 9" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
        }
        cin.ignore();

        switch (choice)
        {
        case 1:
            battleMenu();
            break;
        case 2:
            shopMenu();
            break;
        case 3:
            if (player->getPet() == nullptr)
            {
                cout << "\nYou need to select a pet first!" << endl;
                cout << "Select your pet companion:" << endl;
                player->selectPet(availablePets, 4);
                cout << "\nGreat choice! Your " << player->getPet()->getName() << " is ready for training! " << endl;
            }
            trainingMenu();
            break;
        case 4:
            guildMenu();
            break;
        case 5:
            cout << "\n=== Top Players ===\n";
            leaderboard->displayTop5();
            cout << "\nPress Enter to return to the main menu...";
            cin.get();
            break;
        case 6:
            player->showInventory();
            cout << "\nPress Enter to return to the main menu...";
            cin.get();
            break;
        case 7:
            saveProgress();
            break;
        case 8:
            loadProgress();
            break;
        case 9:
            cout << "\nThank you for playing Magical Pets Kingdom!" << endl;
            cout << "Come back soon for more adventures!" << endl;
            return;
        default:
            cout << "\nInvalid choice! Please select a number between 1 and 9." << endl;
            break;
        }
    }
}
void Game::battleMenu()
{
    while (true) 
    {
        cout << "\n=== Battle Menu ===\n";
        cout << "1. 1v1 Battle" << endl;
        cout << "2. 2v2 Battle" << endl;
        cout << "3. Guild War (5v5)" << endl;
        cout << "4. View Leaderboard" << endl;
        cout << "5. Return to Main Menu" << endl;
        cout << "Your choice: ";

        int choice;
        cin >> choice;
        if (cin.fail())
        {
            cout << "Invalid input! Please enter a valid number." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore(10000, '\n');

        int playerIdx = -1, playerIdx1 = -1, playerIdx2 = -1;
        int botIdx = -1, botIdx1 = -1, botIdx2 = -1;
        int petChoice;
        Pet* playerPet1 = nullptr;
        Pet* playerPet2 = nullptr;
        Pet* botPet1 = nullptr;
        Pet* botPet2 = nullptr;
        Pet* playerGuildPets[5] = { nullptr };
        Pet* botGuildPets[5] = { nullptr };
        int selectedSpecies[5] = { -1, -1, -1, -1, -1 };
        int numGuildPets = 0;

        switch (choice)
        {
        case 1:
            cout << "\nPreparing for 1v1 Battle..." << endl;
            cout << "Select your pet:" << endl;
            for (int i = 0; i < 4; ++i)
            {
                cout << i + 1 << ". " << availablePets[i]->getName() << endl;
            }
            cin >> petChoice;
            cin.ignore(10000, '\n');
            if (petChoice < 1 || petChoice > 4) 
            {
                petChoice = 1;
            }
            
            if (petChoice == 1) 
            {
                playerPet1 = new Dragon("Dragon");
            }
            else if (petChoice == 2) 
            {
                playerPet1 = new Phoenix("Phoenix");
            }
            else if (petChoice == 3) 
            {
                playerPet1 = new Unicorn("Unicorn");
            }
            else if (petChoice == 4) 
            {
                playerPet1 = new Griffin("Griffin");
            }

            int nameChoice;
            cout << "Would you like to name your pet? (1 for Yes, 0 for No): ";
            cin >> nameChoice;
            cin.ignore(10000, '\n');
            if (nameChoice == 1)
            {
                cout << "Enter the pet's name: ";
                string petName;
                cin.ignore(10000, '\n');
                getline(cin, petName);
                playerPet1->setName(petName);
            }
            /*Bot selects a random pet different from player's*/
            do { 
                botIdx = rand() % 4;
                 } while (botIdx == (petChoice - 1));
            if (botIdx == 0)
            {
                 botPet1 = new Dragon("Dragon");
            }
            else if (botIdx == 1) 
            {
                botPet1 = new Phoenix("Phoenix");
            }
            else if (botIdx == 2) 
            {
                botPet1 = new Unicorn("Unicorn");
            }
            else if (botIdx == 3)
            {
                 botPet1 = new Griffin("Griffin");
            }
            arena->setupOneVsOne(playerPet1, botPet1);
            arena->startBattle();
            delete playerPet1;
            delete botPet1;
            return;
        case 2:
            cout << "\nPreparing for 2v2 Battle...\n";
            cout << "Select your first pet:\n";
            for (int i = 0; i < 4; ++i) 
            {
                cout << i + 1 << ". " << availablePets[i]->getName() << endl;
            }
            cin >> petChoice;
            cin.ignore(10000, '\n');
            if (petChoice < 1 || petChoice > 4) 
            {
                petChoice = 1;
            }
            if (petChoice == 1) 
            {
                playerPet1 = new Dragon("Dragon");
            }
            else if (petChoice == 2) 
            {
                playerPet1 = new Phoenix("Phoenix");
            }
            else if (petChoice == 3) 
            {
                playerPet1 = new Unicorn("Unicorn");
            }
            else if (petChoice == 4) 
            {
                playerPet1 = new Griffin("Griffin");
            }
            cout << "Would you like to name your first pet? (1 for Yes, 0 for No): ";
            cin >> nameChoice;
            cin.ignore(10000, '\n');
            if (nameChoice == 1)
            {
                cout << "Enter the pet's name: ";
                string petName;
                cin.ignore(10000, '\n');
                getline(cin, petName);
                playerPet1->setName(petName);
            }

            cout << "Select your second pet:" << endl;
            for (int i = 0; i < 4; ++i)
            {
                cout << i + 1 << ". " << availablePets[i]->getName() << endl;
            }
            cin >> petChoice;
            cin.ignore(10000, '\n');
            if (petChoice < 1 || petChoice > 4) petChoice = 2;
            
            if (petChoice == 1) 
            {
                playerPet2 = new Dragon("Dragon");
            }
            else if (petChoice == 2) 
            {
                playerPet2 = new Phoenix("Phoenix");
            }
            else if (petChoice == 3) 
            {
                playerPet2 = new Unicorn("Unicorn");
            }
            else if (petChoice == 4)
            {
                 playerPet2 = new Griffin("Griffin");
            }
            cout << "Would you like to name your second pet? (1 for Yes, 0 for No): ";
            cin >> nameChoice;
            cin.ignore(10000, '\n');
            if (nameChoice == 1) 
            {
                cout << "Enter the pet's name: ";
                string petName;
                cin.ignore(10000, '\n');
                getline(cin, petName);
                playerPet2->setName(petName);
            }

            cout << "You own the following pets:" << endl;
            cout << "1. " << playerPet1->getName() << endl;
            cout << "2. " << playerPet2->getName() << endl;

            /*Bot selects two random pets different from player's*/
            do { 
                botIdx1 = rand() % 4;
                 } while (botIdx1 == (playerPet1 - availablePets[0]));
            do {
                 botIdx2 = rand() % 4;
             } while (botIdx2 == (playerPet1 - availablePets[0]) || botIdx2 == (playerPet2 - availablePets[0]) || botIdx2 == botIdx1);
           
            if (botIdx1 == 0) 
            {
                botPet1 = new Dragon("Dragon");
            }
            else if (botIdx1 == 1) 
            {
                botPet1 = new Phoenix("Phoenix");
            }
            else if (botIdx1 == 2) 
            {
                botPet1 = new Unicorn("Unicorn");
            }
            else if (botIdx1 == 3) botPet1 = new Griffin("Griffin");

            if (botIdx2 == 0) 
            {
                botPet2 = new Dragon("Dragon");
            }
            else if (botIdx2 == 1) 
            {
                botPet2 = new Phoenix("Phoenix");
            }
            else if (botIdx2 == 2) 
            {
                botPet2 = new Unicorn("Unicorn");
            }
            else if (botIdx2 == 3) 
            {
                botPet2 = new Griffin("Griffin");
            }
            arena->setupTwoVsTwo(playerPet1, playerPet2, botPet1, botPet2);
            arena->startBattle();
            delete playerPet1;
            delete playerPet2;
            delete botPet1;
            delete botPet2;
            return;
        case 3:
            cout << "\nPreparing for Guild War (5v5)...\n";
            for (int i = 0; i < 5; ++i)
            {
                cout << "Select guild member " << (i + 1) << ":\n";
                for (int j = 0; j < 4; ++j) {
                    cout << j + 1 << ". " << availablePets[j]->getName() << endl;
                }
                cin >> petChoice;
                cin.ignore(10000, '\n');
                if (petChoice < 1 || petChoice > 4)
                {
                    petChoice = 1;
                }
                // Always create a new pet of the selected type (allowing duplicates)
                if (petChoice == 1)
                {
                    playerGuildPets[i] = new Dragon("Dragon");
                }
                else if (petChoice == 2)
                {
                    playerGuildPets[i] = new Phoenix("Phoenix");
                }
                else if (petChoice == 3)
                {
                    playerGuildPets[i] = new Unicorn("Unicorn");
                }
                else if (petChoice == 4)
                {
                    playerGuildPets[i] = new Griffin("Griffin");
                }
                cout << "Would you like to name this pet? (1 for Yes, 0 for No): ";
                int nameChoice;
                cin >> nameChoice;
                cin.ignore(10000, '\n');
                if (nameChoice == 1)
                {
                    cout << "Enter the pet's name: ";
                    string petName;
                    cin.ignore(10000, '\n');
                    getline(cin, petName);
                    playerGuildPets[i]->setName(petName);
                }
            }
            // Show the names of the player's guild pets after selection
            cout << "You own the following guild pets:" << endl;
            for (int i = 0; i < 5; ++i) 
            {
                cout << i + 1 << ". " << playerGuildPets[i]->getName() << endl;
            }
            for (int i = 0; i < 5; ++i)
            {
                int botSpecies = rand() % 4;
                if (botSpecies == 0)
                {
                    botGuildPets[i] = new Dragon("Dragon");
                }
                else if (botSpecies == 1)
                {
                    botGuildPets[i] = new Phoenix("Phoenix");
                }
                else if (botSpecies == 2)
                {
                    botGuildPets[i] = new Unicorn("Unicorn");
                }
                else
                {
                    botGuildPets[i] = new Griffin("Griffin");
                }
            }
            arena->setupGuildWar(playerGuildPets, botGuildPets);
            arena->startBattle();

            for (int i = 0; i < 5; ++i)
            {
                delete playerGuildPets[i];
                delete botGuildPets[i];
            }
            return;
        case 4:
            leaderboard->displayTop5();
            break;
        case 5:
            return;
        default:
            cout << "Invalid choice! Please select a number between 1 and 5.\n";
            break;
        }
    }
}
void Game::shopMenu()
{
    while (true)
     {
        shop->showItems();
        cout << "\n1) Buy Item\n2) Back\n";
        int choice; cin >> choice;

        if (choice == 1) 
        {
            cout << "Enter item number to buy: ";
            int idx; 
            cin >> idx;
            if (idx > 0 && idx <= 4)
             {
                if (shop->canAfford(idx - 1, player->getCurrency()))
                {
                    Item* item = shop->buyItem(idx - 1);
                    if (item) {
                        player->addItem(item);
                        player->earnCurrency(-shop->getItemPrice(idx - 1));
                        cout << "Item purchased successfully!" << endl;
                    }
                }
                else
                {
                    cout << "Not enough currency!" << endl;
                }
            }
        }
        else if (choice == 2)
        {
             break;
        }
        else 
        {
            cout << "Invalid choice!" << endl;
        }
    }
}
void Game::trainingMenu()
{
    while (true) {
        trainingCamp->showCamp();
        cout << "Choose a training activity:" << endl;
        cout << "1) Agility Course (Speed)" << endl;
        cout << "2) Strength Training (Attack)" << endl;
        cout << "3) Defense Drills (Defense)" << endl;
        cout << "4) Evolution (if available)" << endl;
        cout << "5) Return to Main Menu" << endl;
        cout << "Your choice: ";
        int choice;
        cin >> choice;

        if (choice == 5)
        {
            return;
        }

        if (choice == 4)
        {
            if (player->getPet()->getCanEvolve())
            {
                cout << "\n" << player->getPet()->getName() << " is ready to evolve!" << endl;
                cout << "Would you like to evolve now? (1: Yes, 0: No): ";
                int evolveChoice;
                cin >> evolveChoice;
                if (evolveChoice == 1) {
                    if (player->getPet()->evolve()) {
                        cout << "Evolution successful!" << endl;
                    }
                }
            }
            else
            {
                cout << endl<< player->getPet()->getName() << " is not ready to evolve yet." << endl;
                cout << "Reach level " << Pet::EVOLUTION_LEVELS[player->getPet()->getEvolutionStage()] << " to evolve!" << endl;
            }
            continue;
        }
        if (choice >= 1 && choice <= 3)
        {
            trainingCamp->trainPet(player->getPet(), choice);
        }
        else
        {
            cout << "Invalid choice!" << endl;
        }
    }
}

void Game::guildMenu()
{
    while (true)
    {
        cout << "\n=== Guild Menu ===\n";
        cout << "1) Create Guild\n";
        cout << "2) Join Guild\n";
        cout << "3) Show My Guild\n";
        cout << "4) Show Rankings\n";
        cout << "5) Start Guild War\n";
        cout << "6) Back\n";
        int choice;
        while (true)
        {
            cout << "Your choice: ";
            cin >> choice;
            if (cin.fail())
            {
                cin.clear();
                cout << "Invalid input! Please enter a valid number" << endl;
                cin.ignore(100000, '\n');
                continue;
            }
            cin.ignore(10000, '\n');
            if (choice >= 1 && choice <= 6)
            {
                break;
            }
            cout << "Invalid choice! Please select a number between 1 and 6.\n";
        }
        cin.ignore();
        switch (choice)
        {
        case 1:
        {
            cout << "Enter guild name: ";
            string name;
            getline(cin, name);
            if (guildManager->createGuild(name))
            {
                cout << "Guild created!" << endl;
                Guild* guild = guildManager->getGuild(name);
                int numPets = 0;
                while (true)
                {
                    cout << "How many pets do you want to add to your guild? (1-5): ";
                    cin >> numPets;
                    if (cin.fail() || numPets < 1 || numPets > 5)
                    {
                        cin.clear();
                        cout << "Please enter a number between 1 and 5.\n";
                        cin.ignore(10000, '\n');
                        continue;
                    }
                    cin.ignore(10000, '\n');
                    break;
                }
                for (int i = 0; i < numPets; ++i)
                {
                    cout << "Select pet " << (i + 1) << endl;
                    cout << "1. Dragon\n2. Phoenix\n3. Unicorn\n4. Griffin\n";
                    int petChoice;
                    while (!(cin >> petChoice) || petChoice < 1 || petChoice > 4)
                    {
                        cin.clear();
                        cout << "Invalid input! Please enter a valid number between 1 and 4" << endl;
                        cin.ignore(10000, '\n');
                    }
                    cin.ignore(10000, '\n');
                    Pet* selectedPet = nullptr;
                    if (petChoice == 1)
                        selectedPet = new Dragon("Dragon");
                    else if (petChoice == 2)
                        selectedPet = new Phoenix("Phoenix");
                    else if (petChoice == 3)
                        selectedPet = new Unicorn("Unicorn");
                    else if (petChoice == 4)
                        selectedPet = new Griffin("Griffin");
                    cout << "Would you like to name this pet? (1 for Yes, 0 for No): ";
                    int nameChoice;
                    while (!(cin >> nameChoice) || (nameChoice != 0 && nameChoice != 1))
                    {
                        cin.clear();
                        cout << "Invalid input! Please enter 1 for Yes or 0 for No" << endl;
                        cin.ignore(10000, '\n');
                    }
                    cin.ignore(10000, '\n');
                    if (nameChoice == 1)
                    {
                        cout << "Enter the pet's name: ";
                        string petName;
                        getline(cin, petName);
                        selectedPet->setName(petName);
                    }
                    cout << "Assign a role to this pet: ";
                    string role;
                    getline(cin, role);
                    guild->addPet(selectedPet, role);
                }
                /* this displays all pets in the guild with their names and roles*/
                cout << "\nYour guild pets:" << endl;
                for (int i = 0; i < guild->getPetCount(); ++i)
                {
                    Pet* pet = guild->getPet(i);
                    cout << i + 1 << ". " << pet->getName() << " (" << guild->getRole(i) << ")" << endl;
                }
            }
            else
            {
                cout << "Failed to create guild.\n";
            }
            break;
        }
        case 2: 
        {
            cout << "Select guild to join:\n";
            guildManager->showAllGuilds();
            if (guildManager->getGuildCount() == 0)
             {
                cout << "No guilds available. Please create a guild first.\n";
                break;
            }
            string name;
            Guild* guild = nullptr;
            bool guildFull = false;
            cin.ignore(10000, '\n'); // Only once before the loop
            do {
                cout << "Enter guild name: ";
                getline(cin, name);
                guild = guildManager->getGuild(name);
                if (guild) {
                    // Check if the guild is already full
                    if (guild->getPetCount() >= Guild::MAX_GUILD_PETS) {
                        cout << "This guild is already full. Press 0 to return to the menu or try another guild.\n";
                        string input;
                        getline(cin, input);
                        if (input == "0") {
                            guildFull = true;
                            break;
                        }
                        else {
                            continue;
                        }
                    }
                }
                else {
                    cout << "Guild not found! Please enter a valid guild name."<<endl;
                }
            } while (!guild || guildFull);
            if (guildFull || !guild) break;
            cout << "Assign a role to your pet in this guild (e.g., tank, healer, damage dealer): ";
            string role;
            getline(cin, role);
            if (guild->addPet(player->getPet(), role))
             {
                cout << "Joined guild as " << role << "!" << endl;
            }
            else {
                cout << "Guild is full!"<<endl;
            }
            cout << "Would you like to name this pet? (1 for Yes, 0 for No): ";
            int nameChoice;
            while (!(cin >> nameChoice) || (nameChoice != 0 && nameChoice != 1))
            {
                cin.clear();
                cout << "Invalid input! Please enter 1 for Yes or 0 for No" << endl;
                cin.ignore(10000, '\n');
            }
            cin.ignore(10000, '\n');
            if (nameChoice == 1)
            {
                cout << "Enter the pet's name: ";
                string petName;
                getline(cin, petName);
                player->getPet()->setName(petName);
            }
            break;
        }
        case 3:
        {
            cout << "Select guild to view:" << endl;
            guildManager->showAllGuilds();
            cout << "Enter guild name: ";
            string name;
            cin.ignore(10000, '\n');
            getline(cin, name);
            Guild* guild = guildManager->getGuild(name);
            if (guild)
            {
                guild->showGuild();
            }
            else
            {
                cout << "Guild not found!\n";
            }
            break;
        }
        case 4:
            guildManager->showGuildRankings();
            break;
        case 5: {
            cout << "Select first guild:\n";
            guildManager->showAllGuilds();
            string g1name;
            Guild* g1 = nullptr;
            cin.ignore(10000, '\n'); 
            while (true)
            {
                cout << "Enter first guild name: ";
                getline(cin, g1name);
                g1 = guildManager->getGuild(g1name);
                if (g1)
                {
                    break;
                }
                cout << "Invalid first guild selection.\n";
            }
            cout << "Enter second guild name: ";
            string g2name;
            Guild* g2 = nullptr;
            while (true)
            {
                getline(cin, g2name);
                g2 = guildManager->getGuild(g2name);
                if (g2 && g2 != g1)
                {
                    break;
                }
                cout << "Invalid second guild selection" << endl;
            }
            cout << "Starting guild war between " << g1->getName() << " and " << g2->getName() << "..." << endl;
            if (guildManager->initiateGuildWar(g1, g2))
            {
                cout << "Guild war completed!" << endl;
            }
            else
            {
                cout << "Failed to start guild war" << endl;
            }
            break;
        }
        case 6:
            return;
        default:
            cout << "Invalid choice!" << endl;
            break;
        }
    }
}