#pragma once

#include <string>
#include <vector>

#include "ability-type.h"
#include "format.h"
#include "enum.h"
#include "player.h"

struct god_power;

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

    // Small Powers
    SMALL_POWER_STABILISE_MUTATION,
    SMALL_POWER_PLACEHOLDER_2,

    // Large Powers
    LARGE_POWER_CREATURE_CALL,
    LARGE_POWER_PLACEHOLDER,

    NUM_ANCIENT_POWERS,
};

struct ancient_power_spec
{
    ancient_power_type type;
    ancient_power_category category;
    const char* description;
    int piety_rank;
    ability_type ability; // ABIL_NON_ABILITY for passives
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

// Ancient god ability cost calculations
int get_ancient_creature_call_piety_cost();
string get_ancient_creature_call_cost_description();
string get_ancient_creature_call_detailed_cost_description();

// Ancient god abilities
spret cast_ancient_stabilise_mutation();