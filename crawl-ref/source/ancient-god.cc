/**
 * @file
 * @brief Ancient God related functions.
**/

#include "ancient-god.h"
#include <cstdint>
#include <cstring>
#include "random.h"
#include "player.h"

// Initial generation functions - produces key values that are
// looked up by lookup functions below. Effectively these key
// functions are the randomisation, and the lookup functions are
// what translates those keys into useable strings/abilities etc

// Generation functions are called once at ng-init
uint8_t generate_ancient_god_name_key()
{
    return random_choose(1,2,3);
}

uint8_t generate_ancient_god_passive_key()
{
    if(coinflip())
    {
        return 1;
    } else 
    {
        return 2;
    }
}

uint8_t generate_ancient_god_small_key()
{
    return 1;
}

uint8_t generate_ancient_god_cap_key()
{
    return 1;
}

uint8_t generate_ancient_god_like_key()
{
    return 1;
}

uint8_t generate_ancient_god_dislike_key()
{
    return 1;
}

// Lookup function - all take generated ints and translate those into
// which god you've randomised for your run!
string god_name_ancient()
{
    // TODO a lot more in here - hash the key and then do some generation
    // and some hardcoding
    switch (you.ancient_god_name_key)
    {
        case 1:
            return "Coolgod";
        case 2:
            return "Chillgod";
        case 3:
            return "Testgod";
    }
    return "Bug! Please report: " + you.ancient_god_name_key;
}

string god_name_ancient_extra()
{
    // TODO base this on other god characteristics
    switch (you.ancient_god_name_key)
    {
        case 1:
            return " the coolest";
        case 2:
            return " the chillest";
        case 3:
            return " the testest";
    }
    return "Bug! Please report: " + you.ancient_god_name_key;
}