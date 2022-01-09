/**
 * @file
 * @brief  Ancient God related functions. 
**/

#pragma once

#include <cstdint>
#include "ability-type.h"
#include "god-passive.h"

uint8_t generate_ancient_god_name_key();
uint8_t generate_ancient_god_passive_key();
uint8_t generate_ancient_god_small_key();
uint8_t generate_ancient_god_cap_key();
uint8_t generate_ancient_god_mut_key();
uint8_t generate_ancient_god_like_key();
uint8_t generate_ancient_god_dislike_key();
string ancient_god_name();
string ancient_god_name_extra();
passive_t ancient_god_passive();
const char* ancient_god_passive_description_short();
string ancient_god_passive_description_long();
ability_type ancient_god_small_ability();
const char* ancient_god_small_ability_description_short();
string ancient_god_small_ability_description_long();
ability_type ancient_god_cap_ability();
const char* ancient_god_cap_ability_description_short();
string ancient_god_cap_ability_description_long();