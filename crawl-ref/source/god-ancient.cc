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

// Defines a themed set of monsters that can be summoned.
const ancient_summon_set ancient_summon_sets[] = {
    // --- Core Branch Sets ---
    {
        ANCIENT_SET_LAIR, "Lair", "Monsters from the Lair, with bonus beasts.",
        {
            {MONS_DEATH_YAK}, {MONS_DIRE_ELEPHANT}, {MONS_SKYSHARK},
            {MONS_TORPOR_SNAIL}, {MONS_CATOBLEPAS}, {MONS_SPRIGGAN_DRUID},
            {MONS_HELLEPHANT}, {MONS_CAUSTIC_SHRIKE},
        }
    },
    {
        ANCIENT_SET_SNAKE, "Snake Pit", "Monsters from the Snake Pit, with bonus serpents.",
        {
            {MONS_NAGA_SHARPSHOOTER}, {MONS_GUARDIAN_SERPENT}, {MONS_SALAMANDER_TYRANT},
            {MONS_NAGARAJA}, {MONS_QUICKSILVER_DRAGON}, {MONS_NAGA_WARRIOR},
            {MONS_ANACONDA}, {MONS_BLACK_MAMBA},
        }
    },
    {
        ANCIENT_SET_SWAMP, "Swamp", "Monsters from the Swamp, with bonus horrors.",
        {
            {MONS_FENSTRIDER_WITCH}, {MONS_TENTACLED_MONSTROSITY}, {MONS_SHAMBLING_MANGROVE},
            {MONS_GOLDEN_DRAGON}, {MONS_GREEN_DEATH}, {MONS_HYDRA},
            {MONS_SWAMP_DRAKE}, {MONS_TYRANT_LEECH},
        }
    },
    {
        ANCIENT_SET_SHOALS, "Shoals", "Monsters from the Shoals, with bonus merfolk.",
        {
            {MONS_MERFOLK_IMPALER}, {MONS_MERFOLK_JAVELINEER}, {MONS_WATER_NYMPH},
            {MONS_MERFOLK_AQUAMANCER}, {MONS_KRAKEN}, {MONS_ELECTRIC_EEL},
            {MONS_MERFOLK}, {MONS_ALLIGATOR},
        }
    },
    {
        ANCIENT_SET_SPIDER, "Spider's Nest", "Monsters from the Spider's Nest, with bonus arachnids.",
        {
            {MONS_TORPOR_SNAIL}, {MONS_GHOST_MOTH}, {MONS_ENTROPY_WEAVER},
            {MONS_MOTH_OF_WRATH}, {MONS_EMPEROR_SCORPION}, {MONS_ORB_SPIDER},
            {MONS_TARANTELLA}, {MONS_REDBACK},
        }
    },
    {
        ANCIENT_SET_SLIME, "Slime Pits", "Monsters from the Slime Pits, with bonus oozes.",
        {
            {MONS_AZURE_JELLY}, {MONS_BROWN_OOZE}, {MONS_ROCKSLIME},
            {MONS_ACID_BLOB}, {MONS_GLASS_EYE}, {MONS_DEATH_OOZE},
            {MONS_SLIME_CREATURE}, {MONS_ROYAL_JELLY},
        }
    },
    {
        ANCIENT_SET_ORC, "Orcish Mines", "Monsters from the Orcish Mines, with bonus leaders.",
        {
            {MONS_ORC_WARLORD}, {MONS_ORC_HIGH_PRIEST}, {MONS_ORC_SORCERER},
            {MONS_STONE_GIANT}, {MONS_IRON_TROLL}, {MONS_JUGGERNAUT},
            {MONS_MOTH_OF_WRATH}, {MONS_UNDYING_ARMOURY},
        }
    },
    {
        ANCIENT_SET_ELF, "Elven Halls", "Monsters from the Elven Halls, with bonus elites.",
        {
            {MONS_DEEP_ELF_HIGH_PRIEST}, {MONS_DEEP_ELF_BLADEMASTER}, {MONS_DEEP_ELF_MASTER_ARCHER},
            {MONS_DEEP_ELF_ANNIHILATOR}, {MONS_DEEP_ELF_DEMONOLOGIST}, {MONS_DEEP_ELF_SORCERER},
            {MONS_DEEP_ELF_KNIGHT}, {MONS_SPRIGGAN_AIR_MAGE},
        }
    },
    {
        ANCIENT_SET_VAULTS, "Vaults", "Monsters from the Vaults, with bonus guardians.",
        {
            {MONS_TITAN}, {MONS_GOLDEN_DRAGON}, {MONS_SPHINX},
            {MONS_ANCIENT_LICH}, {MONS_DREAD_LICH}, {MONS_VAULT_WARDEN},
            {MONS_VAULT_GUARD}, {MONS_IRONBOUND_CONVOKER},
        }
    },
    {
        ANCIENT_SET_CRYPT, "Crypt", "Monsters from the Crypt, with bonus undead.",
        {
            {MONS_DREAD_LICH}, {MONS_ANCIENT_LICH}, {MONS_REVENANT},
            {MONS_CURSE_SKULL}, {MONS_BONE_DRAGON}, {MONS_FLAYED_GHOST},
            {MONS_WRAITH}, {MONS_SKELETAL_WARRIOR},
        }
    },
    {
        ANCIENT_SET_TOMB, "Tomb", "Monsters from the Tomb, with bonus royalty.",
        {
            {MONS_BENNU}, {MONS_MUMMY_PRIEST}, {MONS_ROYAL_MUMMY},
            {MONS_GUARDIAN_MUMMY}, {MONS_USHABTI}, {MONS_SPHINX},
            {MONS_DEATH_SCARAB},
        }
    },
    {
        ANCIENT_SET_ABYSS, "Abyss", "Monsters from the Abyss, with bonus horrors.",
        {
            {MONS_STARCURSED_MASS}, {MONS_WRETCHED_STAR}, {MONS_BONE_DRAGON},
            {MONS_LICH}, {MONS_DEMONSPAWN_CORRUPTER}, {MONS_TENTACLED_MONSTROSITY},
            {MONS_NEQOXEC}, {MONS_CHAOS_SPAWN},
        }
    },
    // --- Hell Branch Sets ---
    {
        ANCIENT_SET_COCYTUS, "Cocytus", "Monsters from Cocytus, with bonus fiends.",
        {
            {MONS_ICE_FIEND}, {MONS_SHARD_SHRIKE}, {MONS_TITAN},
            {MONS_FROST_GIANT}, {MONS_ICE_DEVIL}, {MONS_BLIZZARD_DEMON},
            {MONS_ICE_BEAST}, {MONS_SIMULACRUM},
        }
    },
    {
        ANCIENT_SET_GEHENNA, "Gehenna", "Monsters from Gehenna, with bonus fiends.",
        {
            {MONS_BRIMSTONE_FIEND}, {MONS_HELLION}, {MONS_HELLEPHANT},
            {MONS_BALRUG}, {MONS_FIRE_GIANT}, {MONS_SALAMANDER_TYRANT},
            {MONS_SMOKE_DEMON}, {MONS_HELL_HOUND},
        }
    },
    {
        ANCIENT_SET_DIS, "Iron City of Dis", "Monsters from Dis, with bonus constructs.",
        {
            {MONS_HELL_SENTINEL}, {MONS_IRON_DRAGON}, {MONS_IRON_GIANT},
            {MONS_IRON_TROLL}, {MONS_WAR_GARGOYLE}, {MONS_ELECTRIC_GOLEM},
            {MONS_IRON_IMP}, {MONS_IRONBOUND_CONVOKER},
        }
    },
    {
        ANCIENT_SET_TARTARUS, "Tartarus", "Monsters from Tartarus, with bonus horrors.",
        {
            {MONS_TZITZIMITL}, {MONS_CURSE_TOE}, {MONS_DOOM_HOUND},
            {MONS_SILENT_SPECTRE}, {MONS_REAPER}, {MONS_DEATH_KNIGHT},
            {MONS_SHADOW_WRAITH}, {MONS_BONE_DRAGON},
        }
    },
    {
        ANCIENT_SET_PANDEMONIUM, "Pandemonium", "Monsters from Pandemonium, with bonus demons.",
        {
            {MONS_EXECUTIONER}, {MONS_GREEN_DEATH}, {MONS_BLIZZARD_DEMON},
            {MONS_BALRUG}, {MONS_CACODEMON}, {MONS_CHAOS_SPAWN},
            {MONS_HELLION}, {MONS_TORMENTOR},
        }
    },
    // --- Theme Sets ---
    {
        ANCIENT_SET_GIANTS, "Giants", "Summons a variety of giant-kin.",
        {
            {MONS_CYCLOPS}, {MONS_STONE_GIANT}, {MONS_FIRE_GIANT},
            {MONS_FROST_GIANT}, {MONS_ETTIN}, {MONS_PROTEAN_PROGENITOR},
            {MONS_TITAN}, {MONS_JUGGERNAUT},
        }
    },
    {
        ANCIENT_SET_DRAGONS, "Dragons", "Summons dragons, drakes and hydras.",
        {
            {MONS_FIRE_DRAGON}, {MONS_ICE_DRAGON}, {MONS_STORM_DRAGON},
            {MONS_SHADOW_DRAGON}, {MONS_IRON_DRAGON}, {MONS_GOLDEN_DRAGON},
            {MONS_QUICKSILVER_DRAGON}, {MONS_HYDRA},
        }
    },
    {
        ANCIENT_SET_DRACONIANS, "Draconians", "Summons a variety of draconians.",
        {
            {MONS_DRACONIAN}, {MONS_BLACK_DRACONIAN}, {MONS_GREEN_DRACONIAN},
            {MONS_PALE_DRACONIAN}, {MONS_PURPLE_DRACONIAN}, {MONS_RED_DRACONIAN},
            {MONS_DRACONIAN_KNIGHT}, {MONS_DRACONIAN_ANNIHILATOR},
        }
    },
    {
        ANCIENT_SET_ARCHERS, "Archers", "Summons ranged weapon specialists.",
        {
            {MONS_CENTAUR_WARRIOR}, {MONS_YAKTAUR_CAPTAIN}, {MONS_NAGA_SHARPSHOOTER},
            {MONS_MERFOLK_JAVELINEER}, {MONS_DEEP_ELF_MASTER_ARCHER}, {MONS_FAUN},
            {MONS_SATYR}, {MONS_THORN_HUNTER},
        }
    },
    {
        ANCIENT_SET_CONJURERS, "Conjurers", "Summons powerful spellcasters.",
        {
            {MONS_OGRE_MAGE}, {MONS_ORC_SORCERER}, {MONS_NAGA_MAGE},
            {MONS_TENGU_CONJURER}, {MONS_SPRIGGAN_AIR_MAGE}, {MONS_DEEP_ELF_ANNIHILATOR},
            {MONS_LICH}, {MONS_ANCIENT_LICH},
        }
    },
    {
        ANCIENT_SET_LAIR_RUNES, "Lair Branches", "Summons monsters from the Lair branches.",
        {
            {MONS_NAGARAJA}, {MONS_GUARDIAN_SERPENT}, {MONS_FENSTRIDER_WITCH},
            {MONS_TENTACLED_MONSTROSITY}, {MONS_MERFOLK_AQUAMANCER}, {MONS_WATER_NYMPH},
            {MONS_GHOST_MOTH}, {MONS_MOTH_OF_WRATH},
        }
    },
    {
        ANCIENT_SET_VESTIBULE, "Vestibule of Hell", "Summons monsters from the Vestibule of Hell.",
        {
            {MONS_HELLION}, {MONS_TORMENTOR}, {MONS_HELL_HOUND},
            {MONS_SEARING_WRETCH}, {MONS_SHARD_SHRIKE}, {MONS_SILENT_SPECTRE},
            {MONS_SMOKE_DEMON}, {MONS_IRON_IMP},
        }
    },
    // --- Elemental Sets ---
    {
        ANCIENT_SET_HOLY, "Holy", "Summons divine and holy creatures.",
        {
            {MONS_DAEVA}, {MONS_ANGEL}, {MONS_CHERUB},
            {MONS_PEARL_DRAGON}, {MONS_OPHAN}, {MONS_SERAPH},
            {MONS_APIS}, {MONS_HOLY_SWINE},
        }
    },
    {
        ANCIENT_SET_FIRE, "Fire", "Summons fire elementals and creatures.",
        {
            {MONS_EFREET}, {MONS_FIRE_CRAB}, {MONS_HELL_KNIGHT},
            {MONS_WILL_O_THE_WISP}, {MONS_SALAMANDER_TYRANT}, {MONS_BALRUG},
            {MONS_RED_DRACONIAN}, {MONS_ORB_OF_FIRE},
        }
    },
    {
        ANCIENT_SET_ICE, "Ice", "Summons ice elementals and creatures.",
        {
            {MONS_RIME_DRAKE}, {MONS_FROST_GIANT}, {MONS_AZURE_JELLY},
            {MONS_ICE_DEVIL}, {MONS_BLIZZARD_DEMON}, {MONS_ICE_FIEND},
            {MONS_SIMULACRUM}, {MONS_SHARD_SHRIKE},
        }
    },
    {
        ANCIENT_SET_AIR, "Air", "Summons air elementals and creatures.",
        {
            {MONS_AIR_ELEMENTAL}, {MONS_TITAN}, {MONS_RAIJU},
            {MONS_STORM_DRAGON}, {MONS_ELECTRIC_GOLEM}, {MONS_SPRIGGAN_AIR_MAGE},
            {MONS_SPARK_WASP}, {MONS_SHOCK_SERPENT},
        }
    },
    {
        ANCIENT_SET_EARTH, "Earth", "Summons earth elementals and creatures.",
        {
            {MONS_GARGOYLE}, {MONS_WAR_GARGOYLE}, {MONS_EARTH_ELEMENTAL},
            {MONS_BOULDER_BEETLE}, {MONS_TORPOR_SNAIL}, {MONS_IRON_GOLEM},
            {MONS_CAUSTIC_SHRIKE}, {MONS_ENTROPY_WEAVER},
        }
    },
    {
        ANCIENT_SET_NEGATIVE, "Negative Energy", "Summons negative energy creatures.",
        {
            {MONS_SOUL_EATER}, {MONS_PHANTASMAL_WARRIOR}, {MONS_DEEP_ELF_DEATH_MAGE},
            {MONS_SHADOW_DRAGON}, {MONS_GHOST_CRAB}, {MONS_EIDOLON},
            {MONS_REVENANT}, {MONS_CURSE_SKULL},
        }
    },
    {
        ANCIENT_SET_CHAOS, "Chaos", "Summons chaotic creatures.",
        {
            {MONS_CHAOS_SPAWN}, {MONS_VERY_UGLY_THING}, {MONS_APOCALYPSE_CRAB},
            {MONS_GLOWING_SHAPESHIFTER}, {MONS_PROTEAN_PROGENITOR}, {MONS_KILLER_KLOWN},
            {MONS_NEQOXEC}, {MONS_ORANGE_STATUE},
        }
    },
    // --- Other Sets ---
    {
        ANCIENT_SET_EYE_SOCIETY, "The Eye Society", "Summons creatures obsessed with vision and sight.",
        {
            {MONS_EYE_OF_DRAINING}, {MONS_GOLDEN_EYE}, {MONS_SHINING_EYE},
            {MONS_EYE_OF_DEVASTATION}, {MONS_GREAT_ORB_OF_EYES}, {MONS_FLOATING_EYE},
            {MONS_GLASS_EYE}, {MONS_CYCLOPS},
        }
    },
    {
        ANCIENT_SET_UNDEAD_HORDE, "Undead Horde", "Summons a variety of undead.",
        {
            {MONS_GHOUL}, {MONS_SKELETAL_WARRIOR}, {MONS_WRAITH},
            {MONS_FLAYED_GHOST}, {MONS_BONE_DRAGON}, {MONS_LICH},
            {MONS_REVENANT}, {MONS_CURSE_SKULL},
        }
    },
    {
        ANCIENT_SET_THE_SWARM, "The Swarm", "Summons a swarm of smaller creatures.",
        {
            {MONS_KILLER_BEE}, {MONS_PHARAOH_ANT}, {MONS_RAT},
            {MONS_KOBOLD}, {MONS_SPIDER}, {MONS_VAMPIRE_MOSQUITO},
            {MONS_GIANT_COCKROACH}, {MONS_BUTTERFLY},
        }
    },
    {
        ANCIENT_SET_ARTIFICIAL_BEINGS, "Artificial Beings", "Summons constructs and other artificial beings.",
        {
            {MONS_DANCING_WEAPON}, {MONS_GARGOYLE}, {MONS_WAR_GARGOYLE},
            {MONS_IRON_GOLEM}, {MONS_ELECTRIC_GOLEM}, {MONS_CRYSTAL_GUARDIAN},
            {MONS_IRON_IMP}, {MONS_HELL_SENTINEL},
        }
    },
    {
        ANCIENT_SET_MYSTICAL_CREATURES, "Mystical Creatures", "Summons a variety of magical beasts.",
        {
            {MONS_MANTICORE}, {MONS_SPHINX}, {MONS_HIPPOGRIFF},
            {MONS_CATOBLEPAS}, {MONS_BASILISK}, {MONS_WYVERN},
            {MONS_RAKSHASA}, {MONS_NEKOMATA},
        }
    },
};



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
            you.props[AG_CALL_MOOD_KEY] = mood_idx;
        }

        {
            // Assign a random monster set for creature summoning
            rng::subgenerator subgen_call_set(you.game_seed, 11);
            you.props[AG_CALL_MONSTER_SET_KEY] = random2(NUM_ANCIENT_SETS);
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
    if (!you.props.exists(AG_CALL_MONSTER_SET_KEY) || !you.props.exists(AG_CALL_MOOD_KEY))
    {
        return "Call of Monsters"; // fallback description
    }
    
    const ancient_set_type set = static_cast<ancient_set_type>(you.props[AG_CALL_MONSTER_SET_KEY].get_int());
    const int mood_idx = you.props[AG_CALL_MOOD_KEY].get_int();

    // Safety checks
    if (set < 0 || set >= NUM_ANCIENT_SETS || mood_idx < 0 || mood_idx >= static_cast<int>(ARRAYSZ(mood_data)))
    {
        return "Call of Monsters"; // fallback description
    }

    const string mood = mood_data[mood_idx].name;
    const string set_description = get_ancient_set_description(set);

    // Use a more descriptive format that clearly indicates it's an ability
    // Format: "Call Mighty Earth Elementals" instead of individual monster names
    return make_stringf("Call %s %s", mood.c_str(), get_ancient_set_name(set));
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
    if (!you.props.exists(AG_CALL_MONSTER_SET_KEY) || !you.props.exists(AG_CALL_MOOD_KEY))
        return 4; // Fallback to reduced original cost
    
    const ancient_set_type set = static_cast<ancient_set_type>(you.props[AG_CALL_MONSTER_SET_KEY].get_int());
    const int mood_idx = you.props[AG_CALL_MOOD_KEY].get_int();
    
    // Safety checks
    if (set < 0 || set >= NUM_ANCIENT_SETS || mood_idx < 0 || mood_idx >= static_cast<int>(ARRAYSZ(mood_data)))
        return 4; // Fallback
    
    // Calculate average HD from the monster set
    const auto& monsters = ancient_summon_sets[set].monsters;
    if (monsters.empty())
        return 4; // Fallback
    
    int total_hd = 0;
    int valid_monsters = 0;
    for (const auto& mon_def : monsters)
    {
        const int hd = get_monster_data(mon_def.type)->HD;
        if (hd > 0)
        {
            total_hd += hd;
            valid_monsters++;
        }
    }
    
    if (valid_monsters == 0)
        return 4; // Fallback
    
    const int avg_hd = total_hd / valid_monsters;
    
    // Scale piety cost from ~3 (HD 1) to ~42 (HD 25+)
    // Formula: base_cost = 3 + (HD * 39/25) ≈ 3 + (HD * 1.56)
    const int base_cost = 3 + (avg_hd * 39) / 25;
    const int add_cost = (base_cost + 1) / 2 + 1; // Same formula as generic_cost
    
    // Apply mood scaling using integer arithmetic
    // Use a representative monster from the set for mood scaling
    const monster_type representative_type = monsters[0].type;
    const string& mood_name = mood_data[mood_idx].name;
    const int mood_scaling = _get_mood_scaling_factor(representative_type, mood_name);
    const int scaled_base_cost = (base_cost * mood_scaling) / 100;
    
    return scaled_base_cost + random2avg(add_cost, 1);
}

