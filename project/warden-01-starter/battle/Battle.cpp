// COMP 2450 — Warden of the Foundations (Midterm 1)
// battle/Battle.cpp — YOU build the body.
//
// =====================================================================
// REQUIRED MECHANICS
// =====================================================================
// Re-read the public midterm page if anything below is unclear.
//
// MENU (at minimum, four options):
//   1. Attack          — damage the warden; warden retaliates that turn
//   2. Use item        — pick from inventory; effect; turn ends
//   3. Inspect Warden  — print warden's state; FREE action — turn does
//                        NOT end
//   4. Flee            — leave; gate stays closed
//
// END:
//   Victory  when wardenHP <= 0
//   Defeat   when playerHP <= 0
//   Fled     when the player chose Flee
//
// =====================================================================
// FLOOR-TIED REQUIREMENTS — ALL FOUR MUST APPEAR IN YOUR CODE
// =====================================================================
//
//   F0 (ADT)         — the available actions on a turn must live in a
//                      container of your choice. ABOVE the declaration,
//                      in a comment, name the ADT and defend it in one
//                      sentence (the same kind of defence Trial I Q1
//                      practiced — but this is a different collection
//                      than Q1's items menu; the right ADT may differ).
//
//   F1 (search)      — the Use-item branch MUST call findByName<Item>
//                      against hero.inventory to look up the item the
//                      player typed.
//
//   F2 (sort)        — when the items menu is displayed, sort the
//                      inventory at display time with a comparator
//                      (std::sort or your Floor 2 sortInventory). Pick
//                      a criterion (value — the healing-power stand-in
//                      — weight, or name) and document it in a comment.
//
//   F3 (templates +  — invalid menu input must `throw BattleException`
//      exceptions)     (ready skeleton in Battle.h) — or BagException
//                      where a genuinely bad index is the fault —
//                      caught INSIDE the battle loop so the player gets
//                      another prompt — not a crash, not an exit.
//
// =====================================================================
// WHAT THE GRADER WILL DO
// =====================================================================
//   1. cmake --build the project. If it does not compile, the Warden
//      has won by default.
//   2. Type `battle warden`, play through to BOTH a victory and a
//      defeat (or attempt to — items + RNG permitting).
//   3. Type a deliberately invalid menu choice (e.g., "9" for a
//      4-option menu). The game must NOT crash; it must re-prompt.
//   4. Open this file and find each of the four Floor ties. They must
//      be REAL — i.e., the menu actually runs through your container,
//      Use-item actually goes through findByName, the items menu is
//      actually sorted, the throw actually fires on bad input.

#include "Battle.h"

#include <iostream>
#include <string>

#include "../hero/Bag.h"            // findByName<T>()
#include "../hero/BagException.h"   // Bag<T>
#include "../hero/Item.h"           // BagException
#include "../bestiary/Search.h"     // Item
#include "../hero/Sort.h"           // sortInventory()

namespace dungeon {

namespace {

// =====================================================================
// Tunable battle parameters. Edit to taste; document any tuning in
// encounter-notes.md so the grader knows what to expect.
// =====================================================================
constexpr int kPlayerStartHP   = 30; 
constexpr int kWardenStartHP   = 50;
constexpr int kPlayerAttackDmg = 6;   // damage per Attack action
constexpr int kWardenAttackDmg = 4;   // warden's retaliation damage
// constexpr means these values are combile-time constants
// k prefix --> means constant as a naming convension

// enum creates a set of named choices
// enum class --> allows us to keep our names scoped
enum class MenuAction {
    Attack,
    UseItem,
    Inspect,
    Flee
};
// {1, "Attack", MenuAction::Attack}

struct MenuOption {
    int number; // number typed by player
    std::string label; // text displayed by the menu
    MenuAction action; // action performed by our program
};

// extra functionality to clean up our main() function
void printMenu(Bag<MenuOption>& menu, int playerHP, int wardenHP) {
    std::cout << "\n -- Your turn -- your hp:" << playerHP
    << "    Warden hp: " << wardenHP << "\n";

    for (std::size_t i = 0; i < menu.size(); ++i){
        // Bag will overload our operator[], allowing our menu[i] 
        // to retrieve our MenuOption at index i
        std::cout << "    "
        << menu[i].number
        << ". "
        << menu[i].label
        << "\n";
    }

    std::cout << " > ";

}

MenuAction readmenuChoice(const Bag<MenuOption>& menu){
    std::string line;

    // Getline will get the entire line up to the enter key
    // if the getline fails, std input may have been closed, in which case we exit the program
    // If there's a problem... flee with extra fleeee!
    if (!std::getline(std::cin, line)){
        return MenuAction::Flee;
    }

    int n = -1;

    try {
        n = std::stoi(line);
    }
    catch (...) {
        // catch any exception type
        // replace our low-level stoi excpt with a domain-specific BattleException
        throw BattleException(
            "'" + line + "' is not a menu number (enter 1 to "
            + std::to_string(menu.size()) + ")"
        );
    }

    for (std::size_t i = 0; i < menu.size(); ++i){
        if (menu[i].number == n) {
            return menu[i].action;
        }
    }

    // say the input was numeric, but wasn't in menu options:
    throw BagException(
        static_cast<std::size_t>(n),
        menu.size()
    );
}

