/**
 * @file
 * @brief Ancient God related functions.
**/
#include "AppHdr.h"

#include "ancient-god.h"

#include <cstdint>
#include <cstring>
#include "random.h"
#include "message.h"
#include "player.h"
#include "ability-type.h"
#include "god-passive.h"
#include "ancient-god-config-types.h"
#include "religion.h"

#include "ancient-god-data.h"

// Initial generation functions - produces key values that are
// looked up by lookup functions below. Effectively these key
// functions are the randomisation, and the lookup functions are
// what translates those keys into useable strings/abilities etc

// Generation functions are called once at ng-init
// TODO: make this deterministic on seed value
uint8_t generate_ancient_god_name_key()
{
    return you.game_seed%NUM_AG_NAMES;
}

uint8_t generate_ancient_god_passive_key()
{
    return 1; // testing
    return you.game_seed%NUM_AGP;
}

uint8_t generate_ancient_god_small_key()
{
    return you.game_seed%NUM_AGAS;
}

uint8_t generate_ancient_god_cap_key()

{
    return you.game_seed%NUM_AGAC;
}

uint8_t generate_ancient_god_mut_key()
{
    return you.game_seed%NUM_AGM;
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
    return ag_name_data[you.ancient_god_name_key];
}

// don't store a key for this, just generate deterministically from the
// other keys. Probably a better way of doing this.
// Need to +1 to avoid multiplying by 0
const char* ancient_god_title()
{
    int ancient_god_title_key = ((you.ancient_god_name_key + 1)
        * (you.ancient_god_passive_key + 1)
        * (you.ancient_god_small_key + 1)
        * (you.ancient_god_cap_key + 1)) % NUM_AG_TITLES;
    ASSERT(ancient_god_title_key >= 0);
    ASSERT(ancient_god_title_key < NUM_AG_TITLES);
    // 0 to NUM_AG_TITLES-1 i.e. an index into the data array we can use.

    return ag_title_data[ancient_god_title_key];
}


/********************************************************
*   Functions relating to ancient god passives
********************************************************/

// use passive generation key to return the passive for this ancient god
passive_t ancient_god_passive()
{
//    mprf("Looking up passive abil with key: %d",you.ancient_god_passive_key);
    return ag_passive_data[you.ancient_god_passive_key].passive;
}

// i.e. "ancient god now allows you to <return val>"
const char* ancient_god_passive_description_short()
{
//    mprf("Looking up passive desc with key: %d",you.ancient_god_passive_key);
    return ag_passive_data[you.ancient_god_passive_key].short_description;
}

// i.e. in the ^ menu, how should the full description of the passive appear?
// e.g. "You are resistant to fire"
string ancient_god_passive_description_long()
{
    if (you.piety<piety_breakpoint(ancient_god_passive_breakpoint))
    {
        return "Gain more piety to discover this passive ability.\n";
    }
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
    return ag_ability_small_data[you.ancient_god_small_key].short_description;
}

const char* ancient_god_small_ability_description_long()
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
    return ag_ability_cap_data[you.ancient_god_cap_key].short_description;
}

const char* ancient_god_cap_ability_description_long()
{
    return ag_ability_cap_data[you.ancient_god_cap_key].long_description;
}

// Tracking via enums with these compile checks to ensure we always generate
// a valid set of ancient god passive/abilities etc which is dynamic based
// on the number of existing possibilities for those.
COMPILE_CHECK(ARRAYSZ(ag_passive_data)==NUM_AGP);
COMPILE_CHECK(ARRAYSZ(ag_ability_small_data)==NUM_AGAS);
COMPILE_CHECK(ARRAYSZ(ag_ability_cap_data)==NUM_AGAC);
COMPILE_CHECK(ARRAYSZ(ag_mut_data)==NUM_AGM);
COMPILE_CHECK(ARRAYSZ(ag_name_data)==NUM_AG_NAMES);
COMPILE_CHECK(ARRAYSZ(ag_title_data)==NUM_AG_TITLES);