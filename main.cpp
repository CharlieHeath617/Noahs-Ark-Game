// main.cpp
// Noah's Ark Inventory Application (CIS 142)
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Animal.h"

using namespace std;

const int MAX_ANIMALS = 50;          // ark capacity
const int WIN_MONTH = 36;            // survive this long to win
const int DEATH_CHANCE_PER_1000 = 5; // each animal has a 0.5% chance of dying each month
const int NUM_SPECIES = 6;
const string SPECIES[NUM_SPECIES] = {"cat", "dog", "goldfish", "shark", "eagle", "parakeet"};
const string SAVE_FILE = "ark_save.txt";

// ---------- Setup ----------

// Start with one male and one female of each species, all 0 months old
void setupArk(vector<Animal>& ark) {
    ark.clear();
    for (int i = 0; i < NUM_SPECIES; i++) {
        ark.push_back(Animal(SPECIES[i], true, 0));
        ark.push_back(Animal(SPECIES[i], false, 0));
    }
}

// ---------- Counting helpers ----------

int countSpecies(const vector<Animal>& ark, string species) {
    int count = 0;
    for (int i = 0; i < (int)ark.size(); i++) {
        if (ark[i].getSpecies() == species) count++;
    }
    return count;
}

int countSex(const vector<Animal>& ark, string species, bool female) {
    int count = 0;
    for (int i = 0; i < (int)ark.size(); i++) {
        if (ark[i].getSpecies() == species && ark[i].isFemale() == female) count++;
    }
    return count;
}

// ---------- Menu and inventory ----------

void showMenu(int months) {
    cout << "\nYou have been on the ark for " << months << " months. Would you like to:\n";
    cout << "1 - Let another month pass\n";
    cout << "2 - Check inventory\n";
    cout << "3 - Quit\n";
    cout << "4 - Save game\n";
    cout << "Choice: ";
}

void showInventory(const vector<Animal>& ark) {
    cout << "\n--- Inventory (" << ark.size() << "/" << MAX_ANIMALS << " animals) ---\n";
    for (int i = 0; i < (int)ark.size(); i++) {
        cout << "Animal #" << (i + 1) << ": " << ark[i].describe() << endl;
    }
    cout << "\nSpecies totals: ";
    for (int i = 0; i < NUM_SPECIES; i++) {
        cout << SPECIES[i] << "=" << countSpecies(ark, SPECIES[i]) << "  ";
    }
    cout << endl;
}

// ---------- Month passing ----------

// Ages animals, lets some die, then lets females give birth
void passMonth(vector<Animal>& ark, int& months) {
    months++;
    cout << "\nA month passes...\n";

    // 1. Age everyone
    for (int i = 0; i < (int)ark.size(); i++) {
        ark[i].ageOneMonth();
    }

    // 2. Random deaths (loop backwards so erasing is safe)
    for (int i = (int)ark.size() - 1; i >= 0; i--) {
        if (rand() % 1000 < DEATH_CHANCE_PER_1000) {
            cout << "Sadly, a " << (ark[i].isFemale() ? "female " : "male ")
                 << ark[i].getSpecies() << " has died.\n";
            ark.erase(ark.begin() + i);
        }
    }

    // 3. Births. Only loop over the animals that existed before births began.
    int originalCount = (int)ark.size();
    for (int i = 0; i < originalCount; i++) {
        if (ark[i].readyToGiveBirth()) {
            if ((int)ark.size() >= MAX_ANIMALS) {
                cout << "The ark is full! A " << ark[i].getSpecies()
                     << " could not give birth.\n";
            } else {
                bool babyIsFemale = (rand() % 2 == 0);
                string species = ark[i].getSpecies();
                ark.push_back(Animal(species, babyIsFemale, 0));
                cout << "A baby " << species << " was born!\n";
            }
        }
    }
}

// ---------- Game over check ----------

// Returns true (and explains why) if the game is lost
bool isGameLost(const vector<Animal>& ark) {
    for (int i = 0; i < NUM_SPECIES; i++) {
        string s = SPECIES[i];
        if (countSpecies(ark, s) == 0) {
            cout << "\nThe " << s << " has gone extinct! GAME OVER.\n";
            return true;
        }
        // Without at least one male and one female, a species can't reproduce
        if (countSex(ark, s, true) == 0 || countSex(ark, s, false) == 0) {
            cout << "\nThe " << s << " can no longer reproduce! GAME OVER.\n";
            return true;
        }
    }
    return false;
}

// ---------- Save / Load ----------

bool saveGame(const vector<Animal>& ark, int months) {
    ofstream out(SAVE_FILE);
    if (!out) return false;
    out << months << endl;
    out << ark.size() << endl;
    for (int i = 0; i < (int)ark.size(); i++) {
        out << ark[i].getSpecies() << " " << ark[i].isFemale() << " "
            << ark[i].getAge() << endl;
    }
    return true;
}

bool loadGame(vector<Animal>& ark, int& months) {
    ifstream in(SAVE_FILE);
    if (!in) return false;
    int count;
    in >> months >> count;
    ark.clear();
    for (int i = 0; i < count; i++) {
        string species;
        bool female;
        int age;
        in >> species >> female >> age;
        ark.push_back(Animal(species, female, age));
    }
    return true;
}

// ---------- Main ----------

int main() {
    srand((unsigned)time(0));   // seed random numbers

    vector<Animal> ark;
    int months = 0;

    cout << "=== Noah's Ark Inventory ===\n";
    cout << "Survive " << WIN_MONTH << " months with every species alive and able to breed!\n";

    // Offer to continue a saved game
    ifstream test(SAVE_FILE);
    bool loaded = false;
    if (test) {
        test.close();
        cout << "A saved game was found. Load it? (y/n): ";
        char answer;
        cin >> answer;
        if (answer == 'y' || answer == 'Y') {
            loaded = loadGame(ark, months);
            if (loaded) cout << "Game loaded.\n";
        }
    }
    if (!loaded) {
        setupArk(ark);
    }

    int choice = 0;
    while (choice != 3) {
        showMenu(months);
        cin >> choice;

        if (cin.fail()) {          // handle non-number input
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Please enter a number from the menu.\n";
            choice = 0;
        } else if (choice == 1) {
            passMonth(ark, months);
            if (isGameLost(ark)) {
                showInventory(ark);
                break;
            }
            if (months >= WIN_MONTH) {
                showInventory(ark);
                cout << "\nYou made it to month " << WIN_MONTH
                     << " with every species intact. YOU WIN!\n";
                break;
            }
        } else if (choice == 2) {
            showInventory(ark);
        } else if (choice == 4) {
            if (saveGame(ark, months)) cout << "Game saved.\n";
            else cout << "Could not save the game.\n";
        } else if (choice == 3) {
            cout << "Save before quitting? (y/n): ";
            char answer;
            cin >> answer;
            if (answer == 'y' || answer == 'Y') {
                saveGame(ark, months);
                cout << "Game saved.\n";
            }
            cout << "Goodbye!\n";
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}
