#pragma once

#include "tag-version.h"

enum ag_passive_type
{
    AGP_SHROUD,
    NUM_AGPS,
    MAX_AGPS = 255,
};

COMPILE_CHECK(NUM_AGPS<MAX_AGPS); //uint8