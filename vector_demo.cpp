/*
c++ vector is a lot like a python list
holds a sequence of things
we can add, access by position, loop, etc.
*/

#include <iostream>
#include <string>
#include <vector> // std::vector

using namespace std;

// vector container
// holds a collection of things

// Step 1: create a struct

struct Monster {
    std::string name;
    int hp;
};
// Struct lets us bundle related pieces of information together

// python dataclasses would be similar
/*
@dataclass
class Monster:
    name: str
    hp: int

*/

// interested in storing students, custumers, monsters, bank transations, web pages, graph vertices

int main(){
    vector<Monster> bestiary;
    // I want a vector.. a vector of what?
    // A vector that stores monsters
    // similar to templates...(subtle foreshadowing)
    
    // std::vector<std::string>
    //std::vector<int>

    // python --> bestiary = []

    // c++'s strong type system

    // add some monsters to the bestiary

    bestiary.push_back({"Goblin", 8});
    bestiary.push_back({"skeleton", 10});
    bestiary.push_back({"Wraith", 14});

    // push_back -> put this new thing at the back of the vector
    // python would do besiary.append()

    // vector is a dynamically growing array

    cout << "The bestiary holds "
        << bestiary.size()
        << " monsters.\n";

    // print out every monster in bestiary
    for (const auto& m: bestiary){ // take the reference to every item in bestiary and, without changing the value, store it in local variable m.
            cout << "  "
                << m.name
                << " (HP "
                << m.hp
                << ")\n";
    }

    // find and print the first monster in the bestiary
    cout << "\nFirst by index: "
        << bestiary[0].name
        << endl;

    // bestiary[bestiary.size() - 1] <-- show the value in the last element
    cout << "Last monster: "
        << bestiary.back().name
        << endl;

    // Goblin took damage
    bestiary[0].hp -= 2;

    cout << "\nThe Goblin took 2 damage! "
        << "Goblin HP: "
        << bestiary[0].hp
        << endl;

    return 0;
}