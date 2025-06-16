#include "AppHdr.h"
#include "god-ancient.h"
#include "ability-type.h"
#include "player.h"
#include "religion.h"

static vector<ancient_power_spec> _get_ancient_power_defs()
{
    vector<ancient_power_spec> powers;
    powers.emplace_back(ancient_power_spec{ PASSIVE_STAT_BOOST_STR, ANCIENT_POWER_PASSIVE, "You feel unnaturally strong (+3 Str).", 0, ABIL_NON_ABILITY });
    powers.emplace_back(ancient_power_spec{ PASSIVE_STAT_BOOST_INT, ANCIENT_POWER_PASSIVE, "Your mind feels unnaturally sharp (+3 Int).", 0, ABIL_NON_ABILITY });
    powers.emplace_back(ancient_power_spec{ PASSIVE_STAT_BOOST_DEX, ANCIENT_POWER_PASSIVE, "You feel unnaturally agile (+3 Dex).", 2, ABIL_NON_ABILITY });

    powers.emplace_back(ancient_power_spec{ SMALL_POWER_BLINK, ANCIENT_POWER_SMALL, "You can blink.", 1, ABIL_NON_ABILITY });
    powers.emplace_back(ancient_power_spec{ SMALL_POWER_FLAME, ANCIENT_POWER_SMALL, "You can call forth a gout of flame.", 1, ABIL_NON_ABILITY });

    powers.emplace_back(ancient_power_spec{ MEDIUM_POWER_SUMMON_CRITTER, ANCIENT_POWER_MEDIUM, "You can summon a friendly creature.", 3, ABIL_NON_ABILITY });
    powers.emplace_back(ancient_power_spec{ MEDIUM_POWER_INVISIBILITY, ANCIENT_POWER_MEDIUM, "You can become invisible.", 4, ABIL_NON_ABILITY });

    powers.emplace_back(ancient_power_spec{ LARGE_POWER_CHAIN_LIGHTNING, ANCIENT_POWER_LARGE, "You can unleash a bolt of chain lightning.", 5, ABIL_NON_ABILITY });
    powers.emplace_back(ancient_power_spec{ LARGE_POWER_REGEN, ANCIENT_POWER_LARGE, "You can call on the gods for rapid healing.", 5, ABIL_NON_ABILITY });
    return powers;
}

static const vector<ancient_power_spec> ancient_power_defs = _get_ancient_power_defs();

static void _generate_power(ancient_power_category category)
{
    vector<ancient_power_spec> possible_powers;
    for (const auto& power : ancient_power_defs)
    {
        if (power.category == category)
            possible_powers.push_back(power);
    }
    you.ancient_powers[category] = possible_powers[random2(possible_powers.size())].type;
}

void generate_ancient_god_powers()
{
    for (int i = 0; i < NUM_ANCIENT_POWER_CATEGORIES; ++i)
        _generate_power(static_cast<ancient_power_category>(i));
}

string get_ancient_god_main_description()
{
    return "The Ancient is a god of unknown origin and power. Its "
           "followers are granted a unique set of powers, different "
           "for each champion.";
}

static const ancient_power_spec& _get_power_spec(ancient_power_type type)
{
    for (const auto& power : ancient_power_defs)
    {
        if (power.type == type)
            return power;
    }
    return ancient_power_defs[0]; // Should not happen
}

vector<god_power> get_ancient_god_powers()
{
    vector<god_power> powers;
    for (int i = 0; i < NUM_ANCIENT_POWER_CATEGORIES; ++i)
    {
        const ancient_power_spec& spec = _get_power_spec(you.ancient_powers[i]);
        powers.push_back({ spec.piety_rank, spec.ability, spec.description });
    }
    return powers;
}