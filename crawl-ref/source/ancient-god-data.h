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
        "GOD NOW bends the minds of shopkeepers to offer \
        a highly favourable price.\n",
        " passively bends the minds of shopkeepers to offer a \
        highly favourable price.\n"
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
};

agod_ability_small ag_ability_small_data[] =
{
    {   //AG_S_HEROISM
        ABIL_OKAWARU_HEROISM,
        "testdesc1 heroism",
        "testdesc2 heroism"
    },
};

agod_ability_cap ag_ability_cap_data[] =
{
    {   //AG_C_DISASTER_AREA
        ABIL_QAZLAL_DISASTER_AREA,
        "testdesc1 DA",
        "testdesc2 DA"
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