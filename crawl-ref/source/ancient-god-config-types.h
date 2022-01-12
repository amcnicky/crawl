#pragma once

#include "tag-version.h"

enum ag_passive_type
{
    AG_P_RESCUE_MAMMAL,
    AG_P_DISCOUNT_SHOPPING,
    AG_P_POTION_REFILLS,
    AG_P_SPRIGGAN_SIDEKICK,
    NUM_AGP,
    MAX_AGP = 255,
};

enum ag_abil_small_type
{
    AG_S_HEROISM,
    NUM_AGAS,
    MAX_AGAS = 255,
};

enum ag_abil_cap_type
{
    AG_C_DISASTER_AREA,
    NUM_AGAC,
    MAX_AGAC = 255,
};

enum ag_mut_type
{
    AG_M_HP_CASTING,
    NUM_AGM,
    MAX_AGM = 255,
};

COMPILE_CHECK(NUM_AGP<MAX_AGP); //uint8
COMPILE_CHECK(NUM_AGAS<MAX_AGAS); //uint8
COMPILE_CHECK(NUM_AGAC<MAX_AGAC); //uint8
COMPILE_CHECK(NUM_AGM<MAX_AGM); //uint8