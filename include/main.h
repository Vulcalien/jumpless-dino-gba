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
#ifndef VULC_TEMPLATE_CORE
#define VULC_TEMPLATE_CORE

#include "types.h"
#include "base.h"
#include "util.h"

extern u32 tick_count;

extern u32 high_score;
extern u32 score;

inline u32 actual_score(void) {
    return score / 16;
}

#endif // VULC_TEMPLATE_CORE
