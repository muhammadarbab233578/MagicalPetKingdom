//Syeda Afnan Hussain , SE-A
/*The purpose of this class is to manage the shop from where the player buys different items by spending
the currency they earned by winning mathces*/
#include "Shop.h"
#include <iostream>

using namespace std;

Shop::Shop()
 {
    itemCount = MAX_ITEMS;
    items = new Item*[MAX_ITEMS];
    items[0] = new HealingPotion();
    items[1] = new ManaPotion();
    items[2] = new BuffItem();
    items[3] = new Shield();
}

Shop::~Shop() 
{
    for (int i = 0; i < itemCount; ++i)
     {
        delete items[i];
    }
    delete[] items;
}

void Shop::showItems() const 
{
    cout << "\n=== Shop ===\n";
    for (int i = 0; i < itemCount; i++) 
    {
        cout << (i + 1) << ". " << items[i]->getName() 
             << " - " << items[i]->getPrice() << " rupees" << endl;
    }
}

Item* Shop::buyItem(int index) 
{
    if (index >= 0 && index < itemCount)
     {
        switch(index) 
        {
            case 0:
                return new HealingPotion();
            case 1:
                return new ManaPotion();
            case 2:
                return new BuffItem();
            case 3:
                return new Shield();
            default:
                return nullptr;
        }
    }
    return nullptr;
}

int Shop::getItemPrice(int index) const
 {
    if (index >= 0 && index < itemCount) 
    {
        return items[index]->getPrice();
    }
    return 0;
}

bool Shop::canAfford(int index, int currency) const 
{
    return getItemPrice(index) <= currency;
} 