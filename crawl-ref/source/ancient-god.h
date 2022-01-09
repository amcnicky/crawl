/**
 * @file
 * @brief  Ancient God related functions. 
**/

#pragma once

#include <cstdint>
#include "player.h"

uint8_t generate_ancient_god_name_key();
uint8_t generate_ancient_god_passive_key();
uint8_t generate_ancient_god_small_key();
uint8_t generate_ancient_god_cap_key();
uint8_t generate_ancient_god_like_key();
uint8_t generate_ancient_god_dislike_key();
string god_name_ancient();
string god_name_ancient_extra();