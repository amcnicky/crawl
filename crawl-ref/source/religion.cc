/**
 * @file
 * @brief Misc religion related functions.
**/

#include "AppHdr.h"

#include "religion.h"
#include "ancient-one.h" // Include the new header

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <sstream>

#include "ability.h"
#include "acquire.h"
#include "act-iter.h"
#include "areas.h"
#include "attitude-change.h"
#include "branch.h"
#include "chardump.h"
#include "coordit.h"
#include "dactions.h"
#include "database.h"
#include "decks.h"
#include "delay.h"
#include "describe-god.h"
#include "dgn-event.h"
#include "dlua.h"
#include "english.h"
#include "env.h"
#include "fineff.h"
#include "god-abil.h"
#include "god-companions.h"
#include "god-conduct.h"
#include "god-item.h"
#include "god-passive.h"
#include "god-prayer.h"
#include "god-wrath.h"
#include "hints.h"
#include "hiscores.h"
#include "invent.h"
#include "item-name.h"
#include "item-prop.h"
#include "item-status-flag-type.h"
#include "items.h"
#include "level-state-type.h"
#include "libutil.h"
#include "makeitem.h"
#include "message.h"
#include "mon-death.h"
#include "mon-gear.h" // give_shield
#include "mon-pick.h"
#include "mon-place.h"
#include "mon-tentacle.h"
#include "mutation.h"
#include "nearby-danger.h"
#include "notes.h"
#include "output.h"
#include "player-equip.h"
#include "player-stats.h"
#include "prompt.h"
#include "randbook.h"
#include "shopping.h"
#include "skills.h"
#include "spl-book.h"
#include "spl-summoning.h"
#include "sprint.h"
#include "state.h"
#include "stringutil.h"
#include "tag-version.h"
#include "terrain.h"
#include "transform.h"
#include "view.h"

#ifdef DEBUG_RELIGION
#    define DEBUG_DIAGNOSTICS
#    define DEBUG_GIFTS
#    define DEBUG_SACRIFICE
#    define DEBUG_PIETY
#endif

#define PIETY_HYSTERESIS_LIMIT 1

#define MIN_IGNIS_PIETY_KEY "min_ignis_piety"
#define YRED_SEEN_ZOMBIE_KEY "yred_seen_zombie"

static weapon_type _hepliaklqana_weapon_type(monster_type mc, int HD);
static brand_type _hepliaklqana_weapon_brand(monster_type mc, int HD);
static armour_type _hepliaklqana_shield_type(monster_type mc, int HD);
static special_armour_type _hepliaklqana_shield_ego(int HD);

