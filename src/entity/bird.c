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

IWRAM_SECTION
static void bird_tick(struct Level *level, struct entity_Data *data) {
    entity_move(level, data, -level->scroll_amount, 0);

    if(data->x < -8)
        data->should_remove = true;
}

IWRAM_SECTION
static void bird_draw(struct Level *level, struct entity_Data *data,
                        u32 sprite_index) {
    vu16 *sprite_attribs = &OAM[sprite_index * 4];

    u32 x = data->x - 8;
    u32 y = data->y - 8 + LEVEL_OFFSET - 8;

    sprite_attribs[0] = (y & 0xff) << 0; // Y coordinate
    sprite_attribs[1] = (x & 0x1ff) << 0 | // X coordinate
                        1                       << 14; // Sprite Size (1 is 16x16)

    u32 sprite_tile = 12 + 4 * ((tick_count / 16) % 2);
    sprite_attribs[2] = sprite_tile << 0  | // Tile Number
                        1           << 10;  // Priority
}

const struct Entity entity_bird = {
    .xr = 8,
    .yr = 8,

    .tick = bird_tick,
    .draw = bird_draw
};
