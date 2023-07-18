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
    data->y = 80;

    data->should_remove = false;
}

void level_init(struct Level *level) {
    scroll_speed = 64;

    scroll_progress = 0;
    level->scroll_amount = 0;

    for(u32 i = 1; i < LEVEL_ENTITY_LIMIT; i++)
        level->entities[i].type = ENTITY_INVALID;

    spawn_player(level);
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
    entity = 2;

    if(entity == 0)
        return;

    while(level->entities[spawn_search_index].type < ENTITY_TYPES &&
          spawn_search_index < LEVEL_ENTITY_LIMIT)
        spawn_search_index++;

    if(spawn_search_index < LEVEL_ENTITY_LIMIT) {
        struct entity_Data *data = &level->entities[spawn_search_index];

        data->type = entity;
        data->x = 256 - offset;
        data->y = 64 + row * 16;

        data->should_remove = false;
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

    level->score += level->scroll_amount;

    if(scroll_speed < 512) {
        if(tick_count % 30 == 0)
            scroll_speed++;
    }

    BG1_XOFFSET = level->score;

    static u32 last_column = 0;
    u32 current_column = level->score / 16;
    while(last_column < current_column) {
        last_column++;
        spawn_column(
            level, last_column,
            level->score % 16 + (current_column - last_column) * 16
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
}