const vector<vector<god_power>> & get_all_god_powers()
{
    static vector<vector<god_power>> god_powers =
    {
        // no god
        { },

        // Zin
        {   { 1, ABIL_ZIN_RECITE, "recite Zin's Axioms of Law" },
            { 2, ABIL_ZIN_VITALISATION, "call upon Zin for vitalisation" },
            { 3, ABIL_ZIN_IMPRISON, "call upon Zin to imprison the lawless" },
            { 5, ABIL_ZIN_SANCTUARY, "call upon Zin to create a sanctuary" },
            { 6, "Zin will now cleanse your potions of mutation.",
                 "Zin will no longer cleanse your potions of mutation.",
                 "Zin will cleanse your potions of mutation." },
            {-1, ABIL_ZIN_DONATE_GOLD, "donate money to Zin" },
        },

        // TSO
        {   { 1, "You and your allies can now gain power from killing the unholy and evil.",
                 "You and your allies can no longer gain power from killing the unholy and evil.",
                 "You and your allies can gain power from killing the unholy and evil." },
            { 1, ABIL_TSO_DIVINE_SHIELD, "call upon the Shining One for a divine shield" },
            { 3, ABIL_TSO_CLEANSING_FLAME, "channel blasts of cleansing flame", },
            { 5, ABIL_TSO_SUMMON_DIVINE_WARRIOR, "summon a divine warrior" },
            { 7, ABIL_TSO_BLESS_WEAPON,
                 "The Shining One will bless your weapon with holy wrath... once.",
                 "The Shining One is no longer ready to bless your weapon." },
        },

        // Kikubaaqudgha
        {
            { 1, ABIL_KIKU_UNEARTH_WRETCHES, "bring forth the wretched and dying" },
            { 2, "Kikubaaqudgha is now protecting you from necromantic miscasts and death curses.",
                 "Kikubaaqudgha will no longer protect you from necromantic miscasts or death curses.",
                 "Kikubaaqudgha protects you from necromantic miscasts and death curses." },
            { 4, "Kikubaaqudgha is now protecting you from unholy torment.",
                 "Kikubaaqudgha will no longer protect you from unholy torment.",
                 "Kikubaaqudgha protects you from unholy torment." },
            { 5, ABIL_KIKU_SIGN_OF_RUIN, "invoke the sign of ruin" },
            { 7, ABIL_KIKU_BLESS_WEAPON,
                 "Kikubaaqudgha will grant you forbidden knowledge or bloody your weapon with pain... once.",
                 "Kikubaaqudgha is no longer ready to enhance your necromancy." },
            { 7, ABIL_KIKU_GIFT_CAPSTONE_SPELLS,
                 "Kikubaaqudgha will grant you forbidden knowledge.",
                 "Kikubaaqudgha is no longer ready to enhance your necromancy." },
        },

        // Yredelemnul
        {   { 0, ABIL_YRED_LIGHT_THE_TORCH, "light the black torch and reap souls in Yredelemnul's name" },
            { 0, ABIL_YRED_RECALL_UNDEAD_HARVEST, "recall your undead harvest" },
            { 1, "Yredelemnul will now gift you temporary undead servants when you light the torch.",
                 "Yredelemnul will no longer gift you temporary undead servants when you light the torch.",
                 "Yredelemnul will gift you temporary undead servants when you light the torch." },
            { 2, ABIL_YRED_HURL_TORCHLIGHT, "hurl gouts of umbral torchlight" },
            { 4, ABIL_YRED_BIND_SOUL, "bind living souls" },
            { 5, ABIL_YRED_FATHOMLESS_SHACKLES, "engulf your surroundings in Yredelemnul's grip" },
        },

        // Xom
        { },

        // Vehumet
        {   { 1, "gain magical power from killing" },
            { 3, "Vehumet is now aiding your destructive spells.",
                 "Vehumet will no longer aid your destructive spells.",
                 "Vehumet aids your destructive spells." },
            { 4, "Vehumet is now extending the range of your destructive spells.",
                 "Vehumet will no longer extend the range of your destructive spells.",
                 "Vehumet extends the range of your destructive spells." },
        },

        // Okawaru
        {
            {-1, "", "", "Okawaru requires that you fight alone, and prevents you from gaining allies." },
            { 1, ABIL_OKAWARU_HEROISM, "gain great but temporary skills" },
            { 4, ABIL_OKAWARU_FINESSE, "speed up your combat" },
            { 5, ABIL_OKAWARU_DUEL, "enter into single combat with a foe"},
            { 5, "Okawaru will now gift you throwing weapons as you gain piety.",
                 "Okawaru will no longer gift you throwing weapons.",
                 "Okawaru will gift you throwing weapons as you gain piety." },
            { 6, ABIL_OKAWARU_GIFT_WEAPON,
                 "Okawaru will grant you a choice of weapons... once.",
                 "Okawaru is no longer ready to gift you weaponry." },
            { 6, ABIL_OKAWARU_GIFT_ARMOUR,
                 "Okawaru will grant you a choice of armour... once.",
                 "Okawaru is no longer ready to gift you armour." },
        },

        // Makhleb
        {   { 1, "gain health from killing" },
            { 2, ABIL_MAKHLEB_DESTRUCTION,
                 "unleash Makhleb's destructive might" },
            { 3, ABIL_MAKHLEB_INFERNAL_SERVANT,
                 "summon an infernal servant of Makhleb" },
            { 4, "", ""},   // XXX: A marker, replaced by dynamic text in _describe_god_powers()
            { -1, ABIL_MAKHLEB_VESSEL_OF_SLAUGHTER, ""},
            { 7, ABIL_MAKHLEB_BRAND_SELF_1,
                 "Makhleb will allow you to brand your body with an infernal mark... once.",
                 "Mahkleb will no longer allow you to brand your body with an infernal mark."},
            { 7, ABIL_MAKHLEB_BRAND_SELF_2, ""},
            { 7, ABIL_MAKHLEB_BRAND_SELF_3, ""},
        },

        // Sif Muna
        {   { 1, ABIL_SIF_MUNA_CHANNEL_ENERGY,
                 "call upon Sif Muna for magical energy" },
            { 3, ABIL_SIF_MUNA_FORGET_SPELL,
                 "freely open your mind to new spells",
                 "forget spells at will" },
            { 4, ABIL_SIF_MUNA_DIVINE_EXEGESIS,
                 "call upon Sif Muna to cast any spell from your library" },
            { 5, "Sif Muna will now gift you books as you gain piety.",
                 "Sif Muna will no longer gift you books.",
                 "Sif Muna will gift you books as you gain piety." },
        },

        // Trog
        {
            { 1, ABIL_TROG_BERSERK, "go berserk at will" },
            { 2, ABIL_TROG_HAND,
                 "call upon Trog for regeneration and willpower" },
            { 4, ABIL_TROG_BROTHERS_IN_ARMS, "call in reinforcements" },
            { 5, "Trog will now gift you melee weapons as you gain piety.",
                 "Trog will no longer gift you weapons.",
                 "Trog will gift you melee weapons as you gain piety." },
        },

        // Nemelex
        {
            { 0, "draw from decks of power" },
            { 1, "Nemelex will now gift you decks of power as you gain piety.",
                 "Nemelex will no longer gift you decks.",
                 "Nemelex will gift you decks of power as you gain piety." },
            { 3, ABIL_NEMELEX_TRIPLE_DRAW, "choose one out of three cards" },
            { 4, ABIL_NEMELEX_DEAL_FOUR, "deal four cards at a time" },
            { 5, ABIL_NEMELEX_STACK_FIVE, "stack five cards from your decks",
                                        "stack cards" },
        },

        // Elyvilon
        {
            { 1, ABIL_ELYVILON_PURIFICATION, "purify yourself" },
            { 2, ABIL_ELYVILON_HEAL_OTHER, "heal and attempt to pacify others" },
            { 3, ABIL_ELYVILON_HEAL_SELF, "provide healing for yourself" },
            { 5, ABIL_ELYVILON_DIVINE_VIGOUR, "call upon Elyvilon for divine vigour" },
        },

        // Lugonu
        {   {-1, "", "", "Lugonu protects you from the effects of unwielding distortion weapons." },
            {-1, "", "", "Lugonu may banish nearby foes when other gods try to punish you." },
            { 1, ABIL_LUGONU_ABYSS_EXIT,
                 "depart the Abyss",
                 "depart the Abyss at will" },
            { 2, ABIL_LUGONU_BANISH, "banish your foes" },
            { 4, ABIL_LUGONU_CORRUPT, "corrupt the fabric of space" },
            { 5, ABIL_LUGONU_ABYSS_ENTER, "gate yourself to the Abyss" },
            { 7, ABIL_LUGONU_BLESS_WEAPON,
                 "Lugonu will corrupt your weapon with distortion... once.",
                 "Lugonu is no longer ready to corrupt your weapon." },
        },

        // Beogh
        {   { 2, ABIL_BEOGH_SMITING, "smite your foes" },
            { 1, ABIL_BEOGH_RECALL_APOSTLES, "recall your apostles" },
            { 3, "Beogh will now send orc apostles to challenge you in battle as you gain piety.",
                 "Beogh will no longer send orc apostles to challenge you in battle.",
                 "Beogh will send orc apostles to challenge you in battle as you gain piety." },
            { 0, "", "", "You can recruit apostles that you defeat into your service." },
            { 3, "", "", "Your apostles are sometimes healed when you deal damage." },
            { 5, ABIL_BEOGH_BLOOD_FOR_BLOOD, "rally a vengeful horde" },
            { 0, ABIL_BEOGH_RECRUIT_APOSTLE, "" },
            { 0, ABIL_BEOGH_DISMISS_APOSTLE_1, ""},
            { 0, ABIL_BEOGH_DISMISS_APOSTLE_2, ""},
            { 0, ABIL_BEOGH_DISMISS_APOSTLE_3, ""},
            { 5, "walk on water" },
        },

        // Jiyva
        {   { 2, "Jiyva is now protecting you from corrosive effects.",
                 "Jiyva will no longer protect you from corrosive effects.",
                 "Jiyva protects you from corrosive effects." },
            { 3, "Jiyva will now mutate your body as you gain piety.",
                 "Jiyva will no longer mutate your body.",
                 "Jiyva will mutate your body as you gain piety." },
            { 3, ABIL_JIYVA_OOZEMANCY, "call acidic ooze from nearby walls" },
            { 4, ABIL_JIYVA_SLIMIFY, "turn your foes to slime" },
            { 5, "You may now expel jellies when seriously injured.",
                 "You will no longer expel jellies when injured.",
                 "You may expel jellies when seriously injured." },
        },

        // Fedhas
        {
            { 0, "", "", "You can walk through plants and fire through allied plants." },
            { 2, ABIL_FEDHAS_WALL_OF_BRIARS, "encircle yourself with summoned briar patches"},
            { 3, ABIL_FEDHAS_GROW_BALLISTOMYCETE, "grow a ballistomycete" },
            { 4, ABIL_FEDHAS_OVERGROW, "transform dungeon walls and trees into plant allies"},
            { 5, ABIL_FEDHAS_GROW_OKLOB, "grow an oklob plant" },
        },

        // Cheibriados
        {   { 0, "Cheibriados is now slowing the effects of poison on you.",
                 "Cheibriados will no longer slow the effects of poison on you.",
                 "Cheibriados slows the effects of poison on you." },
            { 1, ABIL_CHEIBRIADOS_TIME_BEND, "bend time to slow others" },
            { 3, ABIL_CHEIBRIADOS_DISTORTION, "warp the flow of time around you" },
            { 4, ABIL_CHEIBRIADOS_SLOUCH, "inflict damage on those overly hasty" },
            { 5, ABIL_CHEIBRIADOS_TIME_STEP, "step out of the flow of time" },
        },

        // Ashenzari
        {   { 0, "Ashenzari warns you of distant threats and treasures.\n"
                 "Ashenzari shows you where magical portals lie." },
            { 1, "Ashenzari will now identify your possessions.",
                 "Ashenzari will no longer identify your possesions.",
                 "Ashenzari identifies your possessions." },
            { 2, "Ashenzari will now reveal the unseen.",
                 "Ashenzari will no longer reveal the unseen.",
                 "Ashenzari reveals the unseen." },
            { 3, "Ashenzari will now keep your mind clear.",
                 "Ashenzari will no longer keep your mind clear.",
                 "Ashenzari keeps your mind clear." },
            { 4, "Ashenzari will now protect you from the dungeon's malevolent forces.",
                 "Ashenzari no longer protects you from the dungeon's malevolent forces.",
                 "Ashenzari protects you from the dungeon's malevolent forces." },
            { 4, "Ashenzari reveals the structure of the nearby dungeon.",
                 "Ashenzari no longer reveals the structure of the nearby dungeon." },
        },

        // Dithmenos
        {   { 1, "Dithmenos quiets all noise in your surroundings.",
                 "Dithmenos no longer quiets all noise in your surroundings."},
            { 2, "Your shadow now sometimes tangibly mimics your actions.",
                 "Your shadow no longer tangibly mimics your actions.",
                 "Your shadow sometimes tangibly mimics your actions." },
            { 3, ABIL_DITHMENOS_SHADOWSLIP,
                 "mislead enemies by swapping places with your shadow" },
            { 4, ABIL_DITHMENOS_APHOTIC_MARIONETTE,
                 "unleash an enemy's spells as if they were a marionette"},
            { 5, ABIL_DITHMENOS_PRIMORDIAL_NIGHTFALL,
                 "surround yourself with impenetrable night"},
        },

        // Gozag
        {   { 0, "", "", "Gozag turns your defeated foes' bodies to gold." },
            { 0, "", "", "Your enemies may become distracted by gold." },
            { 0, ABIL_GOZAG_POTION_PETITION, "petition Gozag for potion effects" },
            { 0, ABIL_GOZAG_CALL_MERCHANT,
                 "fund merchants seeking to open stores in the dungeon" },
            { 0, ABIL_GOZAG_BRIBE_BRANCH,
                 "bribe branches to halt enemies' attacks and recruit allies" },
        },

        // Qazlal
        {
            { 0, "Qazlal grants you and your divine allies immunity to clouds." },
            { 1, "You are now surrounded by a storm.",
                 "Your storm dissipates completely.",
                 "You are surrounded by a storm." },
            { 2, ABIL_QAZLAL_UPHEAVAL, "call upon nature to destroy your foes" },
            { 3, ABIL_QAZLAL_ELEMENTAL_FORCE, "give life to nearby clouds" },
            { 4, "The storm surrounding you is now powerful enough to repel missiles.",
                 "The storm surrounding you is now too weak to repel missiles.",
                 "The storm surrounding you is powerful enough to repel missiles." },
            { 4, "You will now adapt resistances upon receiving elemental damage.",
                 "You will no longer adapt resistances upon receiving elemental damage.",
                 "You adapt resistances upon receiving elemental damage." },
            { 5, ABIL_QAZLAL_DISASTER_AREA,
                 "call upon nature's wrath in a wide area around you" },
        },

        // Ru
        {   { 1, "You now exude an aura of power that intimidates your foes.",
                 "You no longer exude an aura of power that intimidates your foes.",
                 "You now exude an aura of power that intimidates your foes." },
            { 2, "Your aura of power can now strike those that harm you.",
                 "Your aura of power no longer strikes those that harm you.",
                 "Your aura of power can strike those that harm you." },
            { 3, ABIL_RU_DRAW_OUT_POWER, "heal your body and restore your magic" },
            { 4, ABIL_RU_POWER_LEAP, "gather your power into a mighty leap" },
            { 5, ABIL_RU_APOCALYPSE, "wreak a terrible wrath on your foes" },
        },

#if TAG_MAJOR_VERSION == 34
        // Pakellas
        {
            { 0, "gain magical power from killing" },
        },
#endif

        // Uskayaw
        {
            { 1, ABIL_USKAYAW_STOMP, "stomp with the beat" },
            { 2, ABIL_USKAYAW_LINE_PASS, "pass through a line of other dancers" },
            { 3, "Uskayaw will force your foes to helplessly watch your dance.",
                 "Uskayaw will no longer force your foes to helplessly watch your dance."},
            { 4, "Uskayaw will force your foes to share their pain.",
                 "Uskayaw will no longer force your foes to share their pain."},
            { 5, ABIL_USKAYAW_GRAND_FINALE, "merge with and destroy a victim" },
        },

        // Hepliaklqana
        {   { 1, "", "", "Your ancestor manifests to aid you." },
            { 1, ABIL_HEPLIAKLQANA_RECALL, "recall your ancestor" },
            { 1, ABIL_HEPLIAKLQANA_IDENTITY, "remember your ancestor's identity" },
            { 3, ABIL_HEPLIAKLQANA_TRANSFERENCE, "swap creatures with your ancestor" },
            { 4, ABIL_HEPLIAKLQANA_IDEALISE, "heal and protect your ancestor" },
            { 5, "You now drain nearby creatures when transferring your ancestor.",
                 "You no longer drain nearby creatures when transferring your ancestor.",
                 "You drain nearby creatures when transferring your ancestor." },
        },

        // Wu Jian
        {   { 0, "perform damaging attacks by moving towards foes",
                 "perform lunging strikes" },
            { 1, "lightly attack monsters by moving around them",
                 "perform spinning attacks" },
            { 2, ABIL_WU_JIAN_WALLJUMP,
                 "perform airborne attacks" },
            { 3, ABIL_WU_JIAN_SERPENTS_LASH, "briefly move at supernatural speeds",
                 "move at supernatural speeds" },
            { 5, ABIL_WU_JIAN_HEAVENLY_STORM,
                 "summon a storm of heavenly clouds to empower your attacks",
                 "summon a storm of heavenly clouds" },
        },

        // Ignis
        {
            // It would be nice to specify explicitly that this is rF+
            // Unfortunately, including parentheses at the end here breaks
            // the UI on Webtiles.
            { 0, "", "", "You are resistant to fire." },
            { 1, ABIL_IGNIS_FIERY_ARMOUR, "armour yourself in flame" },
            { 1, ABIL_IGNIS_FOXFIRE, "call a swarm of foxfires against your foes" },
            { 7, ABIL_IGNIS_RISING_FLAME, "rocket upward and away, once" },
        },
    };
    static bool god_powers_init = false;

    if (!god_powers_init)
    {
        ASSERT(god_powers.size() == NUM_GODS);
        for (int i = 0; i < NUM_GODS; i++)
            for (auto &p : god_powers[i])
                p.god = static_cast<god_type>(i);
        god_powers_init = true;
    }
    return god_powers;
}

