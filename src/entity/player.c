/* Copyright 2023 Vulcalien
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "entity.h"

#include "level.h"
#include "screen.h"

static inline void player_die(struct Level *level,
                              struct entity_Data *data) {
    // TODO ...
}

IWRAM_SECTION
static void player_tick(struct Level *level, struct entity_Data *data) {
    for(u32 i = 1; i < LEVEL_ENTITY_LIMIT; i++) {
        struct entity_Data *data2 = &level->entities[i];
        if(data->type >= ENTITY_TYPES)
            continue;

        if(entity_touches(data, data2)) {
            bool dies = false;
            if(data2->type == ENTITY_CACTUS)
                dies = true;
            else if(data2->type == ENTITY_BIRD)
                dies = true; // TODO check if the player is crouching

            if(dies)
                player_die(level, data);
        }
    }
}

IWRAM_SECTION
static void player_draw(struct Level *level, struct entity_Data *data,
                        u32 sprite_index) {
    vu16 *sprite_attribs = &OAM[sprite_index * 4];

    sprite_attribs[0] = ((data->y - 8) & 0xff) << 0; // Y coordinate
    sprite_attribs[1] = ((data->x - 8) & 0x1ff) << 0 | // X coordinate
                        1                       << 14; // Sprite Size (1 is 16x16)

    u32 sprite_tile = 0;
    sprite_attribs[2] = sprite_tile << 0  | // Tile Number
                        1           << 10;  // Priority
}

const struct Entity entity_player = {
    .xr = 16,
    .yr = 16,

    .tick = player_tick,
    .draw = player_draw
};
