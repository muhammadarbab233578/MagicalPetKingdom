//Syeda Afnan hUssain, SE-A
/*so the purpose of this class is to train the  pets inorder to increase their stats and make them stronger
.When the pets get tired they are asked to rest, pets must rest for 3 turns before training again*/
#include "TrainingCamp.h"
#include <iostream>

using namespace std;

TrainingCamp::TrainingCamp(const string& n)
 {
    name = n;
}

void TrainingCamp::trainPet(Pet* pet, int activityChoice) 
{
    if (pet->fatigue > 0)
     {
        cout << pet->getName() << " is too tired to train! Needs " << pet->fatigue << " more rest turn(s)"<<endl;
        return;
    }

    int progress = rand() % 10 + 5; /*Random progress between 5 and 14*/
    bool breakthrough = (rand() % 100 < 10); /*10% chance for a breakthrough*/

    switch (activityChoice)
     {
        case 1: 
            if (breakthrough)
             {
                progress += 10;
                cout << "Breakthrough! ";
            }
            pet->speedProgress += progress;
            cout << pet->getName() << " completed Agility Course! Speed progress +" << progress <<endl;
            if (pet->speedProgress >= 100) 
            {
                pet->increaseSpeed(1);
                pet->speedProgress -= 100;
                cout << pet->getName() << "'s Speed increased by 1!"<<endl;
            }
            break;
        case 2: 
            if (breakthrough) 
            {
                progress += 10;
                cout << "Breakthrough! ";
            }
            pet->attackProgress += progress;
            cout << pet->getName() << " completed Strength Training! Attack progress +" << progress << ".\n";
            if (pet->attackProgress >= 100) 
            {
                pet->increaseAttack(1);
                pet->attackProgress -= 100;
                cout << pet->getName() << "'s Attack increased by 1!"<<endl;
            }
            break;
        case 3: 
            if (breakthrough)
             {
                progress += 10;
                cout << "Breakthrough! ";
            }
            pet->defenseProgress += progress;
            cout << pet->getName() << " completed Defense Drills! Defense progress +" << progress <<endl;
            if (pet->defenseProgress >= 100) 
            {
                pet->increaseDefense(1);
                pet->defenseProgress -= 100;
                cout << pet->getName() << "'s Defense increased by 1!"<<endl;
            }
            break;
        default:
            cout << "Invalid activity choice!"<<endl;
            return;
    }
    pet->fatigue = 3; /*Pet must rest 3 turns before training again*/
}

void TrainingCamp::restPet(Pet* pet)
 {
    if (pet->fatigue > 0) 
    {
        pet->fatigue--;
        cout << pet->getName() << " is resting. Fatigue: " << pet->fatigue <<endl;
    }
     else 
    {
        cout << pet->getName() << " is ready to train!"<<endl;
    }
}

void TrainingCamp::showCamp() const 
{
    cout << "Training Camp: " << name << endl;
} 