vector<god_power> get_god_powers(god_type god)
{
    // Handle dynamic powers for Ancient One
    if (god == GOD_ANCIENT_ONE)
        return get_ancient_one_powers();

    // Original logic for other gods
    vector<god_power> ret;
    for (const auto& power : get_all_god_powers()[god])
    {
        // hack :( don't show fake hp restore
        if (god == GOD_VEHUMET && power.rank == 1
            && you.has_mutation(MUT_HP_CASTING))
        {
            continue;
        }
        if (!(power.abil != ABIL_NON_ABILITY
              && fixup_ability(power.abil) == ABIL_NON_ABILITY))
        {
            ret.push_back(power);
        }
    }
    return ret;
}

/**
 * Print a description of getting/losing this power.
 *
 * @param gaining If true, use this->gain; otherwise, use this->loss.
 * @param fmt  A string containing "%s" that will be used as a format
 *             string with our string as parameter; it is not used if
 *             our string begins with a capital letter. IF THIS DOES
 *             NOT CONTAIN "%s", OR CONTAINS OTHER FORMAT SPECIFIERS,
 *             BEHAVIOUR IS UNDEFINED.
 * @return a string suitable for being read by the user.
 */
void god_power::display(bool gaining, const char* fmt) const
{
    // hack: don't mention the necronomicon alone unless it wasn't
    // already mentioned by the other message
    if (abil == ABIL_KIKU_GIFT_CAPSTONE_SPELLS
        && !you.has_mutation(MUT_NO_GRASPING))
    {
        return;
    }

    // Uskayaw uses short-time-scale piety where the gain/loss messasges
    // are not informative while running
    if (you.running && you_worship(GOD_USKAYAW))
        return;

    const char* str = gaining ? gain : loss;

    // Don't print empty messages about 'hidden' abilities
    if (str[0] == '\0')
        return;

    if (isupper(str[0]))
        god_speaks(you.religion, str);
    else
        god_speaks(you.religion, make_stringf(fmt, str).c_str());
}

static void _place_delayed_monsters();

bool is_evil_god(god_type god)
{
    return god == GOD_KIKUBAAQUDGHA
           || god == GOD_MAKHLEB
           || god == GOD_YREDELEMNUL
           || god == GOD_BEOGH
           || god == GOD_LUGONU;
}

bool is_good_god(god_type god)
{
    return god == GOD_ZIN
           || god == GOD_SHINING_ONE
           || god == GOD_ELYVILON;
}

bool is_chaotic_god(god_type god)
{
    return god == GOD_XOM
           || god == GOD_MAKHLEB
           || god == GOD_NEMELEX_XOBEH
           || god == GOD_LUGONU
           || god == GOD_JIYVA;
}

bool is_unknown_god(god_type god)
{
    return god == GOD_NAMELESS;
}

// Not appearing in new games, but still extant.
static bool _is_disabled_god(god_type god)
{
    switch (god)
    {
#if TAG_MAJOR_VERSION == 34
    // Disabled, pending a rework.
    case GOD_PAKELLAS:
        return true;
#endif
    default:
        return false;
    }
}

bool is_unavailable_god(god_type god)
{
    if (_is_disabled_god(god))
        return true;

    if (god == GOD_JIYVA && jiyva_is_dead())
        return true;

    if (god == GOD_IGNIS && ignis_is_dead())
        return true;

    return false;
}

bool god_has_name(god_type god)
{
    return god != GOD_NO_GOD && god != GOD_NAMELESS;
}

god_type random_god()
{
    god_type god;

    do
    {
        god = static_cast<god_type>(random2(NUM_GODS - 1) + 1);
    }
    while (is_unavailable_god(god));

    return god;
}


god_iterator::god_iterator() :
    i(0) { } // might be ok to start with GOD_ZIN instead?

god_iterator::operator bool() const
{
    return i < NUM_GODS;
}

god_type god_iterator::operator*() const
{
    if (i < NUM_GODS)
        return (god_type)i;
    else
        return GOD_NO_GOD;
}

god_type god_iterator::operator->() const
{
    return **this;
}

god_iterator& god_iterator::operator++()
{
    ++i;
    return *this;
}

god_iterator god_iterator::operator++(int)
{
    god_iterator copy = *this;
    ++(*this);
    return copy;
}

void maybe_clear_traitor(god_type god)
{
    const god_type betrayed_god = static_cast<god_type>(
                                    you.attribute[ATTR_TRAITOR]);
    if (betrayed_god == god ||
        is_good_god(betrayed_god) && is_good_god(god))
    {
        you.attribute[ATTR_TRAITOR] = 0;
        you.redraw_title = true;
    }
}

bool active_penance(god_type god)
{
    // Good gods only have active wrath when they hate your current god.
    return player_under_penance(god)
           && !is_unavailable_god(god)
           && god != GOD_ASHENZARI
           && god != GOD_GOZAG
           && god != GOD_RU
           && god != GOD_HEPLIAKLQANA
#if TAG_MAJOR_VERSION == 34
           && god != GOD_PAKELLAS
#endif
           && god != GOD_ELYVILON
           && (god == you.religion && !(is_good_god(god) || god == GOD_BEOGH)
               || god_hates_your_god(god, you.religion));
}

// True for gods whose wrath is passive and expires with XP gain.
bool xp_penance(god_type god)
{
    return player_under_penance(god)
           && !is_unavailable_god(god)
           && (god == GOD_ASHENZARI
               || god == GOD_GOZAG
               || god == GOD_HEPLIAKLQANA
#if TAG_MAJOR_VERSION == 34
               || god == GOD_PAKELLAS
#endif
               || god == GOD_ELYVILON)
           && god_hates_your_god(god, you.religion);
}

void dec_penance(god_type god, int val)
{
    if (val <= 0 || you.penance[god] <= 0)
        return;

#ifdef DEBUG_PIETY
    mprf(MSGCH_DIAGNOSTICS, "Decreasing penance by %d", val);
#endif
    if (you.penance[god] <= val)
    {
        const bool had_halo = have_passive(passive_t::halo);
        const bool had_umbra = have_passive(passive_t::umbra);

        you.penance[god] = 0;

        mark_milestone("god.mollify",
                       "mollified " + god_name(god) + ".");

        if (god == GOD_IGNIS)
        {
            simple_god_message(", with one final cry of rage, "
                               "burns out of existence.", false, god);
            add_daction(DACT_REMOVE_IGNIS_ALTARS);
        }
        else
        {
            const bool dead_jiyva = (god == GOD_JIYVA && jiyva_is_dead());
            simple_god_message(
                make_stringf(" seems mollified%s.",
                             dead_jiyva ? ", and vanishes" : "").c_str(),
                false, god);

            if (dead_jiyva)
                add_daction(DACT_JIYVA_DEAD);
        }


        take_note(Note(NOTE_MOLLIFY_GOD, god));

        if (you_worship(god))
        {
            // Redraw piety display and, in case the best skill is Invocations,
            // redraw the god title.
            you.redraw_title = true;

            // TSO's halo is once more available.
            if (!had_halo && have_passive(passive_t::halo))
            {
                mprf(MSGCH_GOD, "Your divine halo returns!");
                invalidate_agrid(true);
            }
            if (!had_umbra && have_passive(passive_t::umbra))
            {
                mprf(MSGCH_GOD, "Your aura of darkness returns!");
                invalidate_agrid(true);
            }
            if (have_passive(passive_t::sinv))
            {
                mprf(MSGCH_GOD, "Your vision regains its divine sight.");
                autotoggle_autopickup(false);
            }
            if (have_passive(passive_t::stat_boost))
            {
                simple_god_message(" restores the support of your attributes.");
                redraw_screen();
                update_screen();
                notify_stat_change();
            }
            if (have_passive(passive_t::storm_shield))
            {
                mprf(MSGCH_GOD, "A storm instantly forms around you!");
                you.redraw_armour_class = true; // also handles shields
            }
            // When you've worked through all your penance, you get
            // another chance to make hostile slimes strict neutral.

            if (have_passive(passive_t::neutral_slimes))
                add_daction(DACT_SLIME_NEW_ATTEMPT);

            if (have_passive(passive_t::friendly_plants)
                && env.forest_awoken_until)
            {
                // XXX: add a dact here & on-join to handle offlevel
                // awakened forests?
                for (monster_iterator mi; mi; ++mi)
                     mi->del_ench(ENCH_AWAKEN_FOREST);
            }

            if (god == GOD_BEOGH)
                beogh_end_ostracism();
        }
        else if (god == GOD_HEPLIAKLQANA)
        {
            calc_hp(); // frailty ends
            mprf(MSGCH_GOD, god, "Your full life essence returns.");
        }
    }
    else
    {
        you.penance[god] -= val;
        return;
    }

    // We only get this far if we just mollified a god.
    // If we just mollified a god, see if we have any angry gods left.
    // If we don't, clear the stored wrath / XP counter.
    god_iterator it;
    for (; it; ++it)
    {
        if (active_penance(*it))
            break;
    }

    maybe_clear_traitor(god);

    if (it)
        return;

    you.attribute[ATTR_GOD_WRATH_COUNT] = 0;
    you.attribute[ATTR_GOD_WRATH_XP] = 0;
}

