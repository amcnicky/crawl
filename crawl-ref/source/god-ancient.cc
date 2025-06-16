#include "AppHdr.h"
#include "god-ancient.h"
#include "god-ancient-data.h"

#include "ability-type.h"
#include "player.h"
#include "random.h"
#include "religion.h"
#include "stringutil.h"

#define AG_NAME_KEY "ag_name_idx"
#define AG_TITLE_KEY "ag_title_idx"
#define AG_ASPECT_KEY "ag_aspect_idx"
#define AG_FACTION_KEY "ag_faction_idx"
#define AG_INTRO_KEY "ag_intro_idx"
#define AG_WAS_KEY "ag_was_idx"
#define AG_FALL_KEY "ag_fall_idx"
#define AG_NOW_KEY "ag_now_idx"

class AncientGodIdentity
{
public:
    AncientGodIdentity()
    {
        // _ensure_identity(); // purpose: ensure the identity is set, and set it if it's not
    }

    string get_name()
    {
        _ensure_identity();
        return ag_name_data[you.props[AG_NAME_KEY].get_int()];
    }

    string get_title()
    {
        _ensure_identity();
        return ag_title_data[you.props[AG_TITLE_KEY].get_int()];
    }

    string get_description()
    {
        _ensure_identity();
        string desc = "";
        desc += _apply_dynamics(ag_desc_introduction_data[you.props[AG_INTRO_KEY].get_int()]);
        desc += _apply_dynamics(ag_desc_how_god_was_data[you.props[AG_WAS_KEY].get_int()]);
        desc += _apply_dynamics(ag_desc_god_fall_data[you.props[AG_FALL_KEY].get_int()]);
        desc += _apply_dynamics(ag_desc_god_now_data[you.props[AG_NOW_KEY].get_int()]);
        return desc;
    }

private:
    void _ensure_identity()
    {
        if (you.props.exists(AG_NAME_KEY))
            return;

        // need to salt to avoid correlation between the different parts of the description
        {
            rng::subgenerator subgen_name(you.game_seed, 1);
            you.props[AG_NAME_KEY] = random2(ARRAYSZ(ag_name_data));
        }

        {
            rng::subgenerator subgen_title(you.game_seed, 2);
            you.props[AG_TITLE_KEY] = random2(ARRAYSZ(ag_title_data));
        }

        {
            rng::subgenerator subgen_aspect(you.game_seed, 3);
            you.props[AG_ASPECT_KEY] = random2(ARRAYSZ(ag_desc_aspect_data));
        }

        {
            rng::subgenerator subgen_faction(you.game_seed, 4);
            you.props[AG_FACTION_KEY] = random2(ARRAYSZ(ag_desc_faction_data));
        }

        {
            rng::subgenerator subgen_intro(you.game_seed, 5);
            you.props[AG_INTRO_KEY] = random2(ARRAYSZ(ag_desc_introduction_data));
        }

        {
            rng::subgenerator subgen_was(you.game_seed, 6);
            you.props[AG_WAS_KEY] = random2(ARRAYSZ(ag_desc_how_god_was_data));
        }

        {
            rng::subgenerator subgen_fall(you.game_seed, 7);
            you.props[AG_FALL_KEY] = random2(ARRAYSZ(ag_desc_god_fall_data));
        }

        {
            rng::subgenerator subgen_now(you.game_seed, 8);
            you.props[AG_NOW_KEY] = random2(ARRAYSZ(ag_desc_god_now_data));
        }
    }

    string _apply_dynamics(string desc)
    {
        string godname = get_name();
        string aspect = ag_desc_aspect_data[you.props[AG_ASPECT_KEY].get_int()];
        string faction = ag_desc_faction_data[you.props[AG_FACTION_KEY].get_int()];

        desc = replace_all(desc, "GODNAME", godname);
        desc = replace_all(desc, "ASPECT", aspect);
        desc = replace_all(desc, "FACTION", faction);
        return desc;
    }
};

static AncientGodIdentity& _get_god_identity()
{
    static AncientGodIdentity identity;
    return identity;
}

string get_ancient_god_name()
{
    return _get_god_identity().get_name();
}

string get_ancient_god_title()
{
    return _get_god_identity().get_title();
}

string get_ancient_god_main_description()
{
    return _get_god_identity().get_description();
}

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
    // ensure god identity is set first, so power generation can be seeded
    // this will call get_name(), which now ensures the identity is created
    (void)_get_god_identity().get_name();
    for (int i = 0; i < NUM_ANCIENT_POWER_CATEGORIES; ++i)
        _generate_power(static_cast<ancient_power_category>(i));
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