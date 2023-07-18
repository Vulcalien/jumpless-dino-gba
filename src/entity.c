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

const struct Entity *entity_list[ENTITY_TYPES] = {
    &entity_player,
    &entity_cactus,
    &entity_bird
};

IWRAM_SECTION
static bool move2(struct Level *level, struct entity_Data *data,
                  i32 xm, i32 ym) {
    data->x += xm;
    data->y += ym;
    return true;
}

IWRAM_SECTION
bool entity_move(struct Level *level, struct entity_Data *data,
                 i32 xm, i32 ym) {
    if(xm == 0 && ym == 0)
        return true;

    bool stopped = true;
    if(xm != 0 && move2(level, data, xm, 0))
        stopped = false;
    if(ym != 0 && move2(level, data, 0, ym))
        stopped = false;

    return !stopped;
}