void dec_penance(int val)
{
    dec_penance(you.religion, val);
}

// TODO: find out what this is duplicating & deduplicate it
static bool _need_water_walking()
{
    return you.ground_level() && !you.has_mutation(MUT_MERTAIL)
           && env.grid(you.pos()) == DNGN_DEEP_WATER;
}

static void _grant_temporary_waterwalk()
{
    mpr("Your water-walking will last only until you reach solid ground.");
    you.props[TEMP_WATERWALK_KEY] = true;
}

bool jiyva_is_dead()
{
    return you.royal_jelly_dead
           && !you_worship(GOD_JIYVA) && !you.penance[GOD_JIYVA];
}

bool ignis_is_dead()
{
    return you.worshipped[GOD_IGNIS]
        && !you_worship(GOD_IGNIS)
        && !you.penance[GOD_IGNIS];
}

/// Is there any penalty from your god for removing an amulet of faith?
bool faith_has_penalty()
{
    return !you.has_mutation(MUT_FAITH)
        && ignore_faith_reason().empty()
        && !you_worship(GOD_XOM)
        && !you_worship(GOD_USKAYAW)
        && !you_worship(GOD_NO_GOD);
}

/// Is an amulet of faith irrelevant to you while you worship your current god?
/// If so, what how would that god explain why?
string ignore_faith_reason()
{
    switch (you.religion)
    {
    case GOD_GOZAG:
        return " cares for nothing but gold!";
    case GOD_ASHENZARI:
        return " cares nothing for such trivial demonstrations of your faith.";
    case GOD_IGNIS:
        // XXX: would it be better to offer a discount..?
        return " already offers you all the fire that remains!";
    case GOD_RU:
        if (you.piety >= piety_breakpoint(5))
        {
            return " says: An ascetic of your devotion"
                   " has no use for such trinkets.";
        }
        break;
    default:
        break;
    }
    return "";
}

void set_penance_xp_timeout()
{
    if (you.attribute[ATTR_GOD_WRATH_XP] > 0)
        return;

    // TODO: make this more random?
    you.attribute[ATTR_GOD_WRATH_XP] +=
        max(div_rand_round(exp_needed(you.experience_level + 1)
                          - exp_needed(you.experience_level),
                          200),
            1);
}

static void _inc_penance(god_type god, int val)
{
    if (val <= 0)
        return;

    if (!player_under_penance(god))
    {
        god_acting gdact(god, true);

        take_note(Note(NOTE_PENANCE, god));

        const bool had_halo = have_passive(passive_t::halo);
        const bool had_umbra = have_passive(passive_t::umbra);

        you.penance[god] += val;
        you.penance[god] = min((uint8_t)MAX_PENANCE, you.penance[god]);

        if (had_halo && !have_passive(passive_t::halo))
        {
            mprf(MSGCH_GOD, god, "Your divine halo fades away.");
            invalidate_agrid();
        }
        if (had_umbra && !have_passive(passive_t::umbra))
        {
            mprf(MSGCH_GOD, god, "Your aura of darkness fades away.");
            invalidate_agrid();
        }

        if (will_have_passive(passive_t::water_walk)
            && _need_water_walking() && !have_passive(passive_t::water_walk))
        {
            _grant_temporary_waterwalk();
        }

        if (will_have_passive(passive_t::stat_boost))
        {
            redraw_screen();
            update_screen();
            notify_stat_change();
        }

        if (god == GOD_TROG)
        {
            if (you.duration[DUR_TROGS_HAND])
                trog_remove_trogs_hand();
            dismiss_divine_allies_fineff::schedule(GOD_TROG);
        }
        else if (god == GOD_ZIN)
        {
            if (you.duration[DUR_DIVINE_STAMINA])
                zin_remove_divine_stamina();
            if (env.sanctuary_time)
                remove_sanctuary();
        }
        else if (god == GOD_SHINING_ONE)
        {
            if (you.duration[DUR_DIVINE_SHIELD])
               you.duration[DUR_DIVINE_SHIELD] = 0;
            dismiss_divine_allies_fineff::schedule(GOD_SHINING_ONE);
        }
        else if (god == GOD_ELYVILON)
        {
            if (you.duration[DUR_DIVINE_VIGOUR])
                elyvilon_remove_divine_vigour();
        }
        else if (god == GOD_JIYVA)
        {
            if (you.duration[DUR_SLIMIFY])
                you.duration[DUR_SLIMIFY] = 0;
            if (you.duration[DUR_OOZEMANCY])
                jiyva_end_oozemancy();
            if (slime_wall_neighbour(you.pos()))
            {
                // lose slime wall immunity
                you.wield_change = true;
                you.redraw_armour_class = true;
            }
        }
        else if (god == GOD_QAZLAL)
        {
            // Can't use have_passive(passive_t::storm_shield) because we
            // just gained penance.
            if (you.piety >= piety_breakpoint(0))
            {
                mprf(MSGCH_GOD, god, "The storm surrounding you dissipates.");
                you.redraw_armour_class = true;
            }
            if (you.duration[DUR_QAZLAL_FIRE_RES])
            {
                mprf(MSGCH_DURATION, "Your resistance to fire fades away.");
                you.duration[DUR_QAZLAL_FIRE_RES] = 0;
            }
            if (you.duration[DUR_QAZLAL_COLD_RES])
            {
                mprf(MSGCH_DURATION, "Your resistance to cold fades away.");
                you.duration[DUR_QAZLAL_COLD_RES] = 0;
            }
            if (you.duration[DUR_QAZLAL_ELEC_RES])
            {
                mprf(MSGCH_DURATION,
                     "Your resistance to electricity fades away.");
                you.duration[DUR_QAZLAL_ELEC_RES] = 0;
            }
            if (you.duration[DUR_QAZLAL_AC])
            {
                mprf(MSGCH_DURATION,
                     "Your resistance to physical damage fades away.");
                you.duration[DUR_QAZLAL_AC] = 0;
                you.redraw_armour_class = true;
            }
        }
        else if (god == GOD_SIF_MUNA)
        {
            if (you.duration[DUR_CHANNEL_ENERGY])
                you.duration[DUR_CHANNEL_ENERGY] = 0;
#if TAG_MAJOR_VERSION == 34
            if (you.attribute[ATTR_DIVINE_ENERGY])
                you.attribute[ATTR_DIVINE_ENERGY] = 0;
#endif
        }
        else if (god == GOD_OKAWARU)
        {
            if (you.duration[DUR_HEROISM])
                okawaru_remove_heroism();
            if (you.duration[DUR_FINESSE])
                okawaru_remove_finesse();
        }
        else if (god == GOD_BEOGH)
            dismiss_divine_allies_fineff::schedule(GOD_BEOGH);
        else if (god == GOD_YREDELEMNUL)
        {
            you.props.erase(YRED_TORCH_POWER_KEY);
            you.duration[DUR_FATHOMLESS_SHACKLES] = 0;
            yred_end_blasphemy();
        }

        if (you_worship(god))
        {
            // Redraw piety display and, in case the best skill is Invocations,
            // redraw the god title.
            you.redraw_title = true;
        }
    }
    else
    {
        you.penance[god] += val;
        you.penance[god] = min((uint8_t)MAX_PENANCE, you.penance[god]);
    }

    set_penance_xp_timeout();
}

static void _inc_penance(int val)
{
    _inc_penance(you.religion, val);
}

static void _set_penance(god_type god, int val)
{
    you.penance[god] = val;
}

static void _set_wrath_penance(god_type god)
{
    _set_penance(god, initial_wrath_penance_for(god));
}

static void _inc_gift_timeout(int val)
{
    if (200 - you.gift_timeout < val)
        you.gift_timeout = 200;
    else
        you.gift_timeout += val;
}

