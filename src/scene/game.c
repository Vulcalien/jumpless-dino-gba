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

#include "level.h"
#include "screen.h"
#include "sound.h"

static struct Level level;

static i32 current_darkness;
static i32 darkness_increase;

#define DARKNESS_MIN (2)
#define DARKNESS_MAX (27)

static void game_init(u32 flags) {
    score = 0;

    current_darkness = DARKNESS_MIN;
    darkness_increase = -1;

    screen_clear_bg0();
    screen_draw_bg2();
    level_init(&level);
}

IWRAM_SECTION
static void game_tick(void) {
    u32 previous_score = actual_score();

    level_tick(&level);

    // if a score multiple of 100 has been hit, play a sound effect
    if(actual_score() / 100 != previous_score / 100)
        SOUND_PLAY(sound_good, sound_channel_B, false);

    // if a score multiple of 500 has been hit, toggle between day/night
    if(actual_score() / 500 != previous_score / 500)
        darkness_increase = -darkness_increase;

    // increase/decrease darkness value
    if(tick_count % 2 == 0) {
        current_darkness += darkness_increase;

        if(current_darkness < DARKNESS_MIN)
            current_darkness = DARKNESS_MIN;
        else if(current_darkness > DARKNESS_MAX)
            current_darkness = DARKNESS_MAX;
    }
}

IWRAM_SECTION
static void game_draw(void) {
    screen_set_palette(current_darkness);

    level_draw(&level);
}

const struct Scene scene_game = {
    .init = game_init,
    .tick = game_tick,
    .draw = game_draw
};
