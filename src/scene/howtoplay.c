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

static void howtoplay_init(u32 flags) {
}

static void howtoplay_tick(void) {
    if(INPUT_RELEASED(KEY_A) || INPUT_RELEASED(KEY_B) ||
       INPUT_RELEASED(KEY_START))
        scene_set(&scene_start, 2);
}

static void howtoplay_draw(void) {
    for(u32 y = 0; y < 20; y++) {
        for(u32 x = 0; x < 30; x++) {
            BG0_TILEMAP[x + y * 32] = 20;
            BG1_TILEMAP[x + y * 32] = 20;
        }
    }

    screen_write("HOW TO PLAY", 0, 9, 1);

    screen_write(
        "RUN AS FAR AS YOU CAN WHILE\n"
        "AVOIDING CACTUSES AND\n"
        "PTERODACTYLS.\n"
        "\n"
        "YOU CAN ALSO CROUCH TO AVOID\n"
        "PTERODACTYLS AND PASS BELOW\n"
        "THEM.\n"
        "\n"
        "USE THE DPAD TO MOVE UP AND\n"
        "DOWN. USE A OR B TO CROUCH.",
        0, 1, 4);

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

const struct Scene scene_howtoplay = {
    .init = howtoplay_init,
    .tick = howtoplay_tick,
    .draw = howtoplay_draw
};