// These are sorted in order of power.
// monsters here come from genera: n, z, V and W
// - Vampire mages are excluded because they worship scholarly Kiku
// - M genus is all Kiku's domain
// - Curse *, putrid mouths, and bloated husks left out as they might
//   do too much collateral damage
static const vector<random_pick_entry<monster_type>> _yred_servants =
{
  { -2,  5,   80, PEAK, MONS_NECROPHAGE },
  { -1,  7,   75, PEAK, MONS_PHANTOM },
  {  2,  9,   70, SEMI, MONS_MARROWCUDA },
  {  4,  11,  145, SEMI, MONS_WIGHT },
  {  6,  13,  90, SEMI, MONS_SHADOWGHAST },
  {  8,  15,  110, SEMI, MONS_WRAITH },
  {  9,  15,  90, SEMI, MONS_VAMPIRE },
  { 10,  16,  110, SEMI, MONS_FREEZING_WRAITH },
  { 13,  20,  150, SEMI, MONS_SKELETAL_WARRIOR },
  { 13,  20,  75, SEMI, MONS_SHADOW_WRAITH },
  { 14,  20,  125, SEMI, MONS_PHANTASMAL_WARRIOR },
  { 13,  22,  60, FLAT, MONS_FLAYED_GHOST },
  { 13,  22,  125, SEMI, MONS_LAUGHING_SKULL },
  { 14,  21,  120, SEMI, MONS_BOG_BODY },
  { 16,  23,  180, SEMI, MONS_JIANGSHI },
  { 18,  25,  120, SEMI, MONS_EIDOLON },
  { 20,  25,  120, SEMI, MONS_VAMPIRE_KNIGHT },
  { 20,  25,  150, SEMI, MONS_GHOUL },
  { 20,  27,   80, SEMI, MONS_REVENANT_SOULMONGER },
  { 22,  27,   60, FLAT, MONS_DEATH_COB },
  { 23,  27,  180, SEMI, MONS_ANCIENT_CHAMPION },
  { 24,  30,  110, SEMI, MONS_SEARING_WRETCH },
  { 24,  30,  220, SEMI, MONS_PROFANE_SERVITOR },
  { 25,  32,  120, SEMI, MONS_BONE_DRAGON },
};

bool yred_random_servant(unsigned int pow, bool force_hostile, int num)
{
    monster_picker yred_picker;

    monster_type mon_type = yred_picker.pick(_yred_servants, pow, MONS_NECROPHAGE);

    if (mon_type == MONS_WIGHT && x_chance_in_y(pow, 8))
        num *= 2;
    else if (mon_type == MONS_SKELETAL_WARRIOR && x_chance_in_y(pow, 17))
        num *= 2;
    else if (mon_type == MONS_LAUGHING_SKULL)
        num *= random_range(2, 3);

    bool created = false;
    for (int i = 0; i < num; ++i)
    {
        mgen_data mg(mon_type, !force_hostile ? BEH_FRIENDLY : BEH_HOSTILE,
                 you.pos(), MHITYOU, MG_AUTOFOE, GOD_YREDELEMNUL);

        if (force_hostile)
        {
            mg.set_summoned(nullptr, MON_SUMM_WRATH);
            mg.non_actor_summoner = "the anger of Yredelemnul";
            mg.extra_flags |= (MF_NO_REWARD | MF_HARD_RESET);
            mg.set_range(2, you.current_vision);
        }
        else
            mg.set_summoned(&you, MON_SUMM_AID, summ_dur(5));

        if (create_monster(mg))
            created = true;
    }

    return created;
}

bool yred_reap_chance()
{
    // Count how many reaped allies we already have.
    int hd = 0;
    for (monster_iterator mi; mi; ++mi)
    {
        if (!mi->friendly())
            continue;

        if ((mi->type == MONS_ZOMBIE || mi->type == MONS_SPECTRAL_THING)
             && mi->was_created_by(you, MON_SUMM_YRED_REAP))
        {
            hd += mi->get_experience_level();
        }
    }

    // Always reap if we have no minions. Otherwise, use a sliding scale based
    // on total HD of allies, with a rate boosted by piety.
    if (hd == 0)
        return true;

    // Minimum chance scales from 15% at 0 piety to 40% at 6 stars
    int ratio = min(piety_breakpoint(5), (int)you.piety) * 100 / piety_breakpoint(5);
    int min_chance = 15 + (25 * ratio / 100);

    ratio = min(100, (hd * 100 / you.piety * 4));
    int chance = (ratio * min_chance / 100) + ((100 - ratio));

    return x_chance_in_y(chance, 100);
}

static bool _want_nemelex_gift()
{
    if (you.piety < piety_breakpoint(0))
        return false;
    const int piety_over_one_star = you.piety - piety_breakpoint(0);

    // Nemelex will give at least one gift early.
    if (!you.num_total_gifts[GOD_NEMELEX_XOBEH]
        && x_chance_in_y(piety_over_one_star + 1, piety_breakpoint(1)))
    {
        return true;
    }

    return one_chance_in(3) && x_chance_in_y(piety_over_one_star + 1, MAX_PIETY);
}

static bool _give_nemelex_gift(bool forced = false)
{
    if (!forced && !_want_nemelex_gift())
        return false;

    if (gift_cards())
    {
        simple_god_message(" deals you some cards!");
        mprf(MSGCH_GOD, "You now have %s.", deck_summary().c_str());
    }
    _inc_gift_timeout(5 + random2avg(9, 2));
    you.num_current_gifts[you.religion]++;
    you.num_total_gifts[you.religion]++;
    take_note(Note(NOTE_GOD_GIFT, you.religion));
    return true;
}

static bool _jiyva_mutate()
{
    simple_god_message(" alters your body.");

    bool deleted = false;
    // Go through each level of each existing non-temp, non-innate mutation.
    // Give a 1/4 chance of removing each, or a 1/2 chance for bad mutations.
    // Since we gift 4 mut levels (90% good), this means we stabilize at:
    //
    //      total mut levels = (t.m.l * (0.75 * 0.9 + 0.1 * 0.5)) + 4
    //
    // Which comes out to about 14.5 mut levels.
    for (int i = 0; i < NUM_MUTATIONS; ++i)
    {
        const mutation_type mut = (mutation_type)i;
        const int lvl = you.get_base_mutation_level(mut, false, false, true);
        if (!lvl)
            continue;
        const int chance = is_bad_mutation(mut) ? 50 : 25;
        const int deletions = binomial(lvl, chance);
        for (int del = 0; del < deletions; ++del)
        {
            deleted = delete_mutation(mut, "Jiyva's grace", true, false, true)
                      || deleted;
        }
    }

    // Try to gift 4 total levels of mutations. Focus on one mutation at a time
    // until capping its level, to maximize impact.
    int to_give = 4;
    for (int attempts = 0; to_give > 0 && attempts < 500; ++attempts)
    {
        const mutation_type cat
            = random_choose_weighted(6, RANDOM_GOOD_MUTATION,
                                     4, RANDOM_SLIME_MUTATION);
        const mutation_type mut = concretize_mut(cat);
        while (to_give > 0 && mutate(mut, "Jiyva's grace", false, false, true))
               --to_give;
    }
    return to_give == 0 || deleted;
}

// Is Vehumet offering this? With "only" only return true if this is the only
// reason the player can learn the spell now.
bool vehumet_is_offering(spell_type spell, bool only)
{
    return you.vehumet_gifts.count(spell)
           && !(only && you.spell_library[spell]);
}

void vehumet_accept_gift(spell_type spell)
{
    if (vehumet_is_offering(spell))
    {
        you.vehumet_gifts.erase(spell);
        you.duration[DUR_VEHUMET_GIFT] = 0;
    }
}

static void _add_to_old_gifts(spell_type spell)
{
    you.old_vehumet_gifts.insert(spell);
}

static bool _is_old_gift(spell_type spell)
{
    return you.old_vehumet_gifts.count(spell);
}

static set<spell_type> _vehumet_eligible_gift_spells(set<spell_type> excluded_spells)
{
    set<spell_type> eligible_spells;

    const int gifts = you.num_total_gifts[you.religion];
    if (gifts >= NUM_VEHUMET_GIFTS)
        return eligible_spells;

    const int min_lev[] = {1,2,2,3,3,4,4,5,5,5,5,6,8};
    const int max_lev[] = {1,3,3,4,5,7,7,7,7,7,7,8,9};
    COMPILE_CHECK(ARRAYSZ(min_lev) == NUM_VEHUMET_GIFTS);
    COMPILE_CHECK(ARRAYSZ(max_lev) == NUM_VEHUMET_GIFTS);
    int min_level = min_lev[gifts];
    int max_level = max_lev[gifts];

    if (min_level > you.experience_level)
        return eligible_spells;

    set<spell_type> backup_spells;
    for (int i = 0; i < NUM_SPELLS; ++i)
    {
        spell_type spell = static_cast<spell_type>(i);
        if (!is_valid_spell(spell))
            continue;

        if (excluded_spells.count(spell))
            continue;

        if (vehumet_supports_spell(spell)
            && !you.has_spell(spell)
            && !you.spell_library[spell]
            && is_player_book_spell(spell)
            && spell_difficulty(spell) <= max_level
            && spell_difficulty(spell) >= min_level)
        {
            if (!_is_old_gift(spell))
                eligible_spells.insert(spell);
            else
                backup_spells.insert(spell);
        }
    }
    // Don't get stuck just because all spells have been seen/offered.
    if (eligible_spells.empty())
    {
        if (backup_spells.empty())
        {
            // This is quite improbable to happen, but in this case just
            // skip the gift and increment the gift counter.
            if (gifts <= 12)
            {
                you.num_current_gifts[you.religion]++;
                you.num_total_gifts[you.religion]++;
            }
        }
        return backup_spells;
    }
    return eligible_spells;
}

