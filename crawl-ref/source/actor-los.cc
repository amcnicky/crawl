#include "AppHdr.h"

#include "actor.h"

#include "coord.h"
#include "god-passive.h"
#include "los-type.h"
#include "losglobal.h"
#include "state.h"
#include "player.h"

bool actor::observable() const
{
    return crawl_state.game_is_arena() || is_player() || you.can_see(*this);
}

bool actor::see_cell(const coord_def &p) const
{
    if (!in_bounds(pos()))
        return false; // actor is off the map

    return cell_see_cell(pos(), p, LOS_DEFAULT);
}

bool player::see_cell(const coord_def &p) const
{
    if (!map_bounds(p))
        return false; // Players can't see (-1,-1) but maybe can see (0,0).
    if (crawl_state.game_is_arena() || crawl_state.arena_suspended)
        return true;
    if (!in_bounds(pos()))
        return false; // A non-arena player at (0,0) can't see anything.
    if (wizard_vision || you.duration[DUR_REVELATION])
        return (pos() - p).rdist() <= current_vision;
    
    // Obsidian Gateweb temporary vision - see around gate positions
    if (you.duration[DUR_OBSIDIAN_GATEWEB_VISION] && you.props.exists("obsidian_gateweb_positions"))
    {
        const CrawlVector& gate_positions = you.props["obsidian_gateweb_positions"].get_vector();
        for (const auto& gate_item : gate_positions)
        {
            const coord_def gate_pos = gate_item.get_coord();
            // Safety check: ensure gate position is valid before using it
            if (!in_bounds(gate_pos))
                continue;
            // Grant vision in 5x5 area around each gate (2 radius)
            if ((gate_pos - p).rdist() <= 2)
                return true;
        }
    }
    
    return actor::see_cell(p);
}

bool actor::can_see(const actor &target) const
{
    return target.visible_to(this) && see_cell(target.pos());
}

bool actor::see_cell_no_trans(const coord_def &p) const
{
    return cell_see_cell(pos(), p, LOS_NO_TRANS);
}

bool player::trans_wall_blocking(const coord_def &p) const
{
    return see_cell(p) && !see_cell_no_trans(p);
}

bool player::can_see(const actor& a) const
{
    if (crawl_state.game_is_arena() || crawl_state.arena_suspended)
        return see_cell(a.pos());
    else
        return actor::can_see(a);
}
