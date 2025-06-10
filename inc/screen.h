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

#ifndef SCREEN_H_
#define SCREEN_H_

/** @file def.h
 ** @brief declaraciones del modulo para la gestion de una pantalla multiplexada de 7 segmentos
 **/

/* === Headers files inclusions ==================================================================================== */
#include <stdint.h>
#include <stdbool.h>
#include "screen.h"
#include "cia.h"
/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */
#define SEGMENT_A (1<<0)
#define SEGMENT_B (1<<1)
#define SEGMENT_C (1<<2)
#define SEGMENT_D (1<<3)
#define SEGMENT_E (1<<4)
#define SEGMENT_F (1<<5)
#define SEGMENT_G (1<<6)
#define SEGMENT_P (1<<7)
/* === Public data type declarations =============================================================================== */

typedef void(*digits_turn_off_t)(void);

typedef void(*segments_update_t)(uint8_t);

typedef void(*digit_turn_on_t)(uint8_t);

typedef struct screen_driver_s {
    digits_turn_off_t digits_turn_off;
    segments_update_t segments_update;
    digit_turn_on_t digit_turn_on;
}const * screen_driver_t;



/**
 * @brief 
 * 
 */

typedef struct screen_s * screen_t;
/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */

screen_t screen_create(uint8_t digits, screen_driver_t driver);


void screen_write_BCD(screen_t screen, uint8_t value[], uint8_t size);

void screen_refresh(screen_t screen);

/**
 * @brief funcion para hacer parpadear algunos digitos de la pantalla
 * 
 * @param display puntero al despriptor de la pantalla con la que se quiere operar
 * @param from posicion del primer digito que se quiere hacer parpadear
 * @param to    posicion del ultimo digito que se quiere hacer parpadear
 * @param frecuency factor de division de la frecuencia de refresco para el parpadeo
 */
void display_flash_digits(screen_t screen, uint8_t from, uint8_t to, uint16_t divisor);
// la cantidad de veces 

/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* SCREEN_H_ */
