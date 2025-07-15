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

/** @file clock.c
 ** @brief clock para la creación de archivos de código fuente en lenguaje C
 **/

/* === Headers files inclusions ==================================================================================== */

#include "clock.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
/* === Macros definitions ========================================================================================== */

/* === Private data type declarations ============================================================================== */

/* === Private function declarations =============================================================================== */

struct clock_s {
    clock_time_t current_time;
    clock_time_t alarm_time;
    bool valid;
    bool alarm_enabled;
    bool alarm_triggered_flag;
    uint16_t tick_count;
    uint16_t ticks_per_second;
};
/* === Private variable definitions ================================================================================ */

/* === Public variable definitions ================================================================================= */

/* === Private function definitions ================================================================================ */

/* === Public function definitions ============================================================================== */


clock_t clock_create(uint16_t ticks_per_second) {
    static struct clock_s self[1];// declaración de la estructura del reloj
    memset(self, 0, sizeof(struct clock_s));// inicializa la estructura a cero
    self->ticks_per_second = ticks_per_second;// establece los ticks por segundo
    return self;
}
bool clock_time_is_valid(clock_t clock) {
    // Implementación de la verificación de la validez del tiempo del reloj
    // Aquí se puede verificar si el reloj tiene un tiempo válido
    //como no lo necesito pero uso un puntero tengo que castear la variable
    //con void le digo que no se usa
    (void) clock;
    return false; 
}
bool clock_get_time(clock_t self, clock_time_t *result) {
    if (!self || !result) return false;
    memcpy(result, &self->current_time, sizeof(clock_time_t));
    return self->valid;
}


bool clock_set_time(clock_t self, const clock_time_t *new_time){
    if (!self || !new_time) return false;
    self->valid = true;
    memcpy(&self->current_time, new_time, sizeof(clock_time_t));
    return self->valid;
}


static void bcd_increment(clock_time_t *time) {
    if (++time->bcd[0] > 9) {
        time->bcd[0] = 0;
        if (++time->bcd[1] > 5) {
            time->bcd[1] = 0;
            if (++time->bcd[2] > 9) {
                time->bcd[2] = 0;
                if (++time->bcd[3] > 5) {
                    time->bcd[3] = 0;
                    if (++time->bcd[4] > 9) {
                        time->bcd[4] = 0;
                        if (++time->bcd[5] > 2 || (time->bcd[5] == 2 && time->bcd[4] > 3)) {
                            memset(time, 0, sizeof(clock_time_t));//memset lo que hace es setear a 00:00:00
                        }
                    }
                }
            }
        }
    }
}

void clock_new_tick(clock_t clock) {
    if (!clock->valid) return;
    clock->tick_count++;
    if (clock->tick_count >= clock->ticks_per_second) {
        clock->tick_count = 0;
        bcd_increment(&clock->current_time);

        if (clock->alarm_enabled &&
            memcmp(&clock->current_time, &clock->alarm_time, sizeof(clock_time_t)) == 0) {
            clock->alarm_triggered_flag = true;
        }
    }
}

bool clock_set_alarm_time(clock_t clock, const clock_time_t *alarm_time) {
    if (!clock || !alarm_time) return false;
    memcpy(&clock->alarm_time, alarm_time, sizeof(clock_time_t));
    return true;
}


bool clock_get_alarm_time(clock_t clock, clock_time_t *alarm_time) {
    if (!clock || !alarm_time) return false;
    memcpy(alarm_time, &clock->alarm_time, sizeof(clock_time_t));
    return true;
}


void clock_enable_alarm(clock_t clock) {
    clock->alarm_enabled = true;// habilita la alarma
    clock->alarm_triggered_flag = false;// reinicia el indicador de alarma activada
    // Esta línea establece la alarma como habilitada y reinicia el indicador de alarma activada
    // para que no se active inmediatamente al habilitarla.
    // Esto permite que la alarma se active solo cuando el tiempo actual del reloj coincide con la
    // hora de la alarma establecida.
}

void clock_disable_alarm(clock_t clock) {
    clock->alarm_enabled = false;// deshabilita la alarma
    clock->alarm_triggered_flag = false;// desactiva la alarma
}

bool clock_is_alarm_enabled(clock_t clock) {
    return clock->alarm_enabled;
}

bool clock_alarm_triggered(clock_t clock) {
    return clock->alarm_triggered_flag;
}

void clock_snooze_alarm(clock_t clock, uint8_t minutes) {
    clock->alarm_triggered_flag = false;
    for (uint8_t i = 0; i < minutes; i++) {
        if (++clock->alarm_time.bcd[2] > 9) {
            clock->alarm_time.bcd[2] = 0;
            if (++clock->alarm_time.bcd[3] > 5) {
                clock->alarm_time.bcd[3] = 0;
                if (++clock->alarm_time.bcd[4] > 9) {
                    clock->alarm_time.bcd[4] = 0;
                    if (++clock->alarm_time.bcd[5] > 2 ||
                        (clock->alarm_time.bcd[5] == 2 && clock->alarm_time.bcd[4] > 3)) {
                        memset(&clock->alarm_time, 0, sizeof(clock_time_t));
                    }
                }
            }
        }
    }
}

/* === End of documentation ======================================================================================== */