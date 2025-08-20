//Syeda afnan hussain,  SE-A
#ifndef TRAININGCAMP_H
#define TRAININGCAMP_H
#include "Pet.h"
#include <string>
using namespace std;
class TrainingCamp 
{
    string name;
public:
    TrainingCamp(const std::string& n);
    void trainPet(Pet* pet, int activityChoice);
    void restPet(Pet* pet);
    void showCamp() const;
};
#endif 