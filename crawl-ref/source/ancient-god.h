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
passive_t ancient_god_passive();
const char* ancient_god_passive_description_short();
string ancient_god_passive_description_long();
string desc_freq_of_ancient_god_passive();
ability_type ancient_god_small_ability();
const char* ancient_god_small_ability_description_short();
const char* ancient_god_small_ability_description_long();
ability_type ancient_god_cap_ability();
const char* ancient_god_cap_ability_description_short();
const char* ancient_god_cap_ability_description_long();