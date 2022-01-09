/**
 * @file
 * @brief Ancient God related functions.
**/
#include "AppHdr.h"

#include "ancient-god.h"

#include <cstdint>
#include <cstring>
#include "random.h"
#include "player.h"
#include "ability-type.h"
#include "god-passive.h"
#include "ancient-god-config-types.h"

const int NUM_ANCIENT_GOD_SMALLS = 8;
const int NUM_ANCIENT_GOD_CAPS = 8;

struct agod_passive
{
    passive_t passive;
    string short_description;
    string long_description;
};

struct agod_ability_small
{
    ability_type ability;
    string short_description;
    string long_description;
};

struct agod_ability_cap
{
    ability_type ability;
    string short_description;
    string long_description;
};

struct agod_mut
{
    mutation_type mutation;
};

#include "ancient-god-data.h"

// Initial generation functions - produces key values that are
// looked up by lookup functions below. Effectively these key
// functions are the randomisation, and the lookup functions are
// what translates those keys into useable strings/abilities etc

// Generation functions are called once at ng-init
uint8_t generate_ancient_god_name_key()
{
    return random_range(1,3);
}

uint8_t generate_ancient_god_passive_key()
{
    return random_range(0,NUM_AGP);
}

uint8_t generate_ancient_god_small_key()
{
    return random_range(0,NUM_AGAS);
}

uint8_t generate_ancient_god_cap_key()
{
    return random_range(0,NUM_AGAC);
}

uint8_t generate_ancient_god_mut_key()
{
    return random_range(0,NUM_AGM);
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

/********************************************************
*   Functions relating to ancient god passives
********************************************************/

// use passive generation key to return the passive for this ancient god
passive_t ancient_god_passive()
{
    return ag_passive_data[you.ancient_god_passive_key].passive;
}

// i.e. "ancient god now allows you to <return val>"
const char* ancient_god_passive_description_short()
{
    return ag_passive_data[you.ancient_god_passive_key].short_description.c_str();
}

// i.e. in the ^ menu, how should the full description of the passive appear?
// e.g. "You are resistant to fire"
string ancient_god_passive_description_long()
{
    return ag_passive_data[you.ancient_god_passive_key].long_description;
}


/********************************************************
*   Functions relating to ancient god smaller abilities
********************************************************/

ability_type ancient_god_small_ability()
{
    return ag_ability_small_data[you.ancient_god_small_key].ability;
}

const char* ancient_god_small_ability_description_short()
{
    return ag_ability_small_data[you.ancient_god_small_key].short_description.c_str();
}

string ancient_god_small_ability_description_long()
{
    return ag_ability_small_data[you.ancient_god_small_key].long_description;
}


/********************************************************
*   Functions relating to ancient god capstone abilities
********************************************************/

ability_type ancient_god_cap_ability()
{
    return ag_ability_cap_data[you.ancient_god_cap_key].ability;
}

const char* ancient_god_cap_ability_description_short()
{
    return ag_ability_cap_data[you.ancient_god_cap_key].short_description.c_str();
}

string ancient_god_cap_ability_description_long()
{
    return ag_ability_cap_data[you.ancient_god_cap_key].long_description;
}


COMPILE_CHECK(ARRAYSZ(ag_passive_data)==NUM_AGP);
COMPILE_CHECK(ARRAYSZ(ag_ability_small_data)==NUM_AGAS);
COMPILE_CHECK(ARRAYSZ(ag_ability_cap_data)==NUM_AGAC);
COMPILE_CHECK(ARRAYSZ(ag_mut_data)==NUM_AGM);