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

static void gameover_init(u32 flags) {
    high_score = score / 8;
}

static void gameover_tick(void) {
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
