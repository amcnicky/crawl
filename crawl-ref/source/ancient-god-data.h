#pragma once

#include "enum.h"
#include "tag-version.h"
#include "stringutil.h"

struct agod_passive
{
    passive_t passive;
    const char* short_description;
    string long_description;
};

struct agod_ability_small
{
    ability_type ability;
    const char* short_description;
    const char* long_description;
};

struct agod_ability_cap
{
    ability_type ability;
    const char* short_description;
    const char* long_description;
};

struct agod_mut
{
    mutation_type mutation;
};

agod_passive ag_passive_data[] = 
{
    {   //AG_P_RESCUE_MAMMAL
        passive_t::rescue_mammal,
        "GOD NOW sends mammals to aid you in dangerous situations.\n",
        " sends mammals to your aid in dangeroud situations.\n"
    },
    {   //AG_P_CONSISTENT_SHOPPING
        passive_t::discount_shopping,
        "GOD NOW bends the minds of shopkeepers to offer "
        "a highly favourable price.\n",
        " passively bends the minds of shopkeepers to offer a "
        "highly favourable price.\n"
    },
    {   //AG_P_POTION_REFILLS
        passive_t::potion_refills,
        "GOD NOW occasionally refills potions as you drink them.\n",
        " magically refills your consumed potions.\n"
    },
    {   //AG_P_SPRIGGAN_SIDEKICK
        passive_t::spriggan_sidekick,
        "GOD NOW grants a reviving spriggan sidekick.\n",
        " grants a reviving spriggan sidekick.\n"
    },
    {
        passive_t::threatening_boost,
        "buggy when read from here - dynamic treatment in ancient-god.cc",
        "buggy when read from here - dynamic treatment in ancient-god.cc"
    },
};

agod_ability_small ag_ability_small_data[] =
{
    {   //AG_S_HEROISM
        ABIL_AG_REALITY_DILATION,
        "You can warp reality to temporarily move incredibly quickly.",
        "GODNAME long description for reality dilation."
    },
    {   //AG_S_SPATIAL_SINGULARITY
        ABIL_AG_SPATIAL_SINGULARITY,
        "You can return to a previous location through a spatial singularity.",
        "GODNAME long description for spatial singularity."
    },
    {   //AG_S_DIVINE_CONDUIT
        ABIL_AG_DIVINE_CONDUIT,
        "outdated useage, to tidy up this section and delete.",
        "outdated useage, to tidy up this section and delete."
    },
};

agod_ability_cap ag_ability_cap_data[] =
{
    {   //AG_C_REALITY_DILATION
        ABIL_BEOGH_GIFT_ITEM,
        "GODNAME short description for capstone.",
        "GODNAME long description for capstone."
    },
};

agod_mut ag_mut_data[] =
{
    {   //AG_M_HP_CASTING
        MUT_HP_CASTING
    },
};

// Adding names is easy - just increment NUM_AG_NAMES and add to the data array
const char* ag_name_data[] =
{
    "Yib",
    "Ungoth",
    "Morwen",
    "Morloth",
    "Xen",
    "Pivrasso",
    "Arlent",
    "Phraeglurk",
    "Benthep",
    "Mliarancel",
    "Xzyx",

    // Egyptian Inspiration
    "Sehetepibre",
    "Kheperkare",
    "Nubkaure",
    "Khakheperre",
    "Khakaure",
    "Nimaatre",
    "Sobekkare",
    "Gengen Wer",
    "Ba'al",

    // Greek Inspiration
    "Aeolus",
    "Alastor",
    "Asclepius",
    "Boreas",
    "Crios",
    "Dinlas",
    "Eurus",
    "Hymenaios",
    "Morpheus",
    "Notus",
    "Zeleus",

    // African Inspiration
    "Ammon",
    "Anzar",
    "Belus",
    "Gurzil",
    "Idir",
    "Avrikiti",
    "Lisa",
    "Sakpata",
    "Zinsi",

    // Shout-out to anyone who proposed a god through the old wiki
    // Can't promise I found 100% of them
    "Ulziri",
    "Goddess of Darkness",
    "Acirotes",
    "Jerihod",
    "Iveu",
    "Tsu Muna",
    "Shin",
    "Caries",
    "Patchouli",
    "Ego",
    "Zoprei",
    "Habanaethaegor",
    "Aristar",
    "Solitas",
    "Kogasin",
    "Narcissus",
    "'Crazy' Edalo",
    "Ghib",
    "Balor",
    "Halberhet",
    "Uryon",
    "Doorokhloe",
    "Dorlok",
    "Riefe",
    "Euquanauque",
    "It That Was Bound",
    "Nifargle",
    "Aaroden",
    "Iashol",
    /* long live */ "Pakellas",
    "Phireis",
    "Wynnsym",
    "Rhundival",
    "Ansci",
    "Sethygir",
    "Jorghriss",
    "Erebos",
    "Udzu",
    "Uru Lai Kah",
    "Uukiru",
    "Rrragathor"
};

