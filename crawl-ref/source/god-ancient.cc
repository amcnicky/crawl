#include "AppHdr.h"
#include "god-ancient.h"
#include "god-ancient-data.h"
#include "english.h"
#include "god-passive.h"
#include "random.h"
#include "religion.h"
#include "stringutil.h"
#include "mon-tentacle.h"
#include "mon-util.h"
#include "monster.h"
#include "mutation.h"
#include "prompt.h"
#include "message.h"

#define AG_NAME_KEY "ag_name_idx"
#define AG_TITLE_KEY "ag_title_idx"
#define AG_ASPECT_KEY "ag_aspect_idx"
#define AG_FACTION_KEY "ag_faction_idx"
#define AG_INTRO_KEY "ag_intro_idx"
#define AG_WAS_KEY "ag_was_idx"
#define AG_FALL_KEY "ag_fall_idx"
#define AG_NOW_KEY "ag_now_idx"
#define AG_CALL_MONSTER_KEY "ag_call_monster"
#define AG_CALL_MOOD_KEY "ag_call_mood"

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

// Mood scaling factors for piety cost calculation
// You can manually edit these values as needed
// Values are in hundredths: 150 = 1.5x, 80 = 0.8x, etc.
const mood_scaling_spec mood_scaling_data[] =
{
    // Beneficial moods - higher cost due to increased effectiveness
    { "furious", 150, 80 },      // Good for melee, bad for spellcasters
    { "hasty", 130, 130 },
    { "mighty", 140, 140 },
    { "swift", 120, 120 },
    { "invisible", 160, 160 },
    { "regenerating", 130, 130 },
    { "agile", 120, 120 },
    { "strong-willed", 110, 110 },
    
    // Mixed moods - moderate cost
    { "frenzied", 100, 100 },
    { "confused", 70, 70 },
    { "feared", 60, 60 },
    { "blind", 50, 50 },
    { "dazed", 60, 60 },
    { "sick", 80, 80 },
    
    // Detrimental moods - lower cost due to reduced effectiveness
    { "petrified", 30, 30 },
    { "paralysed", 20, 20 },
    { "sleeping", 10, 10 },
    { "weak", 40, 40 },
    { "slowed", 50, 50 },
};

static vector<monster_type> valid_call_monsters;