static int _vehumet_weighting(spell_type spell)
{
    int bias = 100 + elemental_preference(spell, 10);
    return bias;
}

static spell_type _vehumet_find_spell_gift(set<spell_type> excluded_spells)
{
    set<spell_type> eligible_spells = _vehumet_eligible_gift_spells(excluded_spells);
    spell_type spell = SPELL_NO_SPELL;
    int total_weight = 0;
    int this_weight = 0;
    for (auto elig : eligible_spells)
    {
        this_weight = _vehumet_weighting(elig);
        total_weight += this_weight;
        if (x_chance_in_y(this_weight, total_weight))
            spell = elig;
    }
    return spell;
}

static set<spell_type> _vehumet_get_spell_gifts()
{
    set<spell_type> offers;
    unsigned int num_offers = you.num_total_gifts[you.religion] == 12 ? 3 : 1;
    while (offers.size() < num_offers)
    {
        spell_type offer = _vehumet_find_spell_gift(offers);
        if (offer == SPELL_NO_SPELL)
            break;
        offers.insert(offer);
    }
    return offers;
}

static bool _give_trog_oka_gift(bool forced)
{
    // Break early if giving a gift now means it would be lost.
    if (feat_eliminates_items(env.grid(you.pos())))
        return false;

    // No use for anything below. (No guarantees this will work right if these
    // mutations can ever appear separately.)
    if (you.has_mutation(MUT_NO_GRASPING))
        return false;

    const bool want_weapons = you_worship(GOD_TROG)
                              && (forced || you.piety >= piety_breakpoint(4)
                                            && random2(you.piety) > 120
                                            && one_chance_in(4));
    const bool want_missiles = you_worship(GOD_OKAWARU)
                               && (forced || you.piety >= piety_breakpoint(4)
                                             && random2(you.piety) > 120
                                             && x_chance_in_y(2,5));
    object_class_type gift_type;

    if (want_weapons)
        gift_type = OBJ_WEAPONS;
    else if (want_missiles)
        gift_type = OBJ_MISSILES;
    else
        return false;

    switch (gift_type)
    {
    case OBJ_MISSILES:
        simple_god_message(" grants you throwing weapons!");
        break;
    case OBJ_WEAPONS:
        simple_god_message(" grants you a weapon!");
        break;
    default:
        simple_god_message(" grants you bugs!");
        break;
    }

    const bool success =
        acquirement_create_item(gift_type, you.religion,
                false, you.pos()) != NON_ITEM;
    if (!success)
    {
        mpr("...but nothing appears.");
        return false;
    }
    switch (gift_type)
    {
    case OBJ_MISSILES:
        _inc_gift_timeout(26 + random2avg(19, 2));
        break;
    case OBJ_WEAPONS:
        _inc_gift_timeout(30 + random2avg(19, 2));
        break;
    default:
        break;
    }
    you.num_current_gifts[you.religion]++;
    you.num_total_gifts[you.religion]++;
    take_note(Note(NOTE_GOD_GIFT, you.religion));
    return true;
}

static bool _gift_jiyva_gift(bool forced)
{
    if (forced || you.piety >= piety_breakpoint(2)
                  && random2(you.piety) > 50
                  && one_chance_in(4) && !you.gift_timeout
                  && you.can_safely_mutate())
    {
        if (_jiyva_mutate())
        {
            _inc_gift_timeout(45 + random2avg(30, 2));
            you.num_current_gifts[you.religion]++;
            you.num_total_gifts[you.religion]++;
            return true;
        }
        else
            mpr("You feel as though nothing has changed.");
    }
    return false;
}

static bool _handle_uskayaw_ability_unlocks()
{
    bool success = false;
    // Uskayaw's triggered abilities trigger if you set the timer to -1.
    // We do this so that we trigger at the end of the round instead of
    // at the time we deal damage.
    if (you.piety == piety_breakpoint(2)
        && you.props[USKAYAW_AUDIENCE_TIMER].get_int() == 0)
    {
        you.props[USKAYAW_AUDIENCE_TIMER] = -1;
        success = true;
    }
    else if (you.piety == piety_breakpoint(3)
        && you.props[USKAYAW_BOND_TIMER].get_int() == 0)
    {
        you.props[USKAYAW_BOND_TIMER] = -1;
        success = true;
    }
    return success;
}

static bool _give_sif_gift(bool forced)
{
    // Smokeless fire and books don't get along.
    if (you.has_mutation(MUT_INNATE_CASTER))
        return false;

    // Break early if giving a gift now means it would be lost.
    if (feat_eliminates_items(env.grid(you.pos())))
        return false;

    if (!forced && (you.piety < piety_breakpoint(4)
                    || random2(you.piety) < 121 || one_chance_in(4)))
    {
        return false;
    }

    // Sif Muna special: Keep quiet if acquirement fails
    // because the player already has seen all spells.
    int item_index = acquirement_create_item(OBJ_BOOKS, you.religion,
                                             true, you.pos());
    if (item_index == NON_ITEM)
        return false;

    simple_god_message(" grants you a gift!");
    // included in default force_more_message

    you.num_current_gifts[you.religion]++;
    you.num_total_gifts[you.religion]++;
    const int n_spells = spells_in_book(env.item[item_index]).size();
    _inc_gift_timeout(10 + n_spells * 6 + random2avg(19, 2));
    take_note(Note(NOTE_GOD_GIFT, you.religion));

    return true;
}

static bool _sort_spell_level(spell_type spell1, spell_type spell2)
{
    if (spell_difficulty(spell1) != spell_difficulty(spell2))
        return spell_difficulty(spell1) < spell_difficulty(spell2);

    return strcasecmp(spell_title(spell1), spell_title(spell2)) < 0;
}

static bool _give_kiku_gift(bool forced)
{
    // Djinn can't receive spell gifts.
    if (you.has_mutation(MUT_INNATE_CASTER))
        return false;

    const bool first_gift = !you.num_total_gifts[you.religion];

    // Kikubaaqudgha gives two sets of spells in a quick succession.
    if (!forced && (you.piety < piety_breakpoint(0)
                    || !first_gift && you.piety < piety_breakpoint(2)
                    || you.num_total_gifts[you.religion] > 1))
    {
        return false;
    }

    vector<spell_type> spell_options;
    vector<spell_type> chosen_spells;

    // The first set guarantees the player at least one mid-level spell that
    // benefits from wretches.
    if (first_gift)
    {
        if (coinflip())
            chosen_spells.push_back(SPELL_ANIMATE_DEAD);
        else
            chosen_spells.push_back(SPELL_FUGUE_OF_THE_FALLEN);

        spell_options = {SPELL_SOUL_SPLINTER,
                         SPELL_KISS_OF_DEATH,
                         SPELL_SUBLIMATION_OF_BLOOD,
                         SPELL_GRAVE_CLAW,
                         SPELL_VAMPIRIC_DRAINING};
    }
    else
    {
        spell_options = {SPELL_ANGUISH,
                         SPELL_MARTYRS_KNELL,
                         SPELL_DISPEL_UNDEAD,
                         SPELL_PUTREFACTION,
                         SPELL_CURSE_OF_AGONY,
                         SPELL_BORGNJORS_VILE_CLUTCH,
                         SPELL_DEATH_CHANNEL,
                         SPELL_RIMEBLIGHT};
    }

    shuffle_array(spell_options);
    for (spell_type spell : spell_options)
    {
        if (spell_is_useless(spell, false))
            continue;
        chosen_spells.push_back(spell);
        if (chosen_spells.size() >= 4)
            break;
    }

    bool new_spell = false;
    for (auto spl : chosen_spells)
        if (!you.spell_library[spl])
            new_spell = true;

    if (!new_spell)
        simple_god_message(" has no new spells for you at this time.");
    else
        simple_god_message(" grants you a gift!");
    // included in default force_more_message

    sort(chosen_spells.begin(), chosen_spells.end(), _sort_spell_level);
    library_add_spells(chosen_spells);

    you.num_current_gifts[you.religion]++;
    you.num_total_gifts[you.religion]++;
    take_note(Note(NOTE_GOD_GIFT, you.religion));

    return true;
}

static bool _handle_veh_gift(bool forced)
{
    bool success = false;
    const int gifts = you.num_total_gifts[you.religion];
    if (forced || !you.duration[DUR_VEHUMET_GIFT]
                  && !you.has_mutation(MUT_INNATE_CASTER)
                  && (you.piety >= piety_breakpoint(0) && gifts == 0
                      || you.piety >= piety_breakpoint(0) + random2(6) + 18 * gifts && gifts <= 5
                      || you.piety >= piety_breakpoint(4) && gifts <= 11 && one_chance_in(20)
                      || you.piety >= piety_breakpoint(5) && gifts <= 12 && one_chance_in(20)))
    {
        set<spell_type> offers = _vehumet_get_spell_gifts();
        if (!offers.empty())
        {
            you.vehumet_gifts = offers;
            string prompt = " offers you knowledge of ";
            for (auto it = offers.begin(); it != offers.end(); ++it)
            {
                if (it != offers.begin())
                {
                    if (offers.size() > 2)
                        prompt += ",";
                    prompt += " ";
                    auto next = it;
                    next++;
                    if (next == offers.end())
                        prompt += "and ";
                }
                prompt += spell_title(*it);
                _add_to_old_gifts(*it);
                take_note(Note(NOTE_OFFERED_SPELL, *it));
            }
            prompt += ".";
            if (gifts >= NUM_VEHUMET_GIFTS - 1)
            {
                prompt += " These spells will remain available"
                          " as long as you worship Vehumet.";
            }

            you.duration[DUR_VEHUMET_GIFT] = (100 + random2avg(100, 2)) * BASELINE_DELAY;
            if (gifts >= 5)
                _inc_gift_timeout(30 + random2avg(30, 2));
            you.num_current_gifts[you.religion]++;
            you.num_total_gifts[you.religion]++;

            simple_god_message(prompt.c_str());
            // included in default force_more_message

            success = true;
        }
    }
    return success;
}

