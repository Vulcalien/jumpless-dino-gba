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
#ifndef VULC_TEMPLATE_SCREEN
#define VULC_TEMPLATE_SCREEN

#include "main.h"

#define SCREEN_W (240)
#define SCREEN_H (160)

#define VCOUNT *((vu16 *) 0x04000006)

#define BG0_XOFFSET *((vu16 *) 0x04000010)
#define BG0_YOFFSET *((vu16 *) 0x04000012)

#define BG1_XOFFSET *((vu16 *) 0x04000014)
#define BG1_YOFFSET *((vu16 *) 0x04000016)

#define BG2_XOFFSET *((vu16 *) 0x04000018)
#define BG2_YOFFSET *((vu16 *) 0x0400001a)

#define BG3_XOFFSET *((vu16 *) 0x0400001c)
#define BG3_YOFFSET *((vu16 *) 0x0400001e)

#define BG0_TILEMAP ((vu16 *) 0x06004000)
#define BG1_TILEMAP ((vu16 *) 0x06004800)
#define BG2_TILEMAP ((vu16 *) 0x06005000)

#define OAM ((vu16 *) 0x07000000)

extern void screen_init(void);

extern void screen_set_palette(u16 a, u16 b);

extern void screen_write(char *string, u32 palette, u32 x0, u32 y0);
extern void screen_write_number(u32 number, u32 palette,
                                u32 x0, u32 y0, bool right_to_left);

extern void screen_clear_bg0(void);
extern void screen_draw_bg2(void);

extern void vsync(void);

#endif // VULC_TEMPLATE_SCREEN
