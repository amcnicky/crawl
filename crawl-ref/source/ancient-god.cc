/**
 * @file
 * @brief Ancient God related functions.
**/
#include "AppHdr.h"

#include "ancient-god.h"

#include "act-iter.h"
#include <cstdint>
#include <cstring>
#include "random.h"
#include "message.h"
#include "mon-tentacle.h"
#include "player.h"
#include "ability-type.h"
#include "god-passive.h"
#include "religion.h"
#include "god-companions.h"
#include "mon-util.h"

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
    return you.game_seed%NUM_AGP;
}

uint8_t generate_ancient_god_small_key()
{
    return 2; //testing
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

string sidekick_name()
{
    ASSERT(you.props.exists(AG_SIDEKICK_NAME_KEY));
    return you.props[AG_SIDEKICK_NAME_KEY].get_string();
}

static string _make_sidekick_name()
{
    return ag_spriggan_names[you.game_seed%NUM_AG_SPRIGGAN_NAMES];
}

/// Setup when gaining a spriggan sidekick.
void setup_ag_spriggan_sidekick()
{
    // initial setup.
    if (!you.props.exists(AG_SIDEKICK_NAME_KEY))
    {
        you.props[AG_SIDEKICK_NAME_KEY] = _make_sidekick_name();
    }
}

mid_t ag_sidekick()
{
    for (auto &entry : companion_list)
        if (mons_is_ag_sidekick(entry.second.mons.mons.type))
            return entry.first;
    return MID_NOBODY;
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

static string _ancient_god_aspect()
{
    // will change mid-game if array is modified by an update
    // not ideal, but its also just a description
    return ag_desc_aspect_data[you.game_seed%ARRAYSZ(ag_desc_aspect_data)];
}

static string _ancient_god_faction()
{
    // will change mid-game if array is modified by an update
    // not ideal, but its also just a description
    return ag_desc_faction_data[you.game_seed%ARRAYSZ(ag_desc_faction_data)];
}

static string _apply_dynamic_godname(string desc)
{
    return replace_all(desc,"GODNAME",ancient_god_name());
}

static string _apply_dynamic_aspect(string desc)
{
    return replace_all(desc,"ASPECT",_ancient_god_aspect());
}

static string _apply_dynamic_faction(string desc)
{
    return replace_all(desc,"FACTION",_ancient_god_faction());
}

string apply_ag_dynamics(string desc)
{
    return _apply_dynamic_godname(
        _apply_dynamic_aspect(
            _apply_dynamic_faction(desc)
        )
    );
}

string ag_desc_introduction()
{
    return apply_ag_dynamics(
        ag_desc_introduction_data[you.game_seed%ARRAYSZ(ag_desc_introduction_data)]);
}

string ag_desc_how_god_was()
{
    return apply_ag_dynamics(
    ag_desc_how_god_was_data[you.game_seed%ARRAYSZ(ag_desc_how_god_was_data)]);
}

string ag_desc_god_fall()
{
    return apply_ag_dynamics(
        ag_desc_god_fall_data[you.game_seed%ARRAYSZ(ag_desc_god_fall_data)]);
}

string ag_desc_god_now()
{
    return apply_ag_dynamics(
        ag_desc_god_now_data[you.game_seed%ARRAYSZ(ag_desc_god_now_data)]);
}

string ag_desc_powers()
{
    return _apply_dynamic_godname(
        "As an ancient god, knowledge of GODNAME's powers has been lost in"
        " all but the most devout corners of the world. Gaining piety with"
        " GODNAME will allow you to rediscover and earn the benefits of"
        " worship."
    );
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

bool ancient_god_passive_active(passive_t passive)
{
    return you_worship(GOD_ANCIENT)
        && passive == ancient_god_passive()
        && you.piety >= piety_breakpoint(ancient_god_passive_breakpoint);
}

// an example of how certain passives may have more than one potential subtype
// TODO: there's likely a more extensible way of doing this.
ag_passive_threatening_boost_subtype get_threatening_boost_subtype()
{
    return ag_passive_threatening_boost_subtype::ST_INT; //testing
    // this is a type cast, not a function
    return ag_passive_threatening_boost_subtype(you.game_seed%NUM_ST);
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
    if (you.piety<piety_breakpoint(ancient_god_passive_breakpoint)
        || !(you_worship(GOD_ANCIENT)))
    {
        return "Gain more piety to discover this passive ability.\n";
    } else if (ancient_god_passive()==passive_t::threatening_boost)
    {
        // dynamic treatment of passives with subtypes
        return make_stringf(
            " grants a boost of %s in threatening situations\n",
            desc_subtype_of_ancient_god_passive().c_str());
    }
    // if no subtype just return the static desc per ag_passive_data
    return ag_passive_data[you.ancient_god_passive_key].long_description;
}

string desc_freq_of_ancient_god_passive()
{
    string returnString = "";
    passive_t current_passive = ancient_god_passive();
    switch (current_passive)
        {
            case passive_t::potion_refills:
            {
                if (you.piety < piety_breakpoint(1))
                {
                    returnString += " very rarely";
                } else if (you.piety < piety_breakpoint(2))
                {
                    returnString += " rarely";
                } else if (you.piety < piety_breakpoint(3))
                {
                    returnString += " occasionally";
                } else if (you.piety < piety_breakpoint(4))
                {
                    returnString += " frequently";
                } else if (you.piety >= piety_breakpoint(4))
                {
                    returnString += " often";
                }
                break;
            }
            default:
                break;
        }
    return returnString;
}

string desc_subtype_of_ancient_god_passive()
{
    string returnString = "";
    ag_passive_threatening_boost_subtype type = get_threatening_boost_subtype();
    switch(type)
    {
        case ST_EV:
            returnString += "evasion";
            break;
        case ST_AC:
            returnString += "armour";
            break;
        case ST_Slaying:
            returnString += "slaying";
            break;
        case ST_Regen_HP:
            returnString += "HP regen";
            break;
        case ST_INT:
            returnString += "intelligence";
            break;
        default:
            returnString += "buggy code";
    }
    return returnString;
}

// A slightly smoother generalisation of the current_horror_level function
// used for Ru's cowardly mutation. Used in places where ancient god abilities
// need an opinion on local threat level.
// Threat score: 25/red 10/yellow 1/white 0/grey mons
int current_threat_level()
{
    int return_threat_level = 0;

    for (monster_near_iterator mi(&you, LOS_NO_TRANS); mi; ++mi)
    {

        if (mons_aligned(*mi, &you)
            || !mons_is_threatening(**mi)
            || mons_is_tentacle_or_tentacle_segment(mi->type))
        {
            continue;
        }

        const mon_threat_level_type threat_type = mons_threat_level(**mi);
        switch(threat_type)
        {
            case MTHRT_TRIVIAL:
                break;
            case MTHRT_EASY:
                return_threat_level += 1;
                break;
            case MTHRT_TOUGH:
                return_threat_level += 10;
                break;
            case MTHRT_NASTY:
                return_threat_level += 25;
                break;
            default:
                break;
        }
    }
    
    return_threat_level = max(0, return_threat_level);
    return return_threat_level;
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

COMPILE_CHECK(NUM_AGP<MAX_AGP); //uint8
COMPILE_CHECK(NUM_AGAS<MAX_AGAS); //uint8
COMPILE_CHECK(NUM_AGAC<MAX_AGAC); //uint8
COMPILE_CHECK(NUM_AGM<MAX_AGM); //uint8
COMPILE_CHECK(NUM_ST<MAX_ST);