string get_ancient_creature_call_cost_description()
{
    if (!you.props.exists(AG_CALL_MONSTER_SET_KEY) || !you.props.exists(AG_CALL_MOOD_KEY))
        return "~3 Piety"; // Fallback
    
    const ancient_set_type set = static_cast<ancient_set_type>(you.props[AG_CALL_MONSTER_SET_KEY].get_int());
    const int mood_idx = you.props[AG_CALL_MOOD_KEY].get_int();
    
    // Safety checks
    if (set < 0 || set >= NUM_ANCIENT_SETS || mood_idx < 0 || mood_idx >= static_cast<int>(ARRAYSZ(mood_data)))
        return "~3 Piety"; // Fallback
    
    // Calculate average HD from the monster set
    const auto& monsters = ancient_summon_sets[set].monsters;
    if (monsters.empty())
        return "~3 Piety"; // Fallback
    
    int total_hd = 0;
    int valid_monsters = 0;
    for (const auto& mon_def : monsters)
    {
        const int hd = get_monster_data(mon_def.type)->HD;
        if (hd > 0)
        {
            total_hd += hd;
            valid_monsters++;
        }
    }
    
    if (valid_monsters == 0)
        return "~3 Piety"; // Fallback
    
    const int avg_hd = total_hd / valid_monsters;
    const int base_cost = 3 + (avg_hd * 39) / 25;
    const int add_cost = (base_cost + 1) / 2 + 1;
    
    // Apply mood scaling using integer arithmetic
    const monster_type representative_type = monsters[0].type;
    const string& mood_name = mood_data[mood_idx].name;
    const int mood_scaling = _get_mood_scaling_factor(representative_type, mood_name);
    const int avg_cost = ((base_cost + add_cost / 2) * mood_scaling) / 100;
    
    return make_stringf("~%d Piety", avg_cost);
}

