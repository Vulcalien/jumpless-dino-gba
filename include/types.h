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
#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;

typedef volatile uint8_t  vu8;
typedef volatile uint16_t vu16;
typedef volatile uint32_t vu32;
typedef volatile int8_t   vi8;
typedef volatile int16_t  vi16;
typedef volatile int32_t  vi32;

// Limits
#define  U8_MAX (255)
#define U16_MAX (65535)
#define U32_MAX (4294967295)

#define  I8_MIN (-128)
#define I16_MIN (-32768)
#define I32_MIN (-2147483648)

#define  I8_MAX (127)
#define I16_MAX (32767)
#define I32_MAX (2147483647)
