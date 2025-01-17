/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
 *
 */
#pragma once

#include "../inc/MarlinConfig.h"
#include "../module/probe.h"

#if !WITHIN(TRAMMING_SCREW_THREAD, 30, 51) || TRAMMING_SCREW_THREAD % 10 > 1
  #error "TRAMMING_SCREW_THREAD must be equal to 30, 31, 40, 41, 50, or 51."
#endif

#define G35_PROBE_COUNT 4

void updateTrammingPoints();
static_assert(WITHIN(G35_PROBE_COUNT, 3, 9), "TRAMMING_POINT_XY requires between 3 and 9 XY positions.");

#ifdef TRAMMING_POINT_NAME_9
  #define _NR_TRAM_NAMES 9
#elif defined(TRAMMING_POINT_NAME_8)
  #define _NR_TRAM_NAMES 8
#elif defined(TRAMMING_POINT_NAME_7)
  #define _NR_TRAM_NAMES 7
#elif defined(TRAMMING_POINT_NAME_6)
  #define _NR_TRAM_NAMES 6
#elif defined(TRAMMING_POINT_NAME_5)
  #define _NR_TRAM_NAMES 5
#elif defined(TRAMMING_POINT_NAME_4)
  #define _NR_TRAM_NAMES 4
#elif defined(TRAMMING_POINT_NAME_3)
  #define _NR_TRAM_NAMES 3
#else
  #define _NR_TRAM_NAMES 0
#endif

static_assert(_NR_TRAM_NAMES >= G35_PROBE_COUNT, "Define enough TRAMMING_POINT_NAME_s for all TRAMMING_POINT_XY entries.");

#define _TRAM_NAME_PTR(N) point_name_##N[]
extern const char REPLIST_1(_NR_TRAM_NAMES, _TRAM_NAME_PTR);

extern PGM_P const tramming_point_name[];

#ifdef ASSISTED_TRAMMING_WAIT_POSITION
  void move_to_tramming_wait_pos();
#else
  inline void move_to_tramming_wait_pos() {}
#endif
