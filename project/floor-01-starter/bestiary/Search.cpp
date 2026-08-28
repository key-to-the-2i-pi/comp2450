// COMP 2450 — Floor 1 starter
// bestiary/Search.cpp — YOU implement this file.
//
// Four functions to write. Read Search.h for their contracts.
//
// The big idea this week: the same question ("is X in the list?") has
// three implementations with three VERY different Big-O costs. You will
// write all three, race them in `benchmark`, and then argue — in a commit
// message — which one the rest of the game should use. The code is easy.
// The *thinking* is the point.
//
// Tips for the Unsorted Lich:
//   * binarySearch (and binarySearchRecursive) only work if the bestiary
//     is sorted by name.
//   * main.cpp already calls sortBestiary() right after loading,
//     so you can assume the precondition holds when these run.
//   * If you ever doubt, scan the vector and assert it is sorted.
//
// Submit when:  `search Goblin` returns the goblin's stats and `search Ghost`
//               reports "no such creature." Then run `benchmark` and capture
//               the output for your lab-notes.md.

#include "Search.h"


namespace dungeon {

const Monster* linearSearch(const std::vector<Monster>& bestiary,
                            const std::string&         name) {

        for (const auto& m: bestiary){
            if (m.name == name) return &m;

            // == byte-for-byte equality
            // here, & is address-of operator
            // since m is a reference into a vector
            // &m is a pointer to that slot in the vector;
        }
        return nullptr; // c++ typed null pointer
        // NULL macro --> an int 0 in disguise
    }
    // TODO Floor 1 (Mon): walk every entry; return its address when name matches.
    //                     If you reach the end without a match, return nullptr.
    //
    // Think before you type:
    //   - You return `const Monster*` (a pointer into the vector), NOT
    //     `Monster` (a copy). Why a pointer? What would you even return
    //     from a "copy" version when the name is not found?
    //   - In the range-for loop, `for (auto m : bestiary)` makes a COPY
    //     of each monster each iteration. `for (const auto& m : bestiary)`
    //     does not. Which do you want — and why does the difference matter
    //     more for a `Monster` than for an `int`?
    //   - How do you take the address of the element you're looking at?
    //     (Two common idioms. Pick whichever makes your loop read cleanly.)



const Monster* binarySearch(const std::vector<Monster>& bestiary,
                            const std::string&         name) {
    

    std::size_t low = 0;
    std::size_t high = bestiary.size();

    while (low < high){
        std::size_t mid = low + ((high - low) / 2);

        const std::string& here = bestiary[mid].name;

        if (here == name){
            return &bestiary[mid];
        }
        else if (here < name){
            low = mid + 1;
        }
        else {
            high = mid;
        }
    }
                                    
    return nullptr;
}

namespace {
    const Monster* binSearchRec(
        const std::vector<Monster>& bestiary,
        const std::string& name,
        std::size_t low,
        std::size_t high
    ) {
        // Base case:
        if (low >= high) return nullptr;
        
        // Recursive Case:
        std::size_t mid = low + ((high - low) / 2);
        const std::string& here = bestiary[mid].name;

        if (here == name) return &bestiary[mid];
        else if (here < name){
            return binSearchRec(bestiary, name, mid + 1, high);
        }

        else {return binSearchRec(bestiary, name, low, mid);}
    }
}



const Monster* binarySearchRecursive(const std::vector<Monster>& bestiary,
                                     const std::string&         name) {
    return binSearchRec(bestiary, name, 0, bestiary.size());
}

const Monster* findMonster(const std::vector<Monster>& bestiary,
                           const std::string&         name) {
    
    return binarySearchRecursive(bestiary, name);
}

}

