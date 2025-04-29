#pragma once

#include "player.h" // Required for you.game_seed
#include <vector>
#include "religion-enum.h"
#include "ability-type.h"

// Defines the two possible states for GOD_ANCIENT_ONE based on the seed.
enum class AncientOneState
{
    ALPHA, // Appears D:1, Name "Alpha", Power Alpha Power
    BETA   // Appears D:2, Name "Beta", Power Beta Power
};

/**
 * @brief Determines the state (ALPHA or BETA) for GOD_ANCIENT_ONE based on the game seed.
 *
 * @return AncientOneState::ALPHA or AncientOneState::BETA.
 */
AncientOneState get_ancient_one_state();

// Forward declaration
struct god_power;

/**
 * @brief Gets the powers granted by GOD_ANCIENT_ONE based on the current seed state.
 *
 * @return A vector containing the god_power definitions for the current state.
 */
std::vector<god_power> get_ancient_one_powers(); 