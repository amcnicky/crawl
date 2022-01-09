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