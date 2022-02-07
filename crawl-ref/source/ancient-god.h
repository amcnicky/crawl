/**
 * @file
 * @brief  Ancient God related functions. 
**/

#pragma once

#include <cstdint>
#include "ability-type.h"
#include "god-passive.h"

// used elsewhere to avoid hardcoding in multiple locations
const int ancient_god_passive_breakpoint = 0;
const int ancient_god_small_breakpoint = 2;
const int ancient_god_mut_breakpoint = 3;
const int ancient_god_cap_breakpoint = 5;
const int NUM_AG_NAMES = 81;
const int NUM_AG_TITLES = 30;
const int NUM_AG_SPRIGGAN_NAMES = 38;

enum ag_passive_threatening_boost_subtype
{
    ST_EV,
    ST_AC,
    ST_Slaying,
    ST_Regen_HP,
    ST_Regen_MP,
    ST_INT,
    // STR and DEX are close to Slaying/EV, but could consider them as well.
    // ST_DEX,
    // ST_STR,
    NUM_ST,
    MAX_ST = 255,
};

enum ag_passive_type
{
    AG_P_RESCUE_MAMMAL,
    AG_P_DISCOUNT_SHOPPING,
    AG_P_POTION_REFILLS,
    AG_P_SPRIGGAN_SIDEKICK,
    AG_P_THREATENING_BOOST,
    NUM_AGP,
    MAX_AGP = 255,
};

enum ag_abil_small_type
{
    AG_S_REALITY_DILATION,
    AG_S_SPATIAL_SINGULARITY,
    AG_S_DIVINE_CONDUIT,
    AG_S_RADIATION_STORM,
    AG_S_PLAGUE_WORMS,
    NUM_AGAS,
    MAX_AGAS = 255,
};

enum ag_abil_cap_type
{
    AG_C_REALITY_DILATION,
    NUM_AGAC,
    MAX_AGAC = 255,
};

enum ag_mut_type
{
    AG_M_HP_CASTING,
    NUM_AGM,
    MAX_AGM = 255,
};

uint8_t generate_ancient_god_name_key();
uint8_t generate_ancient_god_passive_key();
uint8_t generate_ancient_god_small_key();
uint8_t generate_ancient_god_cap_key();
uint8_t generate_ancient_god_mut_key();
uint8_t generate_ancient_god_like_key();
uint8_t generate_ancient_god_dislike_key();
string ancient_god_name();
string sidekick_name();
string sidekick_description();
void setup_ag_spriggan_sidekick();
mid_t ag_sidekick();
const char* ancient_god_name_extra();
const char* ancient_god_title();
string apply_ag_dynamics(string desc);
string ag_desc_introduction();
string ag_desc_how_god_was();
string ag_desc_god_fall();
string ag_desc_god_now();
string ag_desc_powers();
passive_t ancient_god_passive();
bool ancient_god_passive_active(passive_t passive);
const char* ancient_god_passive_description_short();
string ancient_god_passive_description_long();
string desc_freq_of_ancient_god_passive();
string desc_subtype_of_ancient_god_passive();
int current_threat_level();
ag_passive_threatening_boost_subtype get_threatening_boost_subtype();
ability_type ancient_god_small_ability();
const char* ancient_god_small_ability_description_short();
const char* ancient_god_small_ability_description_long();
ability_type ancient_god_cap_ability();
const char* ancient_god_cap_ability_description_short();
const char* ancient_god_cap_ability_description_long();