static void _build_valid_call_monsters()
{
    if (!valid_call_monsters.empty())
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

        // Skip derived monsters that shouldn't be summoned directly
        if (type == MONS_SKELETON || type == MONS_ZOMBIE || type == MONS_SIMULACRUM)
            continue;

        // Skip monsters with 0 hit dice (they're usually derived or incomplete)
        if (mons_class_hit_dice(type) <= 0)
            continue;

        valid_call_monsters.push_back(type);
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
            _build_valid_call_monsters();
            rng::subgenerator subgen_call_monster(you.game_seed, 9);
            
            // Safety check for valid monsters list
            if (valid_call_monsters.empty())
            {
                you.props["ag_call_monster"] = MONS_RAT; // Safe fallback
            }
            else
            {
                you.props["ag_call_monster"] = valid_call_monsters[random2(valid_call_monsters.size())];
            }
        }

        {
            rng::subgenerator subgen_call_mood(you.game_seed, 10);
            int total_weight = 0;
            for (const auto& mood : mood_data)
                total_weight += mood.weight;
            
            ASSERT(total_weight > 0); // Weights are all positive constants
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
            // Ensure mood_idx is within bounds as a final safety check
            mood_idx = max(0, min(mood_idx, static_cast<int>(ARRAYSZ(mood_data)) - 1));
            you.props["ag_call_mood"] = mood_idx;
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

string get_call_power_description()
{
    // Ensure identity is set before accessing properties
    _get_god_identity().get_name(); // This will call _ensure_identity()
    
    // Safety checks for properties
    if (!you.props.exists("ag_call_monster") || !you.props.exists("ag_call_mood"))
    {
        return "Call of Monsters"; // fallback description
    }
    
    const monster_type type = static_cast<monster_type>(you.props["ag_call_monster"].get_int());
    const int mood_idx = you.props["ag_call_mood"].get_int();

    // Safety check for mood index
    if (mood_idx < 0 || mood_idx >= static_cast<int>(ARRAYSZ(mood_data)))
    {
        return "Call of Monsters"; // fallback description
    }

    const string mood = mood_data[mood_idx].name;
    const string creature = pluralise_monster(mons_type_name(type, DESC_PLAIN));

    // Use a more descriptive format that clearly indicates it's an ability
    // Format: "Call Furious Draconians" instead of just "Furious Draconians"
    return make_stringf("Call %s %s", mood.c_str(), creature.c_str());
}

static vector<ancient_power_spec> _get_ancient_power_defs()
{
    vector<ancient_power_spec> powers;
    powers.emplace_back(ancient_power_spec{ PASSIVE_DEGENERATIVE_CASTING, ANCIENT_POWER_PASSIVE, "Degenerative Casting", 0, ABIL_NON_ABILITY });
    powers.emplace_back(ancient_power_spec{ SMALL_POWER_STABILISE_MUTATION, ANCIENT_POWER_SMALL, "Stabilise Mutation", 2, ABIL_ANCIENT_STABILISE_MUTATION });
    powers.emplace_back(ancient_power_spec{ SMALL_POWER_PLACEHOLDER_2, ANCIENT_POWER_SMALL, "power to be implemented", 1, ABIL_NON_ABILITY });
    powers.emplace_back(ancient_power_spec{ LARGE_POWER_CREATURE_CALL, ANCIENT_POWER_LARGE, "power to be implemented", 5, ABIL_ANCIENT_CREATURE_CALL });
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
    _generate_power(ANCIENT_POWER_PASSIVE, powers);
    _generate_power(ANCIENT_POWER_SMALL, powers);
    _generate_power(ANCIENT_POWER_SMALL, powers);
    _generate_power(ANCIENT_POWER_LARGE, powers);

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
static string call_power_desc;

vector<god_power> get_ancient_god_powers()
{
    vector<god_power> powers;
    for (const auto& power_val : you.ancient_powers)
    {
        ancient_power_type power_type = static_cast<ancient_power_type>(power_val.get_int());
        const ancient_power_spec& spec = _get_power_spec(power_type);
        if (spec.type == LARGE_POWER_CREATURE_CALL)
        {
            // Use the dynamic description for the "you can now" message
            // This shows the specific creature and mood
            call_power_desc = get_call_power_description();
            god_power power(spec.piety_rank, spec.ability, call_power_desc.c_str());
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

bool has_degenerative_casting()
{
    if (you.religion != GOD_ANCIENT)
        return false;
    
    for (const auto& power_val : you.ancient_powers)
    {
        ancient_power_type power_type = static_cast<ancient_power_type>(power_val.get_int());
        if (power_type == PASSIVE_DEGENERATIVE_CASTING)
            return true;
    }
    return false;
}

// Helper function to check if a monster type is a spellcaster
static bool _is_monster_spellcaster(monster_type type)
{
    // Create a temporary monster to check spellcaster status
    // This is a bit hacky but necessary for the mood scaling
    monster temp_mon;
    temp_mon.type = type;
    temp_mon.set_hit_dice(mons_class_hit_dice(type));
    return temp_mon.is_actual_spellcaster();
}

// Helper function to get mood scaling factor
static int _get_mood_scaling_factor(monster_type type, const string& mood_name)
{
    // Find matching mood scaling factor
    for (size_t i = 0; i < ARRAYSZ(mood_scaling_data); ++i)
    {
        if (strcmp(mood_scaling_data[i].name, mood_name.c_str()) == 0)
        {
            // For furious mood, check if monster is a spellcaster
            if (strcmp(mood_name.c_str(), "furious") == 0)
            {
                return _is_monster_spellcaster(type)
                    ? mood_scaling_data[i].scaling_factor_spellcaster
                    : mood_scaling_data[i].scaling_factor;
            }
            else
            {
                return mood_scaling_data[i].scaling_factor;
            }
        }
    }
    return 100; // Default 1.0x scaling
}

// Ancient god ability cost calculations
int get_ancient_creature_call_piety_cost()
{
    if (!you.props.exists("ag_call_monster") || !you.props.exists("ag_call_mood"))
        return 4; // Fallback to reduced original cost
    
    const monster_type type = static_cast<monster_type>(you.props["ag_call_monster"].get_int());
    const int mood_idx = you.props["ag_call_mood"].get_int();
    
    // Safety check for mood index
    if (mood_idx < 0 || mood_idx >= static_cast<int>(ARRAYSZ(mood_data)))
        return 4; // Fallback
    
    const int base_hd = mons_class_hit_dice(type);
    
    // Scale piety cost from ~3 (HD 1) to ~42 (HD 25+) - reduced by ~30%
    // Formula: base_cost = 3 + (HD * 39/25) ≈ 3 + (HD * 1.56)
    const int base_cost = 3 + (base_hd * 39) / 25;
    const int add_cost = (base_cost + 1) / 2 + 1; // Same formula as generic_cost
    
    // Apply mood scaling using integer arithmetic
    const string& mood_name = mood_data[mood_idx].name;
    const int mood_scaling = _get_mood_scaling_factor(type, mood_name);
    const int scaled_base_cost = (base_cost * mood_scaling) / 100;
    
    return scaled_base_cost + random2avg(add_cost, 1);
}

string get_ancient_creature_call_cost_description()
{
    if (!you.props.exists("ag_call_monster") || !you.props.exists("ag_call_mood"))
        return "~3 Piety"; // Fallback
    
    const monster_type type = static_cast<monster_type>(you.props["ag_call_monster"].get_int());
    const int mood_idx = you.props["ag_call_mood"].get_int();
    
    // Safety check for mood index
    if (mood_idx < 0 || mood_idx >= static_cast<int>(ARRAYSZ(mood_data)))
        return "~3 Piety"; // Fallback
    
    const int base_hd = mons_class_hit_dice(type);
    const int base_cost = 3 + (base_hd * 39) / 25;
    const int add_cost = (base_cost + 1) / 2 + 1;
    
    // Apply mood scaling using integer arithmetic
    const string& mood_name = mood_data[mood_idx].name;
    const int mood_scaling = _get_mood_scaling_factor(type, mood_name);
    const int avg_cost = ((base_cost + add_cost / 2) * mood_scaling) / 100;
    
    return make_stringf("~%d Piety", avg_cost);
}

string get_ancient_creature_call_detailed_cost_description()
{
    if (!you.props.exists("ag_call_monster") || !you.props.exists("ag_call_mood"))
        return "----- (about 2% of your maximum possible piety)"; // Fallback
    
    const monster_type type = static_cast<monster_type>(you.props["ag_call_monster"].get_int());
    const int mood_idx = you.props["ag_call_mood"].get_int();
    
    // Safety check for mood index
    if (mood_idx < 0 || mood_idx >= static_cast<int>(ARRAYSZ(mood_data)))
        return "----- (about 2% of your maximum possible piety)"; // Fallback
    
    const int base_hd = mons_class_hit_dice(type);
    const int base_cost = 3 + (base_hd * 39) / 25;
    const int add_cost = (base_cost + 1) / 2 + 1;
    
    // Apply mood scaling using integer arithmetic
    const string& mood_name = mood_data[mood_idx].name;
    const int mood_scaling = _get_mood_scaling_factor(type, mood_name);
    const int avg_cost = ((base_cost + add_cost / 2) * mood_scaling) / 100;
    
    // Calculate percentage of maximum piety
    const int max_piety = piety_breakpoint(6); // 160 piety
    const int percentage = (avg_cost * 100 + max_piety / 2) / max_piety;
    
    return make_stringf("~%d (about %d%% of your maximum possible piety)", avg_cost, percentage);
}

spret cast_ancient_stabilise_mutation()
{
    mutation_type selected = choose_mutation_to_stabilise();
    if (selected == NUM_MUTATIONS)
    {
        mpr("You have no mutations that can be stabilised.");
        return spret::abort;
    }
    
    // Confirm the selection
    const string mut_name = mutation_name(selected);
        if (!yesno(make_stringf("Stabilise your %s mutation? This cannot be undone.",
                           mut_name.c_str()).c_str(), true, 'n'))
    {
        canned_msg(MSG_OK);
        return spret::abort;
    }
    
    // Stabilise the mutation
    you.stabilized_mutation[selected] = 1;
    
    mprf("Your %s mutation becomes stable and permanent!", mut_name.c_str());
    
    return spret::success;
}