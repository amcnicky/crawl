#include "AppHdr.h"

#include "ancient-one.h"

#include "player.h" // Required for you.game_seed
#include "random.h" // Potentially needed for more complex state logic later
#include "religion.h" // Needed for god_power struct definition
#include <vector>     // Needed for std::vector

/**
 * @brief Determines the state (ALPHA or BETA) for GOD_ANCIENT_ONE based on the game seed.
 *
 * @return AncientOneState::ALPHA or AncientOneState::BETA.
 */
AncientOneState get_ancient_one_state()
{
    // Simple deterministic check based on game seed parity.
    // Ensure you.game_seed is initialized before this is called!
    // If game hasn't started, default might be needed, but for god name/powers,
    // it should be available.
    // TODO: Add checks or default if you.game_seed is 0?
    return (you.game_seed % 2 == 0) ? AncientOneState::ALPHA : AncientOneState::BETA;
}

// Placeholder implementations for abilities (will be registered elsewhere if possible)

/**
 * @brief Placeholder implementation for Alpha Power.
 */
bool ancient_one_alpha_power()
{
    mpr("Alpha power activated! (Placeholder)");
    // TODO: Implement actual ability logic
    return true;
}

/**
 * @brief Placeholder implementation for Beta Power.
 */
bool ancient_one_beta_power()
{
    mpr("Beta power activated! (Placeholder)");
    // TODO: Implement actual ability logic
    return true;
}

/**
 * @brief Gets the powers granted by GOD_ANCIENT_ONE based on the current seed state.
 *
 * @return A vector containing the god_power definitions for the current state.
 */
std::vector<god_power> get_ancient_one_powers()
{
    std::vector<god_power> powers;
    AncientOneState state = get_ancient_one_state();

    // Define powers based on state
    // For MVP, just one power at rank 1 (piety *) for simplicity
    if (state == AncientOneState::ALPHA)
    {
        powers.emplace_back(1, ABIL_ALPHA_POWER, "invoke Alpha Power");
    }
    else // BETA
    {
        powers.emplace_back(1, ABIL_BETA_POWER, "invoke Beta Power");
    }

    // Add common/fixed powers for Ancient One here if any

    // Set the god type for all defined powers
    for (auto& p : powers)
        p.god = GOD_ANCIENT_ONE;

    return powers;
}

// TODO: Investigate how to register these abilities without modifying god-abil.cc
// Possible options:
// 1. Static initializer object that calls a registration function.
// 2. Find a central ability registration point called during startup. 