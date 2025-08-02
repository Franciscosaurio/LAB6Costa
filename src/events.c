/*********************************************************************************************************************
Copyright (c) 2025, Costa Francisco Lucas Sebastian.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

SPDX-License-Identifier: MIT
*********************************************************************************************************************/

/** @file events.c
 ** @brief events para la creación de archivos de código fuente en lenguaje C
 **/

/* === Headers files inclusions ==================================================================================== */

#include "events.h"

/* === Macros definitions ========================================================================================== */

/* === Private data type declarations ============================================================================== */

/* === Private function declarations =============================================================================== */

/* === Private variable definitions ================================================================================ */

/* === Public variable definitions ================================================================================= */

/* === Private function definitions ================================================================================ */

/* === Public function implementation ============================================================================== */
modo_t modo_create(void) {
    return MODO_INVALIDO;  // Devuelve el estado inicial directamente
}

void time_increments(clock_time_t *time, modo_t modo) {
    if (modo == MODO_SET_MINUTO) {
        if (++time->bcd[2] > 9) {  // Mu (hora[0])
            time->bcd[2] = 0;
            if (++time->bcd[3] > 5) {  // Md (hora[1])
                time->bcd[3] = 0;
            }
        }
    } else if (modo == MODO_SET_HORA) {
        // Convertir a valor entero para facilitar control de límite 23
        uint8_t horas = time->bcd[5] * 10 + time->bcd[4];
        horas = (horas + 1) % 24;
        time->bcd[5] = horas / 10;
        time->bcd[4] = horas % 10;
    }
}

void time_decrement(clock_time_t *time, modo_t modo) {
    if (modo == MODO_SET_MINUTO) {
        if (time->bcd[2] == 0) {  // Mu (hora[0])
            time->bcd[2] = 9;
            if (time->bcd[3] == 0) {  // Md (hora[1])
                time->bcd[3] = 5;
            } else {
                time->bcd[3]--;
            }
        } else {
            time->bcd[2]--;
        }
    } else if (modo == MODO_SET_HORA) {
        uint8_t horas = time->bcd[5] * 10 + time->bcd[4];
        if (horas == 0) {
            horas = 23;
        } else {
            horas--;
        }
        time->bcd[5] = horas / 10;
        time->bcd[4] = horas % 10;
    }
}
void get_mode(bool tecla, modo_t *modo, key_t key, clock_t reloj, clock_time_t *time) {
    if (tecla) {        
        key->contador++;
        key->normal = false;
        if (key->contador > 3) {
            key->long_press = true;
            key->press=false;
            *modo = MODO_SET_MINUTO;  // Activar modo de ajuste de hora
            
        }
    } else {
        key->inactivo++;
        key->long_press=false;
        key->press = false;
        key->normal = true;
        if (key->inactivo > 10) {
            if(!(clock_time_is_valid(reloj))){
                key->inactivo = 0;
                key->normal = true;
                key->long_press = false;
                key->press = false;
                *modo = MODO_INVALIDO;  // Volver a modo inactivo
            }else{
                key->inactivo = 0;
                key->normal = true;
                key->long_press = false;
                key->press = false;
                *modo = MODO_NORMAL;  // Volver a modo normal
            }
            
        }
    }

    // Si no se presiona nada por más de 30 ciclos, volver a modo normal
    
}

key_t key_create(void) {
    key_t key = malloc(sizeof(struct key_s));
    if (key != NULL) {
        key->normal = false;
        key->long_press = false;
        key->press = false;
    }
    return key;
}
/* === End of documentation ======================================================================================== */