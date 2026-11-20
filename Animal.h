// Animal.h
// Describes a single animal on the ark.
#ifndef ANIMAL_H
#define ANIMAL_H

#include <string>

class Animal {
private:
    std::string species;   // "cat", "dog", "goldfish", ...
    std::string type;      // "mammal", "bird", or "fish"
    bool female;           // true = female, false = male
    int age;               // age in months

public:
    Animal();
    Animal(std::string speciesName, bool isFemale, int startAge);

    // Getters
    std::string getSpecies() const;
    std::string getType() const;
    bool isFemale() const;
    int getAge() const;

    void ageOneMonth();                 // add one month to age
    int getBirthInterval() const;       // months between births
    bool readyToGiveBirth() const;      // is she "in heat" this month?
    std::string describe() const;       // "I am a female cat, 12 months old"
};

// Works out the type (mammal/bird/fish) from a species name
std::string typeOfSpecies(std::string speciesName);

#endif