    // handle the player's "use item" action:
    // Hero& will give the function access to the original hero object instead of a copy
    void UseItem(Hero& hero, int& playerHP) {
        // handle the case that the inventory is empty:
        if (hero.inventory.empty()){
            std::cout << "Your satchel is empty.\n";
            return;
        }

        sortInventory(hero, "value desc");
        std::cout << "Choose an item by name:\n";
        printInventory(hero);
        std::cout << " > ";

        std::string name;

        // || short-circuit 
        // 1. try to read the line
        // 2. if successful, then i will check whether the line is empty
        // if either condition is true, the player does nothing

        if (!std::getline(std::cin, name) || name.empty()){
            std::cout << "you hesitated.\n";
            return;
        }

        // findByName<Item> <-- function-template specialization <Item> will 
        // tell the compiler this search will operate on Item objects
        const Item* it = findByName<Item>(hero.inventory, name);

        // a nullptr converts to false by default
        if (!it) { throw BattleException(
                "no item found '" + name + "' in your satchel"
            );
        }

        // if you say potion, healing potion, we want all those to count
        if (it->name.find("otion") != std::string::npos){
            // heal 12 hp as default, but we also don't need our healing to exxceed our max health

            playerHP = std::min(
                playerHP + 12,
                kPlayerStartHP
            );
            std::cout << " You drink "
            << it->name
            << ". HP -> "
            << playerHP
            << ".\n";
        }
        else {
            std::cout << " You ready "
            << it->name
            << " - but it is not a consumable.\n";
        }
}

}  // anonymous namespace

BattleOutcome runWardenBattle(Hero& hero) {
    // TODO — write the boss battle. Suggested outline (yours to refactor):
    //
    //   int playerHP = kPlayerStartHP;
    //   int wardenHP = kWardenStartHP;
    //
    //   while (playerHP > 0 && wardenHP > 0) {
    //       print state (HPs, last action — your choice).
    //
    //       try {
    //           show menu (using your F0 container of actions).
    //           read input.
    //           if invalid → throw BattleException(...) [F3 — throw].
    //           dispatch on the action:
    //               Attack:    wardenHP -= kPlayerAttackDmg;
    //                          if wardenHP > 0, playerHP -= kWardenAttackDmg.
    //               Use item:  std::sort(hero.inventory.begin(),
    //                                    hero.inventory.end(),
    //                                    yourComparator)             [F2].
    //                          show sorted menu, read item name.
    //                          const Item* it = findByName<Item>(
    //                              hero.inventory, name);             [F1]
    //                          if (!it) throw BattleException(...);   [F3]
    //                          apply effect (heal? buff next attack? …).
    //                          end turn.
    //               Inspect:   print warden state. FREE — do NOT end turn.
    //               Flee:      return BattleOutcome::Fled.
    //       }
    //       catch (const std::exception& e) {                        [F3 — catch]
    //           std::cout << "  " << e.what() << "  Try again.\n";
    //           continue;   // re-prompt; turn does NOT advance
    //       }
    //   }
    //
    //   return wardenHP <= 0 ? BattleOutcome::Victory
    //                        : BattleOutcome::Defeat;
    //
    // Decompose into helpers however you want. The contract main.cpp
    // depends on is just runWardenBattle(Hero&).
    //
    // Replace the placeholder body below.

    // (void)hero;
    // std::cout << "  (Battle scaffold — runWardenBattle is not yet written.)\n"
    //           << "  (Open battle/Battle.cpp and follow the TODOs.)\n";
    // return BattleOutcome::Fled;

    int playerHP = kPlayerStartHP;
    int wardenHP = kWardenStartHP;

    // create a bag specialized to store our menuOption objects
    Bag<MenuOption> menu;
    menu.push_back({1, "Attack", MenuAction::Attack });
    menu.push_back({2, "Use item", MenuAction::UseItem});
    menu.push_back({3, "Inspect warden", MenuAction::Inspect});
    menu.push_back({4, "Flee", MenuAction::Flee});

    // continue the battle ONLY while both participants are alive
    while (playerHP > 0 && wardenHP > 0){
        // print state (HPs, last action - your choice)

        try {
            printMenu(menu, playerHP, wardenHP);
            //  readMenuChoice returns a MenuAction
            // we'll use a switch state to select the corresponding block of code
            switch (readmenuChoice(menu)){
                case MenuAction::Attack:{
                    wardenHP -= kPlayerAttackDmg;
                    std::cout << "you strike for "
                    << kPlayerAttackDmg
                    << ". Warden HP -> "
                    << std::max(wardenHP,0)
                    << ".\n";

                    // is warden dead?

                    if (wardenHP > 0){
                        playerHP -= kWardenAttackDmg;

                        std::cout << "The warden retaliates for "
                            << kWardenAttackDmg
                            << ". Your HP -> "
                            << std::max(playerHP, 0)
                            << ".\n";
                    }

                    break;
                }

                case MenuAction::UseItem: {
                    // let's use an item
                    UseItem(hero, playerHP);
                    
                    // useing an item does consume our turn
                    // the warden will attack, assuming we are both alive.
                    if (wardenHP > 0 && playerHP > 0){
                        playerHP -= kWardenAttackDmg;

                        std::cout << "The warden stikes while you fumble. Your HP -> "
                            << std::max(playerHP, 0)
                            << ".\n";
                    }
                    break;
                }

                case MenuAction::Inspect: {
                    std::cout << "Warden of the Foundations. HP -> "
                        << wardenHP
                        << " / "
                        << kWardenStartHP
                        << ". No visible weakness (free action).\n";
                    
                    break;
                }

                case MenuAction::Flee: {
                    // Return to immediately exit the function
                    return BattleOutcome::Fled;
                    break;
                }
            }
        }

        catch (const std::exception& e){
            std::cout << "  " << e.what() << "  Try again.\n";
            continue;
        }

    }

    // condition ? valud_if_true : value_if_false
    return wardenHP <= 0 
                    ? BattleOutcome::Victory
                    : BattleOutcome::Defeat;
}

}  // namespace dungeon
