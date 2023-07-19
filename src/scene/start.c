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

static i32 menu_selected = 0;

static void start_init(u32 flags) {
    screen_set_palette(2);

    if(flags == 1)
        menu_selected = 0;
}

static void start_tick(void) {
    if(INPUT_PRESSED(KEY_UP))
        menu_selected--;
    if(INPUT_PRESSED(KEY_DOWN))
        menu_selected++;

    if(menu_selected < 0)
        menu_selected = 2;
    else if(menu_selected >= 3)
        menu_selected = 0;

    if(INPUT_RELEASED(KEY_A) || INPUT_RELEASED(KEY_B) ||
       INPUT_RELEASED(KEY_START)) {
        switch(menu_selected) {
            case 0:
                scene_set(&scene_game, 1);
                break;
            case 1:
                scene_set(&scene_howtoplay, 1);
                break;
            case 2:
                scene_set(&scene_about, 1);
                break;
        }
    }
}

static void start_draw(void) {
    for(u32 y = 0; y < 20; y++) {
        for(u32 x = 0; x < 30; x++) {
            BG0_TILEMAP[x + y * 32] = 20;
            BG1_TILEMAP[x + y * 32] = 20;
        }
    }

    screen_write("JUMPLESS DINO!", 0, 8, 5);

    // draw menu
    screen_write("START",       menu_selected == 0 ? 0 : 1, 12, 8);
    screen_write("HOW TO PLAY", menu_selected == 1 ? 0 : 1, 9,  10);
    screen_write("ABOUT",       menu_selected == 2 ? 0 : 1, 12, 11);

    // animation
    static u32 animation_ticks = 0;
    animation_ticks++;

    // player sprite
    OAM[0] = 120;
    OAM[1] = 12 | 2 << 14;
    OAM[2] = 48 + 16 * ((animation_ticks / 8) % 2);

    // pterodacty sprite
    OAM[4 + 0] = 128;
    OAM[4 + 1] = 204 | 1 << 14;
    OAM[4 + 2] = 12 + 4 * ((animation_ticks / 16) % 2);
}

const struct Scene scene_start = {
    .init = start_init,
    .tick = start_tick,
    .draw = start_draw
};
