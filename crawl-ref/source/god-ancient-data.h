#pragma once

// Adding names is easy - just add to the data array
const char* ag_name_data[] =
{
    // Original Names (Curated)
    "Yib", "Ungoth", "Morwen", "Morloth", "Xen", "Pivrasso", "Arlent",
    "Phraeglurk", "Benthep", "Mliarancel", "Xzyx", "Fargozz", "Garoth",
    "Ellemainiel", "Niarjeb", "Niarlep", "Io", "Sacrazzaet", "Nyaralaphet",
    "Sehetepibre", "Kheperkare", "Nubkaure", "Khakheperre", "Khakaure",

    // Mesopotamian/Sumerian-esque
    "Anu", "Enki", "Inanna", "Utu", "Nanna", "Kur", "Anzu", "Tiamat", "Eresh",
    "Zius", "Lugal", "Ninsun", "Gula", "Pazuzu",

    // Proto-Indo-European-esque
    "Dyēus", "Perkwunos", "Welnos", "Suhul", "Hausos", "Menot", "Devos",
    "Yemós", "Argros",

    // Fictional / Fantasy
    "Malakor", "Zephyros", "Valerius", "Xylos", "Q'tharr", "Zarthus",
    "Vorlag",

    // Community Suggestions (from old wiki, hope I got them all)
    "Zoprei", "Habanaethaegor", "Aristar", "Solitas", "Kogasin", "Edalo",
    "Ulziri", "Acirotes", "Jerihod", "Iveu", "Tsu Muna", "Shin", "Pakellas",
    "Ghib", "Balor", "Halberhet", "Uryon", "Doorokhloe", "Dorlok", "Riefe",
    "Euquanauque", "It That Was Bound", "Nifargle", "Aaroden", "Iashol",
    "Phireis", "Wynnsym", "Rhundival", "Ansci", "Sethygir", "Jorghriss",
    "Erebos", "Udzu", "Uru Lai Kah", "Uukiru", "Rrragathor", "Temaris",

    // New Fictional Names
    "Gorthaur", "Cthaeh", "Morgoth", "Aule", "Yavanna", "Orome", "Mandos",
    "Ulmo", "Varda", "Voronwe", "Ossë", "Irmo", "Finwe", "Melian", "Luthien",
    "Valar", "Maiar", "Zardak", "Grolnok", "Vex'thul", "Kael'thas",
    "Illidan", "Sargeras", "Malygos", "Nozdormu", "Ysera", "Alextrasza"
};

// See above comment on adding names - additions welcome.
// I tried a version with the scroll name generator but hand crafted
// felt better. Generally keeping away from elemental/ability based
// titles since they may not match themeatically what ends up being
// generated. A loose theme of crumbling/ancient/forgotten.
const char* ag_title_data[] =
{
    ", the Forgotten", ", the Dreamer", ", the Darksky Raven",
    ", the Ancient", ", the Old", ", the Tired", ", the Venerable",
    ", the Grizzled", ", the Wasted", ", the Soulshark", ", the Soulbound",
    ", the Entombed", ", the Feaster", ", the Great Maw",
    ", Bane of the Living", ", Bane of the Dead", ", the Great Firestarter",
    ", Demonsbane", ", the Lord of the Forgotten", ", the Queen of the Forgotten",
    ", the Forgotten Prince", ", the Forgotten Princess", ", the Deathsinger",
    ", the Five Eyes", ", the Eye from Before", ", the Great Shadow",
    ", the Horror from Beyond", ", the Unearthly Horror",
    ", the Horror in the Night", ", the Sunshine Terror",
    ", the Primordial Emptiness", ", the Nightmare Lord", ", the Nightmare Queen",
    ", the Silent Whisper", ", Echo of the First Age", ", Who Waits",
    ", of the Sundered Throne", ", the Last Echo", ", whose Name is Dust",
    ", the All-Consuming", ", He of the Thousand Chains", ", She of the Glass Labyrinth"
};

const char* ag_desc_aspect_data[] =
{
    "life", "laughter", "love", "death", "famine", "decay", "memory", "dreams",
    "pain", "suffering", "light", "darkness", "the beyond", "strange happenings",
    "otherworldly experiences", "experience", "youth", "beginnings", "fatigue",
    "energy", "the abyss", "the void", "time", "theft", "courage", "hope",
    "despair", "reasoning", "knowledge", "passion", "rust", "silence",
    "echoes", "secrets", "madness", "gravity", "the hunt", "chance", "entropy",
    "thirst", "mirrors", "lies", "oaths", "storms", "winter", "slumber"
};

