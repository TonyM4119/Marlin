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

#include "../../inc/MarlinConfig.h"

#if ENABLED(SET_PROGRESS_MANUALLY)

#include "../gcode.h"
#include "../../lcd/marlinui.h"
#include "../../sd/cardreader.h"
#include "../../libs/numtostr.h"

#if ENABLED(E3S1PRO_RTS)
  #include "../../lcd/rts/e3s1pro/lcd_rts.h"
#endif

/**
 * M73: Set percentage complete (for display on LCD)
 *
 * Example:
 *   M73 P25.63 ; Set progress to 25.63%
 *   M73 R456   ; Set remaining time to 456 minutes
 *   M73 C12    ; Set next interaction countdown to 12 minutes
 *   M73        ; Report current values
 *
 * M73 Progress: ---%; Time left: -----m; Change: -----m;
 *
 * When PRINT_PROGRESS_SHOW_DECIMALS is enabled - reports percent with 100% / 23.4% / 3.45% format
 *
 */
void GcodeSuite::M73() {

  uint16_t last_remaining_time = 0;
  uint16_t last_progress_percent = 0;

  #if ENABLED(DWIN_LCD_PROUI)

    DWIN_M73();

  #else

    #if ENABLED(SET_PROGRESS_PERCENT)
      if (parser.seenval('P')){
        #if ENABLED(E3S1PRO_RTS)
          if (parser.value_byte() == 0)
            last_start_time = HAL_GetTick();
        #endif
        if(!lcd_rts_settings.external_m73){ 
          ui.set_progress((PROGRESS_SCALE) > 1
            ? parser.value_float() * (PROGRESS_SCALE)
            : parser.value_byte()
          );
        }
        if(lcd_rts_settings.external_m73){             
          last_progress_percent = (unsigned char)((PROGRESS_SCALE) > 1
            ? parser.value_float() * (PROGRESS_SCALE)
            : parser.value_byte()
          );
          #if ENABLED(LCD_RTS_DEBUG_PRINTTIME)
            SERIAL_ECHO_MSG("last_progress_percent M73: ", last_progress_percent);
          #endif
          RTS_SendProgress(last_progress_percent);
          duration_t elapsed = print_job_timer.duration();
          rtscheck.RTS_SndData(elapsed.value / 3600, PRINT_TIME_HOUR_VP);
          rtscheck.RTS_SndData((elapsed.value % 3600) / 60, PRINT_TIME_MIN_VP);
          #if ENABLED(LCD_RTS_DEBUG_PRINTTIME)
            SERIAL_ECHO_MSG("M73 1 PRINT_TIME_HOUR_VP: ", elapsed.value / 3600);
            SERIAL_ECHO_MSG("M73 1 PRINT_TIME_MIN_VP: ", (elapsed.value % 3600));
          #endif              
        }
      }
    #endif

    #if ENABLED(SET_REMAINING_TIME)

      if (parser.seenval('R')){
        if(!lcd_rts_settings.external_m73){
          ui.set_remaining_time(60 * parser.value_int());
          #if ENABLED(LCD_RTS_DEBUG_PRINTTIME)
            SERIAL_ECHO_MSG("M73 ui.set_remaining_time: ", 60 * parser.value_ulong());
          #endif
        }
      }
      if(lcd_rts_settings.external_m73){ 
        if (parser.seenval('R')) {
          last_remaining_time = 60 * parser.value_int();
          rtscheck.RTS_SndData(last_remaining_time / 3600, PRINT_REMAIN_TIME_HOUR_VP);
          rtscheck.RTS_SndData((last_remaining_time % 3600) / 60, PRINT_REMAIN_TIME_MIN_VP);
          #if ENABLED(LCD_RTS_DEBUG_PRINTTIME)
            SERIAL_ECHO_MSG("M73 3 PRINT_REMAIN_TIME_HOUR_VP: ", last_remaining_time / 3600);
            SERIAL_ECHO_MSG("M73 3 PRINT_REMAIN_TIME_MIN_VP: ", (last_remaining_time % 3600));
          #endif          
        }      
      }
    #endif

    #if ENABLED(SET_INTERACTION_TIME)
      if (parser.seenval('C')) ui.set_interaction_time(60 * parser.value_ulong());
    #endif
  
  #endif

  #if ENABLED(M73_REPORT)
    if (TERN1(M73_REPORT_SD_ONLY, IS_SD_PRINTING())) {
      SERIAL_ECHO_START();
      SERIAL_ECHOPGM(" M73");
      #if ENABLED(SET_PROGRESS_PERCENT)
        SERIAL_ECHOPGM(" Progress: ", TERN(PRINT_PROGRESS_SHOW_DECIMALS, permyriadtostr4(ui.get_progress_permyriad()), ui.get_progress_percent()), "%;");
      #endif
      #if ENABLED(SET_REMAINING_TIME)
        SERIAL_ECHOPGM(" Time left: ", ui.remaining_time / 60, "m;");
      #endif
      #if ENABLED(SET_INTERACTION_TIME)
        SERIAL_ECHOPGM(" Change: ", ui.interaction_time / 60, "m;");
      #endif
      SERIAL_EOL();
    }
  #endif
}

#endif // SET_PROGRESS_MANUALLY
