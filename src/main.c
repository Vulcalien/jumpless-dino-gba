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
#include "main.h"

#include "screen.h"
#include "interrupt.h"
#include "sound.h"
#include "input.h"
#include "performance.h"
#include "scene.h"
#include "sram.h"

u32 tick_count = 0;
u32 high_score = 0;
u32 score = 0;

static inline void tick(void) {
    input_tick();
    scene->tick();

    performance_tick();
}

static inline void draw(void) {
    scene->draw();

    performance_draw();
}

static inline void load_high_score(void) {
    // check for the Game Code (ZJDE)
    if(SRAM[0] != 'Z' ||
       SRAM[1] != 'J' ||
       SRAM[2] != 'D' ||
       SRAM[3] != 'E')
        return;

    high_score = SRAM[7] << 24 | SRAM[6] << 16 | SRAM[5] << 8 | SRAM[4];
}

int AgbMain(void) {
    screen_init();
    scene_set(&scene_start, 0);

    interrupt_enable();
    sound_init();

    load_high_score();

    // DEBUG
    screen_set_palette(0x0000, 0x7fff);
    scene_set(&scene_game, 1);

    while(true) {
        tick();

        vsync();
        draw();
    }
    return 0;
}
