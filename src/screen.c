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
                      1 << 8  | // Enable BG 0
                      1 << 9  | // Enable BG 1
                      0 << 10 | // Enable BG 2
                      0 << 11 | // Enable BG 3
                      1 << 12;  // Enable OBJ

    BG0_CONTROL = 0 << 0 | // Priority
                  0 << 2 | // Tile Data character block (=16K)
                  8 << 8;  // Map Data screen block (=2K)

    BG1_CONTROL = 1 << 0 | // Priority
                  0 << 2 | // Tile Data character block (=16K)
                  9 << 8;  // Map Data screen block (=2K)

    // load tileset and sprites into VRAM
    memcpy16(CHAR_BLOCK_0, (vu16 *) tileset,
             sizeof(tileset) / sizeof(u16));
    memcpy16(OBJ_TILESET,  (vu16 *) sprites,
             sizeof(sprites) / sizeof(u16));
    memcpy16(OBJ_TILESET + 32 * 16, (vu16 *) sprites_dino,
             sizeof(sprites_dino) / sizeof(u16));

    // Set backdrop color
    BG_PALETTE[0]  = 0x7c1f;
    OBJ_PALETTE[0] = 0x7c1f;

    // Set third color (gray)
    BG_PALETTE[2] = 0x3def;

    // Set font colors
    BG_PALETTE[15]      = 0x0000;
    BG_PALETTE[16 + 15] = 0x3def;

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
void screen_write(char *string, u32 palette, u32 x0, u32 y0) {
    u32 x = x0;
    u32 y = y0;
    while(*string) {
        char c = *string++;

        if(c == '\n') {
            x = x0;
            y++;
        } else {
            BG0_TILEMAP[x + y * 32] = c | palette << 12;
            x++;
        }
    }
}

IWRAM_SECTION
void screen_write_number(u32 number, u32 palette, u32 x0, u32 y0) {
    u32 x = x0;
    u32 y = y0;

    while(number != 0) {
        u32 digit = number % 10;
        number /= 10;

        BG0_TILEMAP[x + y * 32] = ('0' + digit) | palette << 12;
        x++;
    }
}

void screen_clear_bg0(void) {
    for(u32 y = 0; y < 32; y++)
        for(u32 x = 0; x < 32; x++)
            BG0_TILEMAP[x + y * 32] = 5;
}

IWRAM_SECTION
void vsync(void) {
    __asm__ volatile ("swi 0x05 << 16");
}
