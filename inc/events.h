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

#ifndef EVENTS_H_
#define EVENTS_H_

/** @file plantilla.h
 ** @brief Plantilla para la creación de archivos de de cabeceras en lenguaje C
 **/

/* === Headers files inclusions ==================================================================================== */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "clock.h"
/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */

typedef enum {
    MODO_INVALIDO,
    MODO_NORMAL,
    MODO_SET_HORA,
    MODO_SET_MINUTO,
    MODO_SET_ALARMA_HORA,
    MODO_SET_ALARMA_MINUTO,
} modo_t;

struct key_s {
    bool normal;
    bool long_press;
    bool press;
    uint8_t contador;
    uint8_t inactivo;
};
typedef struct key_s * key_t;

/* === Public data type declarations =============================================================================== */

/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */
/**
 * @brief 
 * 
 * @return key_t 
 */
key_t key_create(void);
/**
 * @brief 
 * 
 * @return modo_t 
 */
modo_t modo_create(void);
/**
 * @brief función que incrementa el tiempo en formato BCD.
 * 
 * @param time la hora de reloj a incrementar.
 * @param modo la estructura que contiene el modo actual del reloj.
 */
void time_increments(clock_time_t *time, modo_t modo);

void time_decrement(clock_time_t *time, modo_t modo);

/**
 * @brief Set the time object
 * 
 * @param tecla 
 * @param modo 
 * @param key 
 * @param reloj 
 * @param time 
 */
void set_time(bool tecla, modo_t *modo, key_t key, clock_t reloj, clock_time_t *time);
/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* EVENTS_H_ */