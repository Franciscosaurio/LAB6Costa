/*********************************************************************************************************************
Copyright (c) 2025, Costa_Francisco Lucas Sebastian

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

/** @file digital.c
 ** @brief Código fuente del modulo para gestion de entradas y salidas digitales
 **/

/* === Headers files inclusions ==================================================================================== */
#include "chip.h"
#include "digital.h"
#include "config.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
/* === Macros definitions ========================================================================================== */


/* === Private data type declarations ============================================================================== */

/*! estructura que representa una salida digital*/

struct digital_output_s{
    uint8_t port; /*!< puerto al que pertenece la salida*/
    uint8_t pin; /*!< pin al que pertenece la salida*/
    

};

// el fabricante me da las cosas para hacer el toggle
//si no me las da lo mismo le puedo hacer un toggle
// tenemos las funciones para hacer set, clear y toggle a nivel fisico



/*! estructura que representa una entrada digital*/

struct digital_input_s{
    uint8_t port; /*!< puerto al que pertenece la entrada*/
    uint8_t pin; /*!< pin al que pertenece la entrada*/
    bool inverted; /*!< estado de la entrada*/
    bool last_state;

};

/* === Private function declarations =============================================================================== */



/* === Private variable definitions ================================================================================ */

/* === Public variable definitions ================================================================================= */

/* === Private function definitions ================================================================================ */


/* === Public function definitions ============================================================================== */

digital_output_t digital_output_create(uint8_t port, uint8_t pin){
    digital_output_t self = malloc(sizeof(struct digital_output_s));
    if (self != NULL){
        self->port = port;
        self->pin=pin;

    }
    return self;
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, self->port, self->pin, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, self->port, self->pin, true);
}
//implemento una funcion implementando malloc,si le devolvio algo entonces recien asigna el puerto y pin
//que recibio a la estructura
//hay que configurar la capa fisica

//sigo creando funciones vacias y despues transportamos las funciones del main adentro
//de las funciones vacias y veo que siga funcionando

void digital_output_activate(digital_output_t self) {
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, self->port, self->pin, true);
}

void digital_output_deactivate(digital_output_t self) {
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, self->port, self->pin, false);
}

void digital_output_toggle(digital_output_t self){
    Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, self->port, self->pin);    
}

digital_input_t digital_input_create(uint8_t gpio, uint8_t bit, bool inverted){
    digital_input_t self = malloc(sizeof(struct digital_input_s));
    if (self != NULL){
        self->port = gpio;
        self->pin = bit;
        self->inverted = inverted;
        //inicializamos la variable
        self->last_state=false;// digital_input_get_is_active(self); <- ¿cuando usar?// consultar
        
        Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, gpio, bit, false);        

    }
    return self;
}

bool digital_input_get_is_active(digital_input_t self) {
    bool state = !Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, self->port, self->pin);
    return self->inverted ? !state : state;
}

digital_states_t digital_was_changed(digital_input_t self){
    digital_states_t result = DIGITAL_INPUT_NO_CHANGE;
     
    //debo leer el estado actual
    // lo hago con mi funcion

    bool state = digital_input_get_is_active(self);
    if (state && !self->last_state){
        result = DIGITAL_INPUT_WAS_ACTIVATED;

    }else if(!state && self->last_state){
        result=DIGITAL_INPUT_WAS_DEACTIVATE;
    }
    self->last_state=state;
    return result;
}

bool digital_was_activated(digital_input_t self) {
    return (digital_was_changed(self) == DIGITAL_INPUT_WAS_ACTIVATED);
}

bool digital_was_deactivated(digital_input_t self) {
    return (digital_was_changed(self) == DIGITAL_INPUT_WAS_DEACTIVATE);
}

/* === End of documentation ======================================================================================== */


