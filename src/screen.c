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
#include "screen.h"

#define DISPLAY_CONTROL *((vu16 *) 0x04000000)
#define DISPLAY_STATUS  *((vu16 *) 0x04000004)

#define WINDOW_IN  *((vu16 *) 0x04000048)
#define WINDOW_OUT *((vu16 *) 0x0400004a)

#define CHAR_BLOCK_0 ((vu16 *) 0x06000000)
#define CHAR_BLOCK_1 ((vu16 *) 0x06004000)
#define CHAR_BLOCK_2 ((vu16 *) 0x06008000)
#define CHAR_BLOCK_3 ((vu16 *) 0x0600c000)

#define OBJ_TILESET ((vu16 *) 0x06010000)

#define BG0_CONTROL *((vu16 *) 0x04000008)
#define BG1_CONTROL *((vu16 *) 0x0400000a)
#define BG2_CONTROL *((vu16 *) 0x0400000c)
#define BG3_CONTROL *((vu16 *) 0x0400000e)

#define BG_PALETTE  ((vu16 *) 0x05000000)
#define OBJ_PALETTE ((vu16 *) 0x05000200)

#include "res/sprites.c"
#include "res/sprites_dino.c"
#include "res/tileset.c"

void screen_init(void) {
    DISPLAY_CONTROL = 0       | // Video mode
                      1 << 6  | // OBJ Character mapping (1 is linear)
                      1 << 7  | // Forced Blank
                      0 << 8  | // Enable BG 0
                      1 << 9  | // Enable BG 1
                      0 << 10 | // Enable BG 2
                      0 << 11 | // Enable BG 3
                      1 << 12;  // Enable OBJ

    BG1_CONTROL = 1 << 0 | // Priority
                  0 << 2 | // Tile Data character block (=16K)
                  8 << 8;  // Map Data screen block (=2K)

    // load tileset and sprites into VRAM
    memcpy16(CHAR_BLOCK_0, (vu16 *) tileset,
             sizeof(tileset) / sizeof(u16));
    memcpy16(OBJ_TILESET,  (vu16 *) sprites,
             sizeof(sprites) / sizeof(u16));
    memcpy16(OBJ_TILESET + 32 * 16, (vu16 *) sprites_dino,
             sizeof(sprites_dino) / sizeof(u16));

    BG_PALETTE[0]  = 0x7c1f;
    OBJ_PALETTE[0] = 0x7c1f;

    // hide all sprites
    for(u32 i = 0; i < 128; i++)
        OAM[i * 4] = 1 << 9;

    // enable V-Blank IRQ
    DISPLAY_STATUS = (1 << 3);

    // Disable Forced Blank
    DISPLAY_CONTROL &= ~(1 << 7);
}

void screen_set_palette(u16 a, u16 b) {
    BG_PALETTE[1] = a;
    BG_PALETTE[2] = b;

    OBJ_PALETTE[1] = a;
    OBJ_PALETTE[2] = b;
}

IWRAM_SECTION
void vsync(void) {
    __asm__ volatile ("swi 0x05 << 16");
}
