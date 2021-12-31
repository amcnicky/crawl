#pragma once

struct ritual_def
{
    ability_type  ritual;        // The ability that the player will be get
                                    // after completing the ritual

    const char*   ritual_text;      // The title of the ritual
    const char*   milestone_text;   // Format: "began <foo>
    int           base_piety;       // The piety that needs to be earned to 
                                    // complete the ritual
};

static const ritual_def rit_data[] =
{

{ 
  ABIL_YIB_TEST_LITTLE,
  "Ritual of the blinky",
  "Began the Ritual of the blinky",
  80,
},

{ 
  ABIL_YIB_TEST_MEDIUM,
  "Ritual of the medium blinky",
  "Began the Ritual of the blinky blinky",
  120,
},

{ 
  ABIL_YIB_TEST_BIG,
  "Ritual of the hard blinky",
  "Began the Ritual of the blinky blinky blinky",
  160,
},

};