string get_ancient_creature_call_detailed_cost_description()
{
    if (!you.props.exists(AG_CALL_MONSTER_SET_KEY) || !you.props.exists(AG_CALL_MOOD_KEY))
        return "----- (about 2% of your maximum possible piety)"; // Fallback
    
    const ancient_set_type set = static_cast<ancient_set_type>(you.props[AG_CALL_MONSTER_SET_KEY].get_int());
    const int mood_idx = you.props[AG_CALL_MOOD_KEY].get_int();
    
    // Safety checks
    if (set < 0 || set >= NUM_ANCIENT_SETS || mood_idx < 0 || mood_idx >= static_cast<int>(ARRAYSZ(mood_data)))
        return "----- (about 2% of your maximum possible piety)"; // Fallback
    
    // Calculate average HD from the monster set
    const auto& monsters = ancient_summon_sets[set].monsters;
    if (monsters.empty())
        return "----- (about 2% of your maximum possible piety)"; // Fallback
    
    int total_hd = 0;
    int valid_monsters = 0;
    for (const auto& mon_def : monsters)
    {
        const int hd = get_monster_data(mon_def.type)->HD;
        if (hd > 0)
        {
            total_hd += hd;
            valid_monsters++;
        }
    }
    
    if (valid_monsters == 0)
        return "----- (about 2% of your maximum possible piety)"; // Fallback
    
    const int avg_hd = total_hd / valid_monsters;
    const int base_cost = 3 + (avg_hd * 39) / 25;
    const int add_cost = (base_cost + 1) / 2 + 1;
    
    // Apply mood scaling using integer arithmetic
    const monster_type representative_type = monsters[0].type;
    const string& mood_name = mood_data[mood_idx].name;
    const int mood_scaling = _get_mood_scaling_factor(representative_type, mood_name);
    const int avg_cost = ((base_cost + add_cost / 2) * mood_scaling) / 100;
    
    // Calculate percentage of maximum piety
    const int max_piety = piety_breakpoint(6); // 200 piety
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


const char* get_ancient_set_name(ancient_set_type set)
{
    if (set >= 0 && set < NUM_ANCIENT_SETS)
        return ancient_summon_sets[set].name;
    return "Unknown";
}

const char* get_ancient_set_description(ancient_set_type set)
{
    if (set >= 0 && set < NUM_ANCIENT_SETS)
        return ancient_summon_sets[set].description;
    return "Unknown creature type.";
}

