#include "AppHdr.h"
#include "god-ancient.h"
#include "god-ancient-data.h"
#include "english.h"
#include "random.h"
#include "religion.h"
#include "stringutil.h"
#include "mon-tentacle.h"

#define AG_NAME_KEY "ag_name_idx"
#define AG_TITLE_KEY "ag_title_idx"
#define AG_ASPECT_KEY "ag_aspect_idx"
#define AG_FACTION_KEY "ag_faction_idx"
#define AG_INTRO_KEY "ag_intro_idx"
#define AG_WAS_KEY "ag_was_idx"
#define AG_FALL_KEY "ag_fall_idx"
#define AG_NOW_KEY "ag_now_idx"
#define AG_MARCH_MONSTER_KEY "ag_march_monster"
#define AG_MARCH_MOOD_KEY "ag_march_mood"

const mood_spec mood_data[] =
{
    // Common beneficial moods (higher weights)
    { "furious", ENCH_BERSERK, 20 },
    { "hasty", ENCH_HASTE, 15 },
    { "mighty", ENCH_MIGHT, 12 },
    { "swift", ENCH_SWIFT, 10 },
    { "invisible", ENCH_INVIS, 8 },
    { "regenerating", ENCH_REGENERATION, 6 },
    { "agile", ENCH_AGILE, 5 },
    { "strong-willed", ENCH_STRONG_WILLED, 4 },
    
    // Uncommon neutral/mixed moods
    { "frenzied", ENCH_FRENZIED, 3 },
    { "confused", ENCH_CONFUSION, 2 },
    { "feared", ENCH_FEAR, 2 },
    { "blind", ENCH_BLIND, 2 },
    { "dazed", ENCH_DAZED, 2 },
    { "sick", ENCH_SICK, 2 },
    
    // Rare detrimental moods (very low weights)
    { "petrified", ENCH_PETRIFIED, 1 },
    { "paralysed", ENCH_PARALYSIS, 1 },
    { "sleeping", ENCH_DEEP_SLEEP, 1 },
    { "weak", ENCH_WEAK, 1 },
    { "slowed", ENCH_SLOW, 1 },
};

static vector<monster_type> valid_march_monsters;

static void _build_valid_march_monsters()
{
    if (!valid_march_monsters.empty())
        return;

    for (int i = 0; i < NUM_MONSTERS; ++i)
    {
        const monster_type type = static_cast<monster_type>(i);
        if (mons_is_unique(type)
            || mons_class_flag(type, M_NO_POLY_TO)
            || mons_class_flag(type, M_UNFINISHED)
            || mons_is_tentacle_or_tentacle_segment(type))
        {
            continue;
        }

        if (mons_class_holiness(type) == MH_PLANT)
            continue;

        valid_march_monsters.push_back(type);
    }
}

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

        {
            _build_valid_march_monsters();
            rng::subgenerator subgen_march_monster(you.game_seed, 9);
            you.props["ag_march_monster"] = valid_march_monsters[random2(valid_march_monsters.size())];
        }

        {
            rng::subgenerator subgen_march_mood(you.game_seed, 10);
            int total_weight = 0;
            for (const auto& mood : mood_data)
                total_weight += mood.weight;
            
            int choice = random2(total_weight);
            int mood_idx = 0;
            for (size_t i = 0; i < ARRAYSZ(mood_data); ++i)
            {
                if (choice < mood_data[i].weight)
                {
                    mood_idx = i;
                    break;
                }
                choice -= mood_data[i].weight;
            }
            you.props["ag_march_mood"] = mood_idx;
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

int get_mood_data_size()
{
    return ARRAYSZ(mood_data);
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

string get_march_power_description()
{
    // Ensure identity is set before accessing properties
    _get_god_identity().get_name(); // This will call _ensure_identity()
    
    // Safety checks for properties
    if (!you.props.exists("ag_march_monster") || !you.props.exists("ag_march_mood"))
    {
        return "March of the Monsters"; // fallback description
    }
    
    const monster_type type = static_cast<monster_type>(you.props["ag_march_monster"].get_int());
    const int mood_idx = you.props["ag_march_mood"].get_int();

    // Safety check for mood index
    if (mood_idx < 0 || mood_idx >= static_cast<int>(ARRAYSZ(mood_data)))
    {
        return "March of the Monsters"; // fallback description
    }

    const string mood = mood_data[mood_idx].name;
    const string creature = pluralise_monster(mons_type_name(type, DESC_PLAIN));

    return make_stringf("March of the %s %s", mood.c_str(), creature.c_str());
}

static vector<ancient_power_spec> _get_ancient_power_defs()
{
    vector<ancient_power_spec> powers;
    powers.emplace_back(ancient_power_spec{ PASSIVE_PLACEHOLDER, ANCIENT_POWER_PASSIVE, "power to be implemented", 0, ABIL_NON_ABILITY });
    powers.emplace_back(ancient_power_spec{ SMALL_POWER_PLACEHOLDER_1, ANCIENT_POWER_SMALL, "power to be implemented", 1, ABIL_NON_ABILITY });
    powers.emplace_back(ancient_power_spec{ SMALL_POWER_PLACEHOLDER_2, ANCIENT_POWER_SMALL, "power to be implemented", 1, ABIL_NON_ABILITY });
    powers.emplace_back(ancient_power_spec{ LARGE_POWER_CREATURE_MARCH, ANCIENT_POWER_LARGE, "power to be implemented", 5, ABIL_ANCIENT_CREATURE_MARCH });
    return powers;
}

static const vector<ancient_power_spec> ancient_power_defs = _get_ancient_power_defs();

static void _generate_power(ancient_power_category category, vector<int>& choices)
{
    vector<ancient_power_spec> possible_powers;
    for (const auto& power : ancient_power_defs)
    {
        if (power.category == category)
        {
            bool already_chosen = false;
            for (auto chosen_power : choices)
                if (chosen_power == power.type)
                    already_chosen = true;
            if (!already_chosen)
                possible_powers.push_back(power);
        }
    }
    choices.push_back(possible_powers[random2(possible_powers.size())].type);
}

void generate_ancient_god_powers()
{
    // ensure god identity is set first, so power generation can be seeded
    // this will call get_name(), which now ensures the identity is created
    (void)_get_god_identity().get_name();

    vector<int> powers;
    _generate_power(ANCIENT_POWER_SMALL, powers);
    _generate_power(ANCIENT_POWER_SMALL, powers);
    powers.push_back(LARGE_POWER_CREATURE_MARCH);

    you.ancient_powers.clear();
    for (const auto& power : powers)
        you.ancient_powers.push_back(power);
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

// Static storage for dynamic descriptions to avoid dangling pointers
static string march_power_desc;

vector<god_power> get_ancient_god_powers()
{
    vector<god_power> powers;
    for (const auto& power_val : you.ancient_powers)
    {
        ancient_power_type power_type = static_cast<ancient_power_type>(power_val.get_int());
        const ancient_power_spec& spec = _get_power_spec(power_type);
        if (spec.type == LARGE_POWER_CREATURE_MARCH)
        {
            // Update the static description string
            march_power_desc = get_march_power_description();
            god_power power(spec.piety_rank, spec.ability, march_power_desc.c_str());
            power.god = GOD_ANCIENT;
            powers.push_back(power);
        }
        else
        {
            god_power power(spec.piety_rank, spec.ability, spec.description);
            power.god = GOD_ANCIENT;
            powers.push_back(power);
        }
    }
    return powers;
}