void mons_make_god_gift(monster& mon, god_type god)
{
    const god_type acting_god =
        (crawl_state.is_god_acting()) ? crawl_state.which_god_acting()
                                      : GOD_NO_GOD;

    if (god == GOD_NO_GOD && acting_god == GOD_NO_GOD)
        return;

    if (god == GOD_NO_GOD)
        god = acting_god;

    if (mon.flags & MF_GOD_GIFT)
    {
        dprf("Monster '%s' was already a gift of god '%s', now god '%s'.",
             mon.name(DESC_PLAIN, true).c_str(),
             god_name(mon.god).c_str(),
             god_name(god).c_str());
    }

    mon.god = god;
    mon.flags |= MF_GOD_GIFT;
}

bool mons_is_god_gift(const monster& mon, god_type god)
{
    return (mon.flags & MF_GOD_GIFT) && mon.god == god;
}

bool is_yred_undead_follower(const monster& mon)
{
    return mon.alive() && mon.holiness() & MH_UNDEAD
           && mon.attitude == ATT_FRIENDLY
           && mons_is_god_gift(mon, GOD_YREDELEMNUL);
}

bool is_apostle_follower(const monster& mon)
{
    return mon.alive() && mon.attitude == ATT_FRIENDLY
           && mon.type == MONS_ORC_APOSTLE;
}

bool is_fellow_slime(const monster& mon)
{
    return mon.alive() && mons_is_slime(mon)
           && mon.attitude == ATT_GOOD_NEUTRAL
           && mons_is_god_gift(mon, GOD_JIYVA);
}

static bool _is_plant_follower(const monster* mon)
{
    return mon->alive() && mons_is_plant(*mon)
           && mon->attitude == ATT_FRIENDLY;
}

bool is_follower(const monster& mon)
{
    if (you_worship(GOD_YREDELEMNUL))
        return is_yred_undead_follower(mon);
    else if (you_worship(GOD_BEOGH))
        return is_apostle_follower(mon);
    else if (you_worship(GOD_JIYVA))
        return is_fellow_slime(mon);
    else if (you_worship(GOD_FEDHAS))
        return _is_plant_follower(&mon);
    else
    {
        return mon.alive() && mon.attitude == ATT_FRIENDLY
                           && !mon.is_summoned();
    }
}

/**
 * What's the name of the ally Hepliaklqana granted the player?
 *
 * @return      The ally's name.
 */
string hepliaklqana_ally_name()
{
    return you.props[HEPLIAKLQANA_ALLY_NAME_KEY].get_string();
}

/**
 * How much HD should the ally granted by Hepliaklqana have?
 *
 * @return      The player's xl * 2/3.
 */
static int _hepliaklqana_ally_hd()
{
    if (!crawl_state.need_save) // on main menu or otherwise don't have 'you'
        return 27; // v0v
    // round up
    return (you.experience_level - 1) * 2 / 3 + 1;
}

/**
 * How much max HP should the ally granted by Hepliaklqana have?
 *
 * @return      5/hd from 1-11 HD, 10/hd from 12-18.
 *              (That is, 5 HP at 1 HD, 120 at 18.)
 */
int hepliaklqana_ally_hp()
{
    const int HD = _hepliaklqana_ally_hd();
    return HD * 5 + max(0, (HD - 12) * 5);
}

/**
 * Choose an antique name for a Hepliaklqana-granted ancestor.
 *
 * @param gender    The ancestor's gender.
 * @return          An appropriate name; e.g. Hrodulf, Citali, Aat.
 */
static string _make_ancestor_name(gender_type gender)
{
    const string gender_name = gender == GENDER_MALE ? " male " :
                               gender == GENDER_FEMALE ? " female " : " ";
    const string suffix = gender_name + "name";
    const string name = getRandNameString("ancestor", suffix);
    return name.empty() ? make_name() : name;
}

/// Setup when gaining a Hepliaklqana ancestor.
static void _setup_hepliaklqana_ancestor()
{
    // initial setup.
    if (!you.props.exists(HEPLIAKLQANA_ALLY_NAME_KEY))
    {
        const gender_type gender = random_choose(GENDER_NEUTRAL,
                                                 GENDER_MALE,
                                                 GENDER_FEMALE);

        you.props[HEPLIAKLQANA_ALLY_NAME_KEY] = _make_ancestor_name(gender);
        you.props[HEPLIAKLQANA_ALLY_GENDER_KEY] = gender;
    }
}

/**
 * Creates a mgen_data with the information needed to create the ancestor
 * granted by Hepliaklqana.
 *
 * XXX: should this be populating a mgen_data passed by reference, rather than
 * returning one on the stack?
 *
 * @return    The mgen_data that creates a hepliaklqana ancestor.
 */
mgen_data hepliaklqana_ancestor_gen_data()
{
    _setup_hepliaklqana_ancestor();
    const monster_type type = you.props.exists(HEPLIAKLQANA_ALLY_TYPE_KEY) ?
        (monster_type)you.props[HEPLIAKLQANA_ALLY_TYPE_KEY].get_int() :
        MONS_ANCESTOR;
    mgen_data mg(type, BEH_FRIENDLY, you.pos(), MHITYOU, MG_AUTOFOE,
                 GOD_HEPLIAKLQANA);
    mg.hd = _hepliaklqana_ally_hd();
    mg.hp = hepliaklqana_ally_hp();
    mg.extra_flags |= MF_NO_REWARD;
    mg.mname = hepliaklqana_ally_name();
    mg.props[MON_GENDER_KEY]
        = you.props[HEPLIAKLQANA_ALLY_GENDER_KEY].get_int();
    return mg;
}

/// Print a message for an ancestor's *something* being gained.
static void _regain_memory(const monster &ancestor, string memory)
{
    mprf("%s regains the memory of %s %s.",
         ancestor.name(DESC_YOUR, true).c_str(),
         ancestor.pronoun(PRONOUN_POSSESSIVE, true).c_str(),
         memory.c_str());
}

static string _item_ego_name(object_class_type base_type, int brand)
{
    switch (base_type)
    {
    case OBJ_WEAPONS:
    {
        // 'remembers... draining' reads better than 'drain', but 'flame'
        // reads better than 'flaming'
        const bool terse = brand == SPWPN_FLAMING
                           || brand == SPWPN_ANTIMAGIC;
        return brand_type_name((brand_type) brand, terse);
    }
    case OBJ_ARMOUR:
        // XXX: hack
        return "reflection";
    default:
        die("unsupported object type");
    }
}

/// Print a message for an ancestor's item being gained/type upgraded.
static void _regain_item_memory(const monster &ancestor,
                                object_class_type base_type,
                                int sub_type,
                                int brand)
{
    const string base_name = item_base_name(base_type, sub_type);
    if (!brand)
    {
        _regain_memory(ancestor, base_name);
        return;
    }

    const string ego_name = _item_ego_name(base_type, brand);
    const string item_name
        = make_stringf("%s of %s",
                       item_base_name(base_type, sub_type).c_str(),
                       ego_name.c_str());
    _regain_memory(ancestor, item_name);
}

/**
 * Update the ancestor's stats after the player levels up. Upgrade HD and HP,
 * and give appropriate messaging for that and any other notable upgrades
 * (spells, resists, etc).
 *
 * @param quiet_force     Whether to squash messages & force upgrades,
 *                        even if the HD is unchanged.
 */
