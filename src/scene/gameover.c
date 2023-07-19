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
#include "scene.h"

#include "input.h"
#include "screen.h"
#include "sram.h"

static u32 restart_cooldown = 0;

static void gameover_init(u32 flags) {
    u32 new_score = actual_score();

    if(new_score > high_score) {
        high_score = new_score;

        SRAM[0] = 'Z';
        SRAM[1] = 'J';
        SRAM[2] = 'D';
        SRAM[3] = 'E';

        SRAM[4] = high_score;
        SRAM[5] = high_score >> 8;
        SRAM[6] = high_score >> 16;
        SRAM[7] = high_score >> 24;
    }

    restart_cooldown = 10;
}

static void gameover_tick(void) {
    if(restart_cooldown > 0) {
        restart_cooldown--;
        return;
    }

    if(INPUT_PRESSED(KEY_A)      || INPUT_PRESSED(KEY_B) ||
       INPUT_PRESSED(KEY_UP)     || INPUT_PRESSED(KEY_DOWN) ||
       INPUT_PRESSED(KEY_LEFT)   || INPUT_PRESSED(KEY_RIGHT) ||
       INPUT_PRESSED(KEY_L)      || INPUT_PRESSED(KEY_R)) {
        scene_set(&scene_game, 1);
    }
}

static void gameover_draw(void) {
    screen_write("GAME  OVER", 0, 10, 4);

    // draw the 'play again' icon
    BG0_TILEMAP[14 + 5 * 32] = 22;
    BG0_TILEMAP[15 + 5 * 32] = 23;
    BG0_TILEMAP[14 + 6 * 32] = 30;
    BG0_TILEMAP[15 + 6 * 32] = 31;
}

const struct Scene scene_gameover = {
    .init = gameover_init,
    .tick = gameover_tick,
    .draw = gameover_draw
};
