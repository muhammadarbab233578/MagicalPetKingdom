//Syeda Afnan hussain , SE-A
/*The purpose of this class is to create ,join and manage guilds, guild wars, add pets in the guilds
assign them roles, manage guild wars , show guild rankings ect*/
#include<iostream>
#include "Guild.h"
using namespace std;

Guild::Guild(const string& n)
{
    name = n;
    petCount = 0;
    wins = 0;
    losses = 0;
    ranking = 0;
    logCount = 0;
    for (int i = 0; i < MAX_GUILD_PETS; i++)
    {
        pets[i] = nullptr;
        roles[i] = "";
    }
}
Guild::~Guild()
{
    for (int i = 0; i < MAX_GUILD_PETS; i++)
    {
        pets[i] = nullptr;  /*We are not deleting  pets as they are owned by the Game class*/
    }
    for (int i = 0; i < MAX_BATTLE_LOG; i++)
    {
        battleLog[i] = "";
    }
    logCount = 0;
}

bool Guild::addPet(Pet* pet, const string& role)
{
    if (pet == nullptr)
    {
        return false;
    }
    if (petCount >= MAX_GUILD_PETS)
    {
        cout << "Max number of pets is " << MAX_GUILD_PETS << endl;
        return false;
    }
    pets[petCount] = pet;
    roles[petCount] = role;
    petCount++;
    return true;
}

void Guild::showGuild() const
{
    cout << endl;
    cout << "=== " << name << " ===" << endl;
    cout << "Ranking: " << ranking << endl;
    cout << " (Wins: " << wins << ", Losses: " << losses << ")" << endl;
    cout << "Members:" << endl;
    for (int i = 0; i < petCount; i++)
    {
        cout << i + 1 << ". " << pets[i]->getName() << " ----" << roles[i] << endl;
    }
}

string Guild::getName() const
{
    return name;
}

void Guild::updateRanking()
{
    if (losses == 0)
    {
        ranking = wins * 100;
    }
    else
    {
        ranking = (wins * 100) / losses;
    }
}
void Guild::recordBattle(const string& action)/*commentary*/
{
    if (logCount < MAX_BATTLE_LOG)
    {
        battleLog[logCount++] = action;
    }
}
void Guild::showBattleLog() const
{
    cout << endl;
    cout << "=== Battle Log for " << name << " ===" << endl;
    for (int i = 0; i < logCount; ++i)
    {
        cout << battleLog[i] << "\n";
    }
}
GuildManager::GuildManager()
{
    guildCount = 0;
    guildCapacity = Guild::MAX_GUILDS;
    guilds = new Guild * [guildCapacity];
    for (int i = 0; i < guildCapacity; ++i)
    {
        guilds[i] = nullptr;
    }
}
GuildManager::~GuildManager()
{
    for (int i = 0; i < guildCount; ++i)
    {
        delete guilds[i];
    }
    delete[] guilds;
}
bool GuildManager::createGuild(const string& name)
{
    if (name.empty())
    {
        return false;
    }
    if (guildCount >= guildCapacity)
    {
        return false;
    }
    /*here we are checking if guild with same name already exists*/
    for (int i = 0; i < guildCount; ++i)
    {
        if (guilds[i] && guilds[i]->getName() == name)
        {
            return false;
        }
    }
    guilds[guildCount++] = new Guild(name);
    return true;
}
Guild* GuildManager::getGuild(const string& name)
{
    if (name.empty())
    {
        return nullptr;
    }
     for (int i = 0; i < guildCount; ++i)
        if (guilds[i] && guilds[i]->getName() == name)
            return guilds[i];
    return nullptr;
    
}

