/**
 * @file
 * @brief Ancient God related functions.
**/

#include "ancient-god.h"
#include <cstdint>
#include <cstring>
#include "random.h"
#include "player.h"
#include "ability-type.h"
#include "god-passive.h"

// Initial generation functions - produces key values that are
// looked up by lookup functions below. Effectively these key
// functions are the randomisation, and the lookup functions are
// what translates those keys into useable strings/abilities etc

// Generation functions are called once at ng-init

int NUM_ANCIENT_GOD_PASSIVES = 8;
int NUM_ANCIENT_GOD_SMALLS = 8;
int NUM_ANCIENT_GOD_CAPS = 8;

uint8_t generate_ancient_god_name_key()
{
    return random_choose(1,2,3);
}

uint8_t generate_ancient_god_passive_key()
{
    return random_range(1,NUM_ANCIENT_GOD_PASSIVES);
}

uint8_t generate_ancient_god_small_key()
{
    return random_range(1,NUM_ANCIENT_GOD_SMALLS);
}

uint8_t generate_ancient_god_cap_key()
{
    return random_range(1,NUM_ANCIENT_GOD_CAPS);
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
string ancient_god_name()
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

string ancient_god_name_extra()
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

// use passive generation key to return the passive for this ancient god
passive_t ancient_god_passive()
{
    return passive_t::umbra;
}

// i.e. "ancient god now allows you to <return val>"
const char* ancient_god_passive_description_short()
{
    switch(ancient_god_passive())
    {
        case passive_t::umbra:
            return "create an umbra around yourself\n";
        default:
            return "your god passive descriptions are broken, please report this bug (1)\n";
    }
    return "your god passive descriptions are broken, please report this bug (2)\n";
}

// i.e. in the ^ menu, how should the full description of the passive appear?
// e.g. "You are resistant to fire"
string ancient_god_passive_description_long()
{
    switch(ancient_god_passive())
    {
        case passive_t::umbra:
            return "You have a dark and eerie umbra\n";
        default:
            return "your god passive descriptions are broken, please report this bug (3)\n";
    }
    return "your god passive descriptions are broken, please report this bug (4)\n";
}

ability_type ancient_god_small_ability()
{
    return ABIL_OKAWARU_HEROISM;
}

ability_type ancient_god_cap_ability()
{
    return ABIL_QAZLAL_DISASTER_AREA;
}