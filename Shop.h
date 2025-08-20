//Syeda afnan hussain,  SE-A
#ifndef SHOP_H
#define SHOP_H
#include "Item.h"
#include <string>

class Shop
 {
private:
    Item** items;
    int itemCount;
    static const int MAX_ITEMS = 4;

public:
    Shop();
    ~Shop();

    void showItems() const;
    Item* buyItem(int index);
    int getItemPrice(int index) const;
    bool canAfford(int index, int currency) const;
};

#endif 