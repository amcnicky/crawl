#pragma once
// Used only in world_reacts()
void player_reacts();
void player_reacts_to_monsters();

// Only functions other than decrement_duratons() which use decrement_a_duration()
void extract_manticore_spikes(const char* endmsg);
