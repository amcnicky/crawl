#pragma once

#include "enum.h"
#include "tag-version.h"

agod_passive ag_passive_data[] = 
{
    {   //AG_P_SHROUD
        passive_t::umbra,
        "create an umbra around yourself\n",
        "You have a dark and eerie umbra\n"
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
const int NUM_AG_NAMES = 81;
string ag_name_data[] =
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
    "It That was Bound",
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
// felt better.
const int NUM_AG_TITLES = 3;
string ag_title_data[] =
{
    " the Coolest",
    " Bringer of Chill",
    " the Great Tester"
};