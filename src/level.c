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

void level_init(struct Level *level) {
    level->scroll_speed = 40;
    level->scroll_progress = 0;

    level->scroll_amount = 0;

    for(u32 i = 0; i < LEVEL_ENTITY_LIMIT; i++)
        level->entities[i].type = ENTITY_INVALID;
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

IWRAM_SECTION
void level_tick(struct Level *level) {
    level->scroll_progress += level->scroll_speed;

    level->scroll_amount = level->scroll_progress / 256;
    level->scroll_progress %= 256;

    level->score += level->scroll_amount;

    /*if(level->scroll_speed < 256) {*/
        /*level->scroll_speed++; // TODO way too fast!*/
    /*}*/

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
