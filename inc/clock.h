/*********************************************************************************************************************
Copyright (c) 2025, Costa Francisco Lucas Sebastian

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

#ifndef CLOCK_H
#define CLOCK_H

/** @file clock.h
 ** @brief declaraciones de la funcionalidad del reloj.
 **/

/* === Headers files inclusions ==================================================================================== */
#include <stdint.h>
#include <stdbool.h>
#include "screen.h"
/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */

/* === Public data type declarations =============================================================================== */

//el union es como un struct, es una forma distinta de ver al mismo almaceamiento
//entonces puedo hacer 2 cosas al mismo tiempo
typedef union{
struct{
    uint8_t seconds[2];
    uint8_t minutes[2];
    uint8_t hours[2];
}time;
uint8_t bcd[6];
}clock_time_t;


typedef struct clock_s *clock_t;


/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */
/**
 * @brief funcion que valida el tiempo que se setea del reloj 
 * 
 * @param reloj estructura de reloj
 * @param time tiempo seteado del MODO_SET_HORA
 */
void validate_time(clock_t reloj, clock_time_t *time);

/**
 * @brief funcion que crea el reloj.
 * @param clock puntero al reloj a crear.
 * @return un puntero al reloj creado.
 */
clock_t clock_create(uint16_t ticks_per_second);

/**
 * @brief función que obtiene el tiempo actual del reloj.
 * 
 * @param clock punter al reloj.
 * @param result puntero al resultado donde se guardará el tiempo actual.
 * @return true 
 * @return false 
 */
bool clock_get_time(clock_t clock, clock_time_t *result);

/**
 * @brief función que verifica si el tiempo del reloj es válido.
 * 
 * @param clock punter al reloj.
 * @return true 
 * @return false 
 */
bool clock_time_is_valid(clock_t clock);

/**
 * @brief función que ajusta el tiempo del reloj.
 * 
 * @param clock punter al reloj.
 * @param new_time puntero al nuevo tiempo a establecer.
 * @return true 
 * @return false 
 */
bool clock_set_time(clock_t clock, const clock_time_t *new_time);

/**
 * @brief función que simula el avance del reloj en segundos.
 * 
 * @param clock punter al reloj.
 */
void clock_new_tick(clock_t clock);

/**
 * @brief función que establece la hora de la alarma.
 * 
 * @param clock punter al reloj.
 * @param alarm_time punter a la hora de la alarma a establecer.
 * @return true 
 * @return false 
 */
bool clock_set_alarm_time(clock_t clock, const clock_time_t *alarm_time);

/**
 * @brief función que obtiene la hora de la alarma.
 * 
 * @param clock 
 * @param alarm_time 
 * @return true 
 * @return false 
 */
bool clock_get_alarm_time(clock_t clock, clock_time_t *alarm_time);


/**
 * @brief función que habilita la alarma.
 * 
 * @param clock 
 */
void clock_enable_alarm(clock_t clock);

// Deshabilita la alarma
/**
 * @brief función que deshabilita la alarma.
 * 
 * @param clock 
 */
void clock_disable_alarm(clock_t clock);

/**
 * @brief función que indica si la alarma está habilitada.
 * 
 * @param clock 
 * @return true 
 * @return false 
 */
// Indica si la alarma está habilitada
bool clock_is_alarm_enabled(clock_t clock);

// Indica si la alarma fue activada
/**
 * @brief función que indica si la alarma fue activada.
 * 
 * @param clock 
 * @return true 
 * @return false 
 */
bool clock_alarm_triggered(clock_t clock);

// Pospone la alarma una cantidad de minutos
/**
 * @brief función que pospone la alarma una cantidad de minutos.
 * 
 * @param clock 
 * @param minutes 
 */
void clock_snooze_alarm(clock_t clock, uint8_t minutes);

/**
 * @brief Muestra la hora actual del reloj en la pantalla de 7 segmentos.
 * 
 * @param clock Puntero al objeto reloj.
 * @param screen Puntero al objeto de pantalla.
 */
void clock_display_time(clock_t clock, screen_t screen);




/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* CLOCK_H */