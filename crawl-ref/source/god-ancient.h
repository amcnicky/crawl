#pragma once

#include <string>
#include <vector>

#include "ability-type.h"
#include "format.h"

struct god_power;

enum ancient_power_category
{
    ANCIENT_POWER_PASSIVE,
    ANCIENT_POWER_SMALL,
    ANCIENT_POWER_MEDIUM,
    ANCIENT_POWER_LARGE,
    NUM_ANCIENT_POWER_CATEGORIES
};

enum ancient_power_type
{
    ANCIENT_POWER_NONE = 0,

    // Passives
    PASSIVE_STAT_BOOST_STR,
    PASSIVE_STAT_BOOST_INT,
    PASSIVE_STAT_BOOST_DEX,

    // Small Powers
    SMALL_POWER_BLINK,
    SMALL_POWER_FLAME,

    // Medium Powers
    MEDIUM_POWER_SUMMON_CRITTER,
    MEDIUM_POWER_INVISIBILITY,

    // Large Powers
    LARGE_POWER_CHAIN_LIGHTNING,
    LARGE_POWER_REGEN,

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

string get_ancient_god_main_description();
void generate_ancient_god_powers();
vector<god_power> get_ancient_god_powers();