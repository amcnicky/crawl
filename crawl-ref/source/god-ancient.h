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
    PASSIVE_PLACEHOLDER,

    // Small Powers
    SMALL_POWER_PLACEHOLDER_1,
    SMALL_POWER_PLACEHOLDER_2,

    // Large Powers
    LARGE_POWER_CREATURE_MARCH,
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

extern const mood_spec mood_data[];
int get_mood_data_size();

string get_ancient_god_name();
string get_ancient_god_title();
string get_ancient_god_main_description();
string get_march_power_description();
void generate_ancient_god_powers();
vector<god_power> get_ancient_god_powers();