const char* ag_desc_faction_data[] =
{
    "assassins", "necromancers", "thieves", "merchants", "sailors",
    "navigators", "pyromancers", "geomancers", "vampires", "were-yaks",
    "forgotten people of Shyixth", "trolls", "centaurs", "humans",
    "hobgoblins", "denizens of Zot", "denizens of the dungeon", "felids",
    "angry felids", "travellers", "adventurers", "knights",
    "tentacled things", "sun-scorched nomads", "chittering swarms",
    "cartographers of madness", "obsidian librarians", "singers of the deep",
    "iron horticulturists", "keepers of the Five-Fold Gate",
    "sky-glass smiths", "rust-cult evangelists", "whispering guild",
    "forgotten kings", "blind sentinels"
};

// End with a space
const char* ag_desc_introduction_data[] =
{
    "A long time ago, in a realm somewhat distant to this one, ",
    "A long-forgotten deity of ASPECT, ",
    "A long-forgotten idol of ASPECT, ",
    "A patron deity of the FACTION, ",
    "Remembered now only by the FACTION of ASPECT, ",
    "Now only seen in the dark temples of the FACTION, ",
    "Now only referenced in exceedingly rare tomes, ",
    "Now only whispered about in the high towers of Valen, ",
    "Now barely a rumour, ",
    "Now only whispered about in dark rooms as the candles burn low, ",
    "A relic from a bygone era, ",
    "A deity from aeons past, ",
    "An ancient mystery, ",
    "Once the patron of all FACTION, ",
    "Before the first sun rose, there was GODNAME. ",
    "In an age of myth, when the world was still young, ",
    "An entry in the fragmented Obsidian Codex speaks of GODNAME, a power of ASPECT. ",
    "When the mountains were but dust, and the seas were not yet salt, GODNAME walked the world. ",
    "They say if you listen to the wind in the old temples, you can still hear the name GODNAME. ",
    "The iron horticulturists of the north once swore fealty only to GODNAME. ",
    "In the time before time, there was only the void and the whisper of GODNAME. ",
    "The first FACTION looked to the sky and saw not stars, but the eyes of GODNAME. ",
    "Buried deep in the archives of the obsidian librarians lies the story of GODNAME. ",
    "There was a name for the feeling of ASPECT, and that name was GODNAME. ",
    "The cartographers of madness charted the paths of many strange beings, but only one, GODNAME, was their master. ",
    "Long before the new gods came, their power was but a shadow of GODNAME's. ",
    "In an age of wonders, the greatest wonder was GODNAME, patron of ASPECT. ",
    "The first shadow was cast by GODNAME, and in it the first secrets were hidden. ",
    "The sun-scorched nomads tell a story of a power that shaped the dunes, a power they called GODNAME. ",
    "There is a truth hidden in the heart of the dungeon: that this place was once the throne of GODNAME. ",
    "The world has forgotten many things, but the deepest, oldest stones remember GODNAME. ",
    "Every forgotten corner of the world holds an echo of a forgotten power, an echo of GODNAME. ",
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
    "there was once no greater honour than to be a high cultist of GODNAME. ",
    "GODNAME was once filled with hate and power. ",
    "GODNAME's followers once held great power and influence. ",
    "All FACTION paid tribute to GODNAME. ",
    "The power of ASPECT flowed from GODNAME alone. ",
    "From a throne of obsidian, GODNAME watched the world turn. ",
    "At GODNAME's whim, the seasons would turn, and the rivers would change their course. ",
    "Kings and queens sought the counsel of GODNAME, their empires rising and falling on a single word. ",
    "It was GODNAME who carved the Whispering Canyons and filled the Sky-Glass Lake. ",
    "The songs of the FACTION were hymns to GODNAME, their art a reflection of its glory. ",
    "To speak the name of GODNAME was to invoke ASPECT itself, changing reality with a breath. ",
    "The world danced to a rhythm only GODNAME could hear. ",
    "The very laws of magic were but suggestions to GODNAME. ",
    "Its voice could sunder mountains and calm the most savage of beasts. ",
    "The FACTION, under GODNAME's guidance, built cities that touched the clouds. ",
    "The might of GODNAME was such that other gods paid it tribute. ",
    "In those days, every birth was a blessing from GODNAME, and every death a summons home. ",
    "The world was a tapestry, and GODNAME held the loom. ",
    "Armies of the FACTION marched under GODNAME's banner, invincible and unwavering. ",
    "The knowledge of ASPECT was a gift, bestowed upon the worthy by GODNAME. ",
    "There was no darkness deep enough to hide from GODNAME's sight. ",
    "The lifeblood of the world pulsed in time with the heart of GODNAME. ",
};

// End with a space
const char* ag_desc_god_fall_data[] =
{
    "However, followers of Ignis all but destroyed the memory of GODNAME. ",
    "However, followers of Cheibriados thought it was all a bit too fast-paced and almost destroyed GODNAME's existence. ",
    "However, GODNAME's power dwindled as the planets changed their alignment and almost all worshippers moved on. ",
    "However, since the slaughter at Saar'ylen, GODNAME's following has dwindled. ",
    "However, as thousands of years passed, GODNAME drifted into obscurity. ",
    "However, as the millennial planetary alignments occurred, a whole new pantheon of gods emerged, who swiftly eroded GODNAME's power into obscurity. ",
    "However, a great, but tragic, event, not recorded by modern scholars, occurred, all but destroying GODNAME's influence. ",
    "However, as the FACTION moved away from their religious roots, so GODNAME's power waned. ",
    "But a rival god, jealous of GODNAME's dominion over ASPECT, laid a trap and bound GODNAME for an eternity. ",
    "But in a war that shook the heavens, GODNAME was defeated and cast down. ",
    "But the FACTION grew proud and turned away from their patron, leaving GODNAME to wither. ",
    "But a trusted acolyte, hungry for power, whispered poison in the ears of the faithful, turning them against their master. ",
    "But the Great Comet, a scar across the heavens, severed GODNAME's connection to its worshippers, leaving it adrift. ",
    "A new philosophy took root among the people, one of cold logic and reason, and there was no place left for GODNAME. ",
    "In a fit of divine hubris, GODNAME sought to unmake a rival, but the backlash of power shattered its own essence. ",
    "GODNAME simply grew tired, its divine will fading like a dying star. ",
    "The source of GODNAME's power, a sacred place of ASPECT, was corrupted, poisoning the god from within. ",
    "The prayers of the FACTION turned to pleas, then to curses, and GODNAME's might crumbled under their weight. ",
    "The language in which GODNAME's name was spoken was lost, and so the god became truly nameless and powerless. ",
    "A being from beyond the stars devoured GODNAME's worshippers, leaving the god to starve. ",
    "The world simply changed, its fundamental nature shifting until GODNAME was no longer relevant. ",
    "In its moment of greatest triumph, GODNAME was betrayed by its own shadow, which rose up to usurp it. ",
    "The FACTION, in their fear, performed a great ritual to seal GODNAME away, trading worship for safety. ",
    "A prophecy foretold GODNAME's fall, and so it came to pass, an inevitable, slow decay. ",
    "The very ASPECT that gave GODNAME its strength became its undoing, turning inward to consume it. ",
};

// Final part - do not end with a space
const char* ag_desc_god_now_data[] =
{
    "Now, GODNAME seeks revenge.",
    "Now, GODNAME seeks the Orb of Zot to reclaim its power.",
    "Now, GODNAME lies in wait, gathering power once again.",
    "Now, GODNAME draws power from the dreams of the FACTION, awaiting a day of power once again.",
    "Now, GODNAME seeks followers to build its power.",
    "Now, GODNAME calls out to you. \"Will you be my champion? Together we can reclaim my godly throne.\"",
    "Now, GODNAME dreams only of ferocious revenge.",
    "Now, GODNAME whispers in the margins of old books, turning over-curious scholars and their students to its aid.",
    "Now, a faint memory in the world, GODNAME stirs in the deep places of the world.",
    "Now, GODNAME offers a sliver of its remaining power in exchange for a champion.",
    "Now, a shard of what it once was, GODNAME seethes in the dark, plotting a return steeped in vengeance.",
    "Scrabbling for purchase in a world that has forgotten it, GODNAME offers its fading power to any who will listen.",
    "It no longer thunders from on high, but whispers in the dreams of outcasts and the ambitions of the desperate.",
    "It does not seek to reclaim its throne, but merely to find a final, quiet place to rest, free from the echoes of its past.",
    "Its only desire is to see the works of the new gods turned to dust.",
    "It has become a parasite, clinging to the edges of reality, looking for a host to carry its will.",
    "Bound to its ruined temple, it can only watch and wait for a fool or a hero to stumble upon its altar.",
    "It sings a silent, sorrowful song that only the mad can hear, calling them to its service.",
    "Reduced to a mere intellect, it seeks a mind strong enough to contain its knowledge and enact its plans.",
    "It is a wound in the fabric of the world, and it seeks to either heal or fester.",
    "It offers bargains to the desperate, trading fragments of ancient power for acts of devotion.",
    "It seeks not to be worshipped, but to be remembered.",
    "The last of its power is a seed, and it seeks fertile ground in the soul of a mortal to plant it.",
    "Its consciousness is fragmented, scattered across lost artifacts, and it yearns to be made whole.",
    "It is a ghost haunting its own legacy, and it will grant great power to anyone who helps it rest.",
}; 