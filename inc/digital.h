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

#ifndef DIGITAL_H_
#define DIGITAL_H_

/** @file digital.h
 ** @brief declaraciones del modulo para gestion de entradas y salidas digitales
 **/

/* === Headers files inclusions ==================================================================================== */

#include <stdint.h>
#include <stdbool.h>

/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */

/* === Public data type declarations =============================================================================== */

typedef enum digital_states_e{
    DIGITAL_INPUT_WAS_DEACTIVATE = -1,
    DIGITAL_INPUT_NO_CHANGE = 0,
    DIGITAL_INPUT_WAS_ACTIVATED = 1,

} digital_states_t;

// declaro el tipo de datos que va a gestionar la salida
//! estructura que representa una salida digital
typedef struct digital_output_s * digital_output_t;

typedef struct digital_input_s * digital_input_t;
/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */

/**
 * @brief Funcion para crear una salida digital
 * la funcion crea un objeto de la clase salida digital
 * la locacion de memoria depende de la 
 * @ref configuracioin de la biblioteca "config"
 * @param gpio 
 * @param bit
 * @return digital_output_t 
 */

digital_output_t digital_output_create(uint8_t gpio, uint8_t bit);
//este micro necesita 5 cosas para configurar un pin
//pero necesita 2 para funcionar
//las otras 3 van a la funcion main en la parte del while

/**
 * @brief 
 * 
 * @param self 
 */
void digital_output_activate(digital_output_t self);

/**
 * @brief 
 * 
 * @param self 
 */

void digital_output_deactivate(digital_output_t self);

/**
 * @brief 
 * 
 * @param self 
 */

void digital_output_toggle(digital_output_t self);

/**
 * @brief 
 * 
 * @param gpio 
 * @param bit 
 * @param inverted 
 * @return digital_input_t 
 */

digital_input_t digital_input_create(uint8_t gpio, uint8_t bit, bool inverted);

/**
 * @brief 
 * 
 * @param input 
 * @return true 
 * @return false 
 */


bool digital_input_get_is_active(digital_input_t input);

/**
 * @brief 
 * 
 * @param input 
 * @return true 
 * @return false 
 */

bool digital_was_activated(digital_input_t input);

/**
 * @brief 
 * 
 * @param input 
 * @return true 
 * @return false 
 */

bool digital_was_deactivated(digital_input_t input);

/**
 * @brief 
 * 
 * @param input 
 * @return digital_states_t 
 */

digital_states_t digital_was_changed(digital_input_t input);
/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* DIGITAL_H_ */
