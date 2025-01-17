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

#include "../../inc/MarlinConfigPre.h"

#if ANY(COOLANT_MIST, COOLANT_FLOOD, AIR_ASSIST)

#include "../gcode.h"
#include "../../module/planner.h"

#if ENABLED(COOLANT_MIST)
  /**
   * M7: Mist Coolant On
   */
  void GcodeSuite::M7() {
    planner.synchronize();                            // Wait for move to arrive
    WRITE(COOLANT_MIST_PIN, !(COOLANT_MIST_INVERT));  // Turn on Mist coolant
  }
#endif

#if ANY(COOLANT_FLOOD, AIR_ASSIST)

  #if ALL(E3S1PRO_RTS, HAS_CUTTER)
    #include "../../feature/spindle_laser.h"
  #endif

  /**
   * M8: Flood Coolant / Air Assist ON
   */
  void GcodeSuite::M8() {
    planner.synchronize();                            // Wait for move to arrive
    #if ENABLED(COOLANT_FLOOD)
      WRITE(COOLANT_FLOOD_PIN, !(COOLANT_FLOOD_INVERT)); // Turn on Flood coolant
    #endif
    #if ENABLED(AIR_ASSIST)
      cutter.air_assist_enable();                     // Turn on Air Assist
    #endif
  }

#endif

/**
 * M9: Coolant / Air Assist OFF
 */
void GcodeSuite::M9() {
  planner.synchronize();                              // Wait for move to arrive
  #if ENABLED(COOLANT_MIST)
    WRITE(COOLANT_MIST_PIN, COOLANT_MIST_INVERT);     // Turn off Mist coolant
  #endif
  #if ENABLED(COOLANT_FLOOD)
    WRITE(COOLANT_FLOOD_PIN, COOLANT_FLOOD_INVERT);   // Turn off Flood coolant
  #endif
  #if ENABLED(AIR_ASSIST)
    cutter.air_assist_disable();                      // Turn off Air Assist
  #endif
}

#endif // COOLANT_MIST | COOLANT_FLOOD | AIR_ASSIST