void GuildManager::showAllGuilds() const
{
    cout << "\n=== All Guilds ===" << endl;
    for (int i = 0; i < guildCount; ++i)
    {
        guilds[i]->showGuild();
    }
}
void GuildManager::showGuildRankings() const/*displays the guilds on console based on the rankings*/
{
    cout << "\n=== Guild Rankings ===" << endl;
    Guild** sortedGuilds = new Guild * [guildCount];/*Creating a copy of guilds array for sorting*/
    for (int i = 0; i < guildCount; ++i)
    {
        sortedGuilds[i] = guilds[i];
    }
    for (int i = 0; i < guildCount - 1; ++i) /*sorting the guilds based on ranking*/
    {
        for (int j = 0; j < guildCount - i - 1; ++j)
        {
            if (sortedGuilds[j]->getRanking() < sortedGuilds[j + 1]->getRanking())
            {
                Guild* temp = sortedGuilds[j];
                sortedGuilds[j] = sortedGuilds[j + 1];
                sortedGuilds[j + 1] = temp;
            }
        }
    }
    for (int i = 0; i < guildCount; ++i)
    {
        cout << i + 1 << ". " << sortedGuilds[i]->getName()
            << " (Rank: " << sortedGuilds[i]->getRanking() /*dsplayinh rankings*/
            << ", W/L: " << sortedGuilds[i]->getWins()
            << "/" << sortedGuilds[i]->getLosses() << ")" << endl;
    }
    delete[] sortedGuilds;
}
bool GuildManager::initiateGuildWar(Guild* guild1, Guild* guild2)
{
    if (guild1 == nullptr || guild2 == nullptr || guild1 == guild2)
    {
        return false;
    }
    GuildWar war(guild1, guild2);/*Creates temporary object*/
    war.startWar();
    Guild* winner = war.determineWinner();/*winner will point to either of the 2 guilds*/
    if (winner)
    {
        winner->recordWin();
        (winner == guild1 ? guild2 : guild1)->recordLoss();
    }
    return true;
}
Guild** GuildManager::getTopRankedGuilds(int& count) const /*gives access to the top guilds*/
{
    if (count <= 0 || guildCount <= 0)
    {
        count = 0;
        return nullptr;
    }
    if (count > guildCount)
    {
        count = guildCount;
    }
    Guild** sortedGuilds = new Guild * [guildCount];
    for (int i = 0; i < guildCount; ++i)
    {
        sortedGuilds[i] = guilds[i];
    }

    for (int i = 0; i < guildCount - 1; ++i)
    {
        for (int j = 0; j < guildCount - i - 1; ++j)
        {
            if (sortedGuilds[j]->getRanking() < sortedGuilds[j + 1]->getRanking())
            {
                Guild* temp = sortedGuilds[j];
                sortedGuilds[j] = sortedGuilds[j + 1];
                sortedGuilds[j + 1] = temp;
            }
        }
    }
    Guild** result = new Guild * [count];/*creates array of top guuildss*/
    for (int i = 0; i < count; ++i)
    {
        result[i] = sortedGuilds[i];
    }
    delete[] sortedGuilds;
    return result;
}
GuildWar::GuildWar(Guild* g1, Guild* g2)
{
    guild1 = g1;
    guild2 = g2;
    logCount = 0;
    logCapacity = Guild::MAX_BATTLE_LOG;
    isActive = false;
    battleLog = new string[logCapacity];
}
GuildWar::~GuildWar()
{
    delete[] battleLog;
}
void GuildWar::startWar()
{
    if (guild1 == nullptr || guild2 == nullptr)
    {
        return;
    }
    isActive = true;
    recordBattle("Guild War started between " + guild1->getName() + " and " + guild2->getName());
}
void GuildWar::recordBattle(const string& action)
{
    if (logCount < logCapacity)
    {
        battleLog[logCount++] = action;
    }
}

void GuildWar::showBattleLog() const
{
    cout << "\n=== Guild War Battle Log ===" << endl;
    for (int i = 0; i < logCount; ++i)
    {
        cout << battleLog[i] << endl;
    }
}
Guild* GuildWar::determineWinner()
{
    if (!isActive)
    {
        return nullptr;
    }
    int guild1Score = 0;
    int guild2Score = 0;

    /*for starting  battles b/w pets*/
    for (int i = 0; i < guild1->getPetCount() && i < guild2->getPetCount(); ++i)
    {
        Pet* pet1 = guild1->getPet(i);
        Pet* pet2 = guild2->getPet(i);

        if (pet1 == nullptr || pet2 == nullptr)
        {
            continue;
        }
        /*start battle b/w  2 pets*/
        while (pet1->getHealth() > 0 && pet2->getHealth() > 0)
        {
            if (pet1->getHealth() > 0)
            {
                pet1->attack(pet2);
                recordBattle(guild1->getName() + "'s " + pet1->getName() + " attacks " +
                    guild2->getName() + "'s " + pet2->getName());
            }

            if (pet2->getHealth() > 0)
            {
                pet2->attack(pet1);
                recordBattle(guild2->getName() + "'s " + pet2->getName() + " attacks " +
                    guild1->getName() + "'s " + pet1->getName());
            }
        }

        if (pet1->getHealth() > 0) /*Points to winners are awarded*/
        {
            guild1Score++;
        }
        else if (pet2->getHealth() > 0)
        {
            guild2Score++;
        }
        pet1->heal(100);/*Resetting health ofor next battleee*/
        pet2->heal(100);
    }

    /*overall winner*/
    Guild* winner = nullptr;
    if (guild1Score > guild2Score)
    {
        winner = guild1;
    }
    else if (guild2Score > guild1Score)
    {
        winner = guild2;
    }
    isActive = false;
    return winner;
}
void GuildWar::endWar()
{
    if (guild1 == nullptr || guild2 == nullptr)
    {
        return;
    }
    isActive = false;
    recordBattle("Guild War ended between " + guild1->getName() + " and " + guild2->getName());
}
ostream& operator<<(ostream& COUT, const Guild& guild)
{
    COUT << "Guild: " << guild.getName() << " (Rank: " << guild.getRanking() << ")";
    return COUT;
}
bool Guild::operator==(const Guild& other) const
{
    return (this->name == other.name);
}
bool Guild::operator<(const Guild& other) const
{
    return (this->ranking < other.ranking);
}