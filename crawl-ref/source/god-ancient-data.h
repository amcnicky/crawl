#pragma once

// Adding names is easy - just add to the data array
const char* ag_name_data[] =
{
    // Random
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
    "Fargozz",
    "Garoth",
    "Ellemainiel",
    "Niarjeb",
    "Niarlep",
    "Io",
    "Sacrazzaet",
    "Nyaralaphet",

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
    "Rrragathor",
    "Temaris"
};

// See above comment on adding names - additions welcome.
// I tried a version with the scroll name generator but hand crafted
// felt better. Generally keeping away from elemental/ability based
// titles since they may not match themeatically what ends up being
// generated. A loose theme of crumbling/ancient/forgotten.
const char* ag_title_data[] =
{
    ", the Forgotten",
    ", the Dreamer",
    ", the Darksky Raven",
    ", the Ancient",
    ", the Old",
    ", the Tired",
    ", the Venerable",
    ", the Grizzled",
    ", the Wasted",
    ", the Soulshark",
    ", the Soulbound",
    ", the Entombed",
    ", the Feaster",
    ", the Great Maw",
    ", Bane of the Living",
    ", Bane of the Dead",
    ", the Great Firestarter",
    ", Demonsbane",
    ", the Lord of the Forgotten",
    ", the Queen of the Forgotten",
    ", the Forgotten Prince",
    ", the Forgotten Princess",
    ", the Deathsinger",
    ", the Five Eyes",
    ", the Eye from Before",
    ", the Great Shadow",
    ", the Horror from Beyond",
    ", the Unearthly Horror",
    ", the Horror in the Night",
    ", the Sunshine Terror",
    ", the Primordial Emptiness",
    ", the Nightmare Lord",
    ", the Nightmare Queen",
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
    "time",
    "theft",
    "courage",
    "hope",
    "despair",
    "reasoning",
    "knowledge",
    "passion",
};

const char* ag_desc_faction_data[] =
{
    "assassins",
    "necromancers",
    "thieves",
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
    "hobgoblins",
    "felids",
    "angry felids",
    "travellers",
    "adventurers",
    "knights",
    "tentacled things",
};

// End with a space
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
    "A relic from a bygone era, ",
    "A deity from aeons past, ",
    "Lord of all FACTION, ",
    "Queen of all FACTION, ",
    "An ancient mystery, ",
};

// End with a space
const char* ag_desc_how_god_was_data[] =
{
    "GODNAME was once the foundation of society. ",
    "GODNAME was once the centre of culture. ",
    "GODNAME's power once burned brightly. ",
    "GODNAME's influence once stretched throughout the land. ",
    "GODNAME was once even worshipped by the Great Pharaoh. ",
    "GODNAME's power was once known to drive worshippers to insanity. ",
    "GODNAME once ruled from within the Holy Volcano of Yen. ",
    "GODNAME's power once lead its people to great feats. ",
    "there was once no greater honour that to be a high cultist of GODNAME. ",
    "GODNAME was once filled with hate and power. ",
    "GODNAME's followers once held great power and influence. ",
};

// End with a space
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
    "However, as the millenial planetary alignments occurred, a whole new "
        "pantheon of gods emerged, who swiftly eroded GODNAME's power "
        "into obscurity. ",
    "However, a great, but tragic, event, not recorded by modern scholars, "
        "occurred, all but destroying GODNAME's influence. ",
    "However, as FACTION moved away from their religious roots, so GODNAME's "
        "power waned. ",
};

// Final part - do not end with a space
const char* ag_desc_god_now_data[] =
{
    "Now, GODNAME seeks revenge.",
    "Now, GODNAME seeks the Orb of Zot to reclaim its power.",
    "Now, GODNAME lies in wait, gathering power once again.",
    "Now, GODNAME draws power from the dreams of FACTION, awaiting "
        "a day of power once again.",
    "Now, GODNAME seeks followers to build its power.",
    "Now, GODNAME calls out to you. \"Will you be my champion? Together "
        "we can reclaim my godly throne.\"",
    "Now, GODNAME dreams only of ferrocious revenge.",
    "Now, GODNAME whispers in the margins of old books, turning over-curious "
        "scholars and their students to its aid.",
}; 