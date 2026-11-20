// Animal.cpp
#include "Animal.h"
#include <sstream>

using namespace std;

string typeOfSpecies(string speciesName) {
    if (speciesName == "cat" || speciesName == "dog") {
        return "mammal";
    }
    if (speciesName == "eagle" || speciesName == "parakeet") {
        return "bird";
    }
    return "fish";   // goldfish, shark
}

Animal::Animal() {
    species = "cat";
    type = "mammal";
    female = true;
    age = 0;
}

Animal::Animal(string speciesName, bool isFemale, int startAge) {
    species = speciesName;
    type = typeOfSpecies(speciesName);
    female = isFemale;
    age = startAge;
}

string Animal::getSpecies() const { return species; }
string Animal::getType() const { return type; }
bool Animal::isFemale() const { return female; }
int Animal::getAge() const { return age; }

void Animal::ageOneMonth() {
    age++;
}

// Mammals: 12 months, fish: 6 months, birds: 9 months
int Animal::getBirthInterval() const {
    if (type == "mammal") return 12;
    if (type == "fish") return 6;
    return 9;
}

// A female gives birth each time her age is a multiple of her interval
bool Animal::readyToGiveBirth() const {
    if (!female) return false;
    if (age == 0) return false;
    return (age % getBirthInterval() == 0);
}

string Animal::describe() const {
    ostringstream out;
    out << "I am a " << (female ? "female" : "male") << " "
        << species << ", " << age << " months old";
    return out.str();
}
