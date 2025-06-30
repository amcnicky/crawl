#pragma once

#include <string>
#include <vector>

#include "ability-type.h"
#include "format.h"
#include "enum.h"
#include "player.h"
#include "mon-enum.h"

struct god_power;

// Property keys for ancient god data
#define AG_CALL_MONSTER_SET_KEY "ag_call_monster_set"
#define AG_CALL_MOOD_KEY "ag_call_mood"
#define CELESTIAL_MARTYRDOM_KEY "celestial_martyrdom"

enum ancient_power_category
{
    ANCIENT_POWER_PASSIVE,
    ANCIENT_POWER_SMALL,
    ANCIENT_POWER_LARGE,
    NUM_ANCIENT_POWER_CATEGORIES
};

enum ancient_power_type
{
    ANCIENT_POWER_NONE = 0,

    // Passives
    PASSIVE_DEGENERATIVE_CASTING,
    PASSIVE_INFERNAL_ABSORPTION,
    PASSIVE_CELESTIAL_MARTYRDOM,
    PASSIVE_RUNIC_TRANSFORMATION,

    // Small Powers
    SMALL_POWER_STABILISE_MUTATION,
    SMALL_POWER_HORRIFYING_VISAGE,

    // Large Powers
    LARGE_POWER_CREATURE_CALL,

    NUM_ANCIENT_POWERS,
};

// Ancient god creature summoning sets for creature call ability
// Based on comprehensive ziggurat monster sets plus creative groupings
enum ancient_set_type
{
    // --- Core Branch Sets ---
    ANCIENT_SET_LAIR,
    ANCIENT_SET_SNAKE,
    ANCIENT_SET_SWAMP,
    ANCIENT_SET_SHOALS,
    ANCIENT_SET_SPIDER,
    ANCIENT_SET_SLIME,
    ANCIENT_SET_ORC,
    ANCIENT_SET_ELF,
    ANCIENT_SET_VAULTS,
    ANCIENT_SET_CRYPT,
    ANCIENT_SET_TOMB,
    ANCIENT_SET_ABYSS,

    // --- Hell Branch Sets ---
    ANCIENT_SET_COCYTUS,
    ANCIENT_SET_GEHENNA,
    ANCIENT_SET_DIS,
    ANCIENT_SET_TARTARUS,
    ANCIENT_SET_PANDEMONIUM,

    // --- Theme Sets ---
    ANCIENT_SET_GIANTS,
    ANCIENT_SET_DRAGONS,
    ANCIENT_SET_DRACONIANS,
    ANCIENT_SET_ARCHERS,
    ANCIENT_SET_CONJURERS,
    ANCIENT_SET_LAIR_RUNES,
    ANCIENT_SET_VESTIBULE,

    // --- Elemental Sets ---
    ANCIENT_SET_HOLY,
    ANCIENT_SET_FIRE,
    ANCIENT_SET_ICE,
    ANCIENT_SET_AIR,
    ANCIENT_SET_EARTH,
    ANCIENT_SET_NEGATIVE,
    ANCIENT_SET_CHAOS,
    ANCIENT_SET_EYE_SOCIETY,
    ANCIENT_SET_UNDEAD_HORDE,
    ANCIENT_SET_THE_SWARM,
    ANCIENT_SET_ARTIFICIAL_BEINGS,
    ANCIENT_SET_MYSTICAL_CREATURES,

    NUM_ANCIENT_SETS,
};

// Describes a single monster within a themed set for ancient god summons.
struct ancient_summon_set_monster
{
    monster_type type;
};

// Defines a themed set of monsters that can be summoned.
struct ancient_summon_set
{
    ancient_set_type set_type;
    const char* name;
    const char* description;
    vector<ancient_summon_set_monster> monsters;
};

extern const ancient_summon_set ancient_summon_sets[];

// Holds the definition for a single power offered by an ancient god.
struct ancient_power_spec
{
    ancient_power_type type;
    ancient_power_category category;
    const char* description;
    int piety_rank;
    ability_type ability;
};

struct mood_spec {
    const char* name;
    enchant_type ench;
    int weight;
};

struct mood_scaling_spec {
    const char* name;
    int scaling_factor;           // Multiplier (e.g., 150 = 1.5x, 80 = 0.8x)
    int scaling_factor_spellcaster; // For furious mood when monster is a spellcaster
};

extern const mood_spec mood_data[];
extern const mood_scaling_spec mood_scaling_data[];

int get_mood_data_size();

string get_ancient_god_name();
string get_ancient_god_title();
string get_ancient_god_main_description();
string get_call_power_description();
void generate_ancient_god_powers();
vector<god_power> get_ancient_god_powers();
bool has_degenerative_casting();
bool has_infernal_absorption();
bool has_celestial_martyrdom();
bool has_runic_transformation();

// Ancient god ability cost calculations
int get_ancient_creature_call_piety_cost();
string get_ancient_creature_call_cost_description();
string get_ancient_creature_call_detailed_cost_description();

// Ancient god abilities
spret cast_ancient_stabilise_mutation();
spret cast_ancient_horrifying_visage(int pow, bool fail);
void do_horrifying_visage_turn();

// Get ancient set information
const char* get_ancient_set_name(ancient_set_type set);
const char* get_ancient_set_description(ancient_set_type set);





