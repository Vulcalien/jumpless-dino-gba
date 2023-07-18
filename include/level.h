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
#ifndef VULC_TEMPLATE_LEVEL
#define VULC_TEMPLATE_LEVEL

#include "main.h"

#include "entity.h"

#define LEVEL_ENTITY_LIMIT (128)

struct Level {
    u32 scroll_speed;
    u32 scroll_progress;

    // the scroll amount in pixels for the current tick
    u32 scroll_amount;

    u32 score;

    struct entity_Data entities[LEVEL_ENTITY_LIMIT];
};

extern void level_init(struct Level *level);
extern void level_tick(struct Level *level);
extern void level_draw(struct Level *level);

#endif // VULC_TEMPLATE_LEVEL