// See above comment on adding names - additions welcome.
// I tried a version with the scroll name generator but hand crafted
// felt better. Generally keeping away from elemental/ability based
// titles since they may not match themeatically what ends up being
// generated. A loose theme of crumbling/ancient/forgotten.
const char* ag_title_data[] =
{
    " the Forgotten",
    " the Dreamer",
    " the Darksky Raven",
    " the Ancient",
    " the Old",
    " the Tired",
    " the Venerable",
    " the Grizzled",
    " the Wasted",
    " the Soulshark",
    " the Soulbound",
    " the Entombed",
    " the Feaster",
    " the Great Maw",
    " Bane of the Living",
    " Bane of the Dead",
    " Demonsbane",
    " the Lord of the Forgotten",
    " the Queen of the Forgotten",
    " the Forgotten Prince",
    " the Forgotten Princess",
    " the Deathsinger",
    " the Five Eyes",
    " the Eye from Before",
    " the Great Shadow",
    " the Horror from Beyond",
    " the Unearthly Horror",
    " the Horror in the Night",
    " the Sunshine Terror",
    " the Primordial Emptiness"
};

const char* ag_spriggan_names[] =
{
    "Jared",
    "Audie",
    "Cesar",
    "Martin",
    "Paul",
    "Benjamin",
    "Neil",
    "Frederick",
    "Thomas",
    "Babe",
    "Ainsley",
    "Alden",
    "Alfred",
    "Alvin",
    "Ashley",
    "Audrey",
    "Bailee",
    "Beckett",
    "Beowulf",
    "Brinley",
    "Charlton",
    "Chester",
    "Cliff",
    "Dana",
    "Darwin",
    "Daxter",
    "Edgar",
    "Edmond",
    "Edvard",
    "Elfie",
    "Emma",
    "Francesca",
    "Kathryn",
    "Lane",
    "Lawley",
    "Lillian",
    "Paxton",
    "Rowena"
};

const char* ag_desc_aspect_data[] =
{
    "life",
    "laughter",
    "love",
    "death",
    "famine",
    "decay",
    "memory",
    "dreams",
    "pain",
    "suffering",
    "light",
    "darkness",
    "the beyond",
    "strange happenings",
    "otherworldly experiences",
    "eperience",
    "youth",
    "beginnings",
    "fatigue",
    "energy",
    "the abyss",
    "the void",
};

const char* ag_desc_faction_data[] =
{
    "assassins",
    "necromancers",
    "theives",
    "merchants",
    "sailors",
    "navigators",
    "pyromancers",
    "geomancers",
    "vampires",
    "were-yaks",
    "the forgotten people of Shyixth",
    "trolls",
    "centaurs",
    "humans",
    "hobgoblins",
    "denizens of Zot",
    "denizens of the dungeon",
};

const char* ag_desc_introduction_data[] =
{
    "A long time ago, in a realm somewhat distant to this one, ",
    "A long-forgotten deity of ASPECT, ",
    "A long-forgotten idol of ASPECT, ",
    "A patron deity of FACTION, ",
    "Remembered now only by the ASPECT FACTION, ",
    "Now only seen in the dark temples of the FACTION of ASPECT, ",
    "Now only referenced in exceedingly rare tomes, ",
    "Now only whispered about in the high towers of Valen, ",
    "Now barely a rumour, ",
    "Now only whispered about in dark rooms as the candles burn low, ",
};

const char* ag_desc_how_god_was_data[] =
{
    "GODNAME was once the foundation of society. ",
    "GODNAME was once the centre of culture. ",
    "GODNAME's power once burned brightly. ",
    "GODNAME's influence once stretched throughout the land. ",
    "GODNAME's was once even worshipped by the Great Pharaoh. ",
    "GODNAME's power was once known to drive worshippers to insanity. ",
    "GODNAME once ruled from within the Holy Volcano of Yen. ",
    "GODNAME's power once lead its people to great feats. ",
    "there was once no greater honour that to be a high cultist of GODNAME. ",
};

const char* ag_desc_god_fall_data[] =
{
    "However, followers of Ignis all but destroyed the memory of GODNAME. ",
    "However, followers of Cheibriados thought it was all a bit too fast-paced"
        " and almost destroyed GODNAME's existence. ",
    "However, GODNAME's power dwindled as the planets changed their alignment"
        " and almost all worshippers moved on. ",
    "However, since the slaughter at Saar'ylen, GODNAME's following has"
        " dwindled. ",
    "However, as thousands of years passed, GODNAME drifted into obscurity. ",
};

const char* ag_desc_god_now_data[] =
{
    "Now, GODNAME seeks revenge.",
    "Now, GODNAME seeks the Orb of Zot to reclaim its power.",
    "Now, GODNAME lies in wait, gathering power once again.",
    "Now, GODNAME draws power from the dreams of FACTION, dreaming itself "
        "of one day being powerful again.",
    "Now, GODNAME seeks followers to build its power.",
};
