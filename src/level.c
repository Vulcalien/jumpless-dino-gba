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
#include "level.h"

#include "entity.h"
#include "screen.h"
#include "map.h"

static u32 scroll_speed = 0;

// keeps track of subpixel movement
static u32 scroll_progress = 0;

static inline void spawn_player(struct Level *level) {
    struct entity_Data *data = &level->entities[0];

    data->type = ENTITY_PLAYER;

    data->x = 8;
    data->y = 8;

    data->should_remove = false;
}

static inline void draw_tiles(void) {
    // Make everything transparent
    for(u32 y = 0; y <= 31; y++)
        for(u32 x = 0; x <= 31; x++)
            BG1_TILEMAP[y * 32 + x] = 64;

    for(u32 y = 4; y < 8; y++) {
        for(u32 x = 0; x <= 15; x++) {
            BG1_TILEMAP[y * 2 * 32 + x * 2] = 0;
            BG1_TILEMAP[y * 2 * 32 + (x * 2 + 1)] = 1;
            BG1_TILEMAP[(y * 2 + 1) * 32 + x * 2] = 8;
            BG1_TILEMAP[(y * 2 + 1) * 32 + (x * 2 + 1)] = 9;
        }
    }

    // draw borders
    for(u32 x = 0; x <= 15; x++) {
        BG1_TILEMAP[7 * 32 + x * 2] = 2;
        BG1_TILEMAP[7 * 32 + x * 2 + 1] = 3;

        BG1_TILEMAP[16 * 32 + x * 2] = 2 | 1 << 11;
        BG1_TILEMAP[16 * 32 + x * 2 + 1] = 3 | 1 << 11;
    }
}

void level_init(struct Level *level) {
    draw_tiles();

    scroll_speed = 256;

    scroll_progress = 0;
    level->scroll_amount = 0;

    for(u32 i = 1; i < LEVEL_ENTITY_LIMIT; i++)
        level->entities[i].type = ENTITY_INVALID;

    spawn_player(level);
    score = 0;
}

static inline void tick_entities(struct Level *level) {
    for(u32 i = 0; i < LEVEL_ENTITY_LIMIT; i++) {
        struct entity_Data *data = &level->entities[i];
        if(data->type >= ENTITY_TYPES)
            continue;

        const struct Entity *entity = entity_type(data);
        entity->tick(level, data);

        if(data->should_remove)
            data->type = ENTITY_INVALID;
    }
}

static u32 spawn_search_index = 0;
static inline void spawn_entity(struct Level *level,
                                u32 entity, u32 row, u32 offset) {
    // DEBUG
    entity = 1;

    if(entity == 0)
        return;

    while(level->entities[spawn_search_index].type < ENTITY_TYPES &&
          spawn_search_index < LEVEL_ENTITY_LIMIT)
        spawn_search_index++;

    if(spawn_search_index < LEVEL_ENTITY_LIMIT) {
        struct entity_Data *data = &level->entities[spawn_search_index];

        data->type = entity;
        data->x = 256 - offset;
        data->y = row * 16 + 8;

        data->should_remove = false;

        if(entity == ENTITY_CACTUS)
            data->data[0] = rand();
    }
}

static inline void spawn_column(struct Level *level,
                                u32 column, u32 offset) {
    // reset the search index
    spawn_search_index = 0;

    for(u32 i = 0; i < 4; i++) {
        // TODO test if this calculation is correct
        u32 entity = map[(column % (sizeof(map) / 4)) * 4 + i];

        spawn_entity(level, entity, i, offset);
    }
}

IWRAM_SECTION
void level_tick(struct Level *level) {
    scroll_progress += scroll_speed;
    level->scroll_amount = scroll_progress / 256;
    scroll_progress %= 256;

    score += level->scroll_amount;

    if(scroll_speed < 1024) {
        if(tick_count % 30 == 0)
            scroll_speed++;
    }

    BG1_XOFFSET = score % 16;

    static u32 last_column = 0;
    u32 current_column = score / 16;
    while(last_column < current_column) {
        last_column++;
        spawn_column(
            level, last_column,
            score % 16 + (current_column - last_column) * 16
        );
    }

    tick_entities(level);
}

IWRAM_SECTION
void level_draw(struct Level *level) {
    // draw entities
    for(u32 i = 0; i < LEVEL_ENTITY_LIMIT; i++) {
        struct entity_Data *data = &level->entities[i];
        const struct Entity *entity = entity_type(data);

        u32 sprite_index = 127 - i;

        if(entity)
            entity->draw(level, data, sprite_index);
        else
            OAM[sprite_index * 4] = 1 << 9;
    }

    // draw high score
    if(high_score) {
        screen_write("HI 00000", 1, 14, 2);
        screen_write_number(high_score, 1, 21, 2, true);
    }

    // draw score
    screen_write("00000", 0, 23, 2);
    screen_write_number(score / 8, 0, 27, 2, true);
}
