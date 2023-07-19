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
#include "input.h"
#include "screen.h"
#include "scene.h"

static inline void player_die(struct Level *level,
                              struct entity_Data *data) {
    level->running = false;

    // change the player's sprite
    vsync();
    OAM[127 * 4 + 2] = 80 << 0  | // Tile Number
                       1  << 10;  // Priority

    scene_set(&scene_gameover, 1);
}

IWRAM_SECTION
static void player_tick(struct Level *level, struct entity_Data *data) {
    static bool going_up   = false;
    static bool going_down = false;

    if(data->y % 16 == 8) {
        if(INPUT_DOWN(KEY_UP) && data->y != 8)
            going_up = true;
        else
            going_up = false;

        if(INPUT_DOWN(KEY_DOWN) && data-> y != 56)
            going_down = true;
        else
            going_down = false;
    }

    if(going_up)
        entity_move(level, data, 0, -2);
    else if(going_down)
        entity_move(level, data, 0, +2);

    if(INPUT_DOWN(KEY_A) || INPUT_DOWN(KEY_B))
        data->data[0] = true;
    else
        data->data[0] = false;

    // check for contact with other entities
    for(u32 i = 1; i < LEVEL_ENTITY_LIMIT; i++) {
        struct entity_Data *data2 = &level->entities[i];
        if(data->type >= ENTITY_TYPES)
            continue;

        if(entity_touches(data, data2)) {
            bool crouched = data->data[0];

            bool dies = false;
            if(data2->type == ENTITY_CACTUS)
                dies = true;
            else if(data2->type == ENTITY_BIRD)
                dies = !crouched;

            if(dies)
                player_die(level, data);
        }
    }
}

IWRAM_SECTION
static void player_draw(struct Level *level, struct entity_Data *data,
                        u32 sprite_index) {
    vu16 *sprite_attribs = &OAM[sprite_index * 4];

    u32 x = data->x - 16;
    u32 y = data->y - 16 + LEVEL_OFFSET - 1;

    sprite_attribs[0] = (y & 0xff) << 0; // Y coordinate
    sprite_attribs[1] = (x & 0x1ff) << 0 | // X coordinate
                        2           << 14; // Sprite Size (2 is 32x32)

    u32 sprite_tile = 48 + 16 * ((tick_count / 8) % 2);

    bool crouched = data->data[0];
    sprite_tile += crouched * 48;

    sprite_attribs[2] = sprite_tile << 0  | // Tile Number
                        1           << 10;  // Priority
}

const struct Entity entity_player = {
    .xr = 6,
    .yr = 5,

    .tick = player_tick,
    .draw = player_draw
};