void upgrade_hepliaklqana_ancestor(bool quiet_force)
{
    monster* ancestor = hepliaklqana_ancestor_mon();
    if (!ancestor || !ancestor->alive())
        return;

    // housekeeping
    ancestor->mname = hepliaklqana_ally_name();
    ancestor->props[MON_GENDER_KEY]
        = you.props[HEPLIAKLQANA_ALLY_GENDER_KEY].get_int();

    const int old_hd = ancestor->get_experience_level();
    const int hd = _hepliaklqana_ally_hd();
    ancestor->set_hit_dice(hd);
    if (old_hd == hd && !quiet_force)
        return; // assume nothing changes except at different HD

    const int old_mhp = ancestor->max_hit_points;
    ancestor->max_hit_points = hepliaklqana_ally_hp();
    ancestor->props[KNOWN_MAX_HP_KEY] = ancestor->max_hit_points;
    ancestor->hit_points =
        div_rand_round(ancestor->hit_points * ancestor->max_hit_points,
                       old_mhp);

    if (!quiet_force)
    {
        mprf("%s remembers more of %s old skill.",
             ancestor->name(DESC_YOUR, true).c_str(),
             ancestor->pronoun(PRONOUN_POSSESSIVE, true).c_str());
    }

    set_ancestor_spells(*ancestor, !quiet_force);

    const bool ancestor_offlevel = companion_is_elsewhere(ancestor->mid);
    if (ancestor_offlevel)
        add_daction(DACT_UPGRADE_ANCESTOR);

    // assumption: ancestors can lose weapons (very rarely - tukima's),
    // and it's weird for them to just reappear, so only upgrade existing ones
    if (ancestor->weapon())
    {
        if (!ancestor_offlevel)
            upgrade_hepliaklqana_weapon(ancestor->type, *ancestor->weapon());

        const weapon_type wpn = _hepliaklqana_weapon_type(ancestor->type, hd);
        const brand_type brand = _hepliaklqana_weapon_brand(ancestor->type, hd);
        if (wpn != _hepliaklqana_weapon_type(ancestor->type, old_hd)
            && !quiet_force)
        {
            _regain_item_memory(*ancestor, OBJ_WEAPONS, wpn, brand);
        }
        else if (brand != _hepliaklqana_weapon_brand(ancestor->type, old_hd)
                 && !quiet_force)
        {
            mprf("%s remembers %s %s %s.",
                 ancestor->name(DESC_YOUR, true).c_str(),
                 ancestor->pronoun(PRONOUN_POSSESSIVE, true).c_str(),
                 apostrophise(item_base_name(OBJ_WEAPONS, wpn)).c_str(),
                 brand_type_name(brand, brand != SPWPN_DRAINING));
        }
    }
    // but shields can't be lost, and *can* be gained (knight at hd 5)
    // so give them out as appropriate
    if (!ancestor_offlevel)
    {
        if (ancestor->shield())
            upgrade_hepliaklqana_shield(*ancestor, *ancestor->shield());
        else
            give_shield(ancestor);
    }

    const armour_type shld = _hepliaklqana_shield_type(ancestor->type, hd);
    if (shld != _hepliaklqana_shield_type(ancestor->type, old_hd)
        && !quiet_force)
    {
        // doesn't currently support egos varying separately from shield types
        _regain_item_memory(*ancestor, OBJ_ARMOUR, shld,
                            _hepliaklqana_shield_ego(hd));
    }

    if (quiet_force)
        return;
}

/**
 * What type of weapon should an ancestor of the given HD have?
 *
 * @param mc   The type of ancestor in question.
 * @param HD   The HD of the ancestor in question.
 * @return     An appropriate weapon_type.
 */
static weapon_type _hepliaklqana_weapon_type(monster_type mc, int HD)
{
    switch (mc)
    {
    case MONS_ANCESTOR_HEXER:
        return HD < 16 ? WPN_DAGGER : WPN_QUICK_BLADE;
    case MONS_ANCESTOR_KNIGHT:
        return HD < 10 ? WPN_FLAIL : WPN_BROAD_AXE;
    case MONS_ANCESTOR_BATTLEMAGE:
        return HD < 13 ? WPN_QUARTERSTAFF : WPN_LAJATANG;
    default:
        return NUM_WEAPONS; // should never happen
    }
}

/**
 * What brand should an ancestor of the given HD's weapon have, if any?
 *
 * @param mc   The type of ancestor in question.
 * @param HD   The HD of the ancestor in question.
 * @return     An appropriate weapon_type.
 */
static brand_type _hepliaklqana_weapon_brand(monster_type mc, int HD)
{
    switch (mc)
    {
        case MONS_ANCESTOR_HEXER:
            return HD < 16 ?   SPWPN_DRAINING :
                               SPWPN_ANTIMAGIC;
        case MONS_ANCESTOR_KNIGHT:
            return HD < 10 ?   SPWPN_NORMAL :
                   HD < 16 ?   SPWPN_FLAMING :
                               SPWPN_SPEED;
        case MONS_ANCESTOR_BATTLEMAGE:
            return HD < 13 ?   SPWPN_NORMAL :
                               SPWPN_FREEZING;
        default:
            return SPWPN_NORMAL;
    }
}

/**
 * Setup an ancestor's weapon after their class is chosen, when the player
 * levels up, or after they're resummoned (or initially created for wrath).
 *
 * @param[in]   mtyp          The ancestor for whom the weapon is intended.
 * @param[out]  item          The item to be configured.
 * @param       notify        Whether messages should be printed when something
 *                            changes. (Weapon type or brand.)
 */
void upgrade_hepliaklqana_weapon(monster_type mtyp, item_def &item)
{
    ASSERT(mons_is_hepliaklqana_ancestor(mtyp));
    if (mtyp == MONS_ANCESTOR)
        return; // bare-handed!

    item.base_type = OBJ_WEAPONS;
    item.sub_type = _hepliaklqana_weapon_type(mtyp,
                                              _hepliaklqana_ally_hd());
    item.brand = _hepliaklqana_weapon_brand(mtyp,
                                            _hepliaklqana_ally_hd());
    item.plus = 0;
    item.flags |= ISFLAG_IDENTIFIED | ISFLAG_SUMMONED;
}

/**
 * What kind of shield should an ancestor of the given HD be given?
 *
 * @param mc        The type of ancestor in question.
 * @param HD        The HD (XL) of the ancestor in question.
 * @return          An appropriate type of shield, or NUM_ARMOURS.
 */
static armour_type _hepliaklqana_shield_type(monster_type mc, int HD)
{
    if (mc != MONS_ANCESTOR_KNIGHT)
        return NUM_ARMOURS;
    if (HD < 13)
        return ARM_KITE_SHIELD;
    return ARM_TOWER_SHIELD;
}

static special_armour_type _hepliaklqana_shield_ego(int HD)
{
    return HD < 13 ? SPARM_NORMAL : SPARM_REFLECTION;
}

/**
 * Setup an ancestor's weapon after their class is chosen, when the player
 * levels up, or after they're resummoned (or initially created for wrath).
 *
 * @param[in]   ancestor      The ancestor for whom the weapon is intended.
 * @param[out]  item          The item to be configured.
 * @return                    True iff the ancestor should have a weapon.
 */
void upgrade_hepliaklqana_shield(const monster &ancestor, item_def &item)
{
    ASSERT(mons_is_hepliaklqana_ancestor(ancestor.type));
    const int HD = ancestor.get_experience_level();
    const armour_type shield_type = _hepliaklqana_shield_type(ancestor.type,
                                                              HD);
    if (shield_type == NUM_ARMOURS)
        return; // no shield yet!

    item.base_type = OBJ_ARMOUR;
    item.sub_type = shield_type;
    item.brand = _hepliaklqana_shield_ego(HD);
    item.plus = 0;
    item.flags |= ISFLAG_IDENTIFIED | ISFLAG_SUMMONED;
    item.quantity = 1;
}

///////////////////////////////
bool do_god_gift(bool forced)
{
    ASSERT(!you_worship(GOD_NO_GOD));

    god_acting gdact;

#if defined(DEBUG_DIAGNOSTICS) || defined(DEBUG_GIFTS)
    int old_num_current_gifts = you.num_current_gifts[you.religion];
    int old_num_total_gifts = you.num_total_gifts[you.religion];
#endif

    bool success = false;

    // Consider a gift if we don't have a timeout and aren't under penance
    if (forced || !player_under_penance() && !you.gift_timeout)
    {
        // Remember to check for water/lava.
        switch (you.religion)
        {
        default:
            break;

        case GOD_NEMELEX_XOBEH:
            success = _give_nemelex_gift(forced);
            break;

        case GOD_OKAWARU:
        case GOD_TROG:
            success = _give_trog_oka_gift(forced);
            break;

        case GOD_JIYVA:
            success = _gift_jiyva_gift(forced);
            break;

        case GOD_USKAYAW:
            success = _handle_uskayaw_ability_unlocks();
            break;

        case GOD_KIKUBAAQUDGHA:
            success = _give_kiku_gift(forced);
            break;

        case GOD_SIF_MUNA:
            success = _give_sif_gift(forced);
            break;

        case GOD_VEHUMET:
            success = _handle_veh_gift(forced);
            break;
        }                       // switch (you.religion)
    }                           // End of gift giving.

    if (success)
        stop_running(false);

#if defined(DEBUG_DIAGNOSTICS) || defined(DEBUG_GIFTS)
    if (old_num_current_gifts < you.num_current_gifts[you.religion])
    {
        mprf(MSGCH_DIAGNOSTICS, "Current number of gifts from this god: %d",
             you.num_current_gifts[you.religion]);
    }
    if (old_num_total_gifts < you.num_total_gifts[you.religion])
    {
        mprf(MSGCH_DIAGNOSTICS, "Total number of gifts from this god: %d",
             you.num_total_gifts[you.religion]);
    }
#endif
    return success;
}

string god_name(god_type which_god, bool long_name)
{
    // First handle gods with special naming logic
    if (which_god == GOD_JIYVA)
    {
        return god_name_jiyva(long_name) +
               (long_name? " the Shapeless" : "");
    }

    if (which_god == GOD_ANCIENT_ONE)
    {
        // Long name is not different for Alpha/Beta in this MVP
        // Use the globally defined function from ancient-one.h
        switch (get_ancient_one_state())
        {
            case AncientOneState::ALPHA:
                return "Alpha";
            case AncientOneState::BETA:
                return "Beta";
        }
        // Should not be reachable
        return "Ancient One Error";
    }

    // Use database lookups for long names of standard gods
    if (long_name)
    {
        const string shortname = god_name(which_god, false);
        const string longname = getMiscString(shortname + " lastname");
        return longname.empty()? shortname : longname;
    }

    // Short names for standard gods
    switch (which_god)
    {
    case GOD_NO_GOD:        return "No God";
    case GOD_RANDOM:        return "random";
    // ... (rest of cases as they were) ...
    case GOD_IGNIS:         return "Ignis";
    // GOD_ANCIENT_ONE handled above
    // GOD_JIYVA handled above
    case GOD_ECUMENICAL:    return "an unknown god";
    case NUM_GODS:          return "Buggy";
    }
    return "Buggy god"; // Should be unreachable
}
