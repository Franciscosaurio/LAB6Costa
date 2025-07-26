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

/** @file screen.c
 ** @brief codigo fuente del modulo para soporte de la placa de hardware
 **/

/* === Headers files inclusions ==================================================================================== */

#include "screen.h"
#include "poncho.h"
#include<string.h>
#include <stdlib.h>

/* === Macros definitions ========================================================================================== */

#ifndef SCREEN_MAX_DIGITS
#define SCREEN_MAX_DIGITS 8
#endif


/* === Private data type declarations ============================================================================== */

struct screen_s{
    uint8_t digits;//!< número de dígitos del display>
    uint8_t current_digit;//!< dígito actual que se está mostrando>
    uint8_t flashing_from;//!< dígito desde el cual se inicia el parpadeo>
    uint8_t flashing_to;//!< dígito hasta el cual se inicia el parpadeo>
    uint8_t flashing_count;//!< contador de parpadeo>
    uint16_t flashing_frequency;//!< frecuencia de parpadeo en ticks>
    screen_driver_t driver;//!< puntero al driver de pantalla>
    uint8_t value[SCREEN_MAX_DIGITS];//!< valor a mostrar en el display>

};

/* === Private function declarations =============================================================================== */

/* === Private variable definitions ================================================================================ */

/* === Public variable definitions ================================================================================= */

/* === Private function definitions ================================================================================ */
// array constante donde esta la traduccion de cada uno de los numeros a los segmentos del display
// cada numero tiene su representacion en segmentos
static const uint8_t IMAGES[10] = {
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F,          // 0
    SEGMENT_B | SEGMENT_C,                                                          // 1
    SEGMENT_A | SEGMENT_B | SEGMENT_D | SEGMENT_E | SEGMENT_G,                           // 2
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_G,                              // 3
    SEGMENT_B | SEGMENT_C | SEGMENT_F | SEGMENT_G,                                         // 4
    SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G,                            // 5
    SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G,                // 6
    SEGMENT_A | SEGMENT_B | SEGMENT_C,                                                   // 7 
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G,  // 8
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G  ,             // 9
};
    



/* === Public function definitions ============================================================================== */

screen_t screen_create(uint8_t digits, screen_driver_t driver) {
    screen_t self = malloc(sizeof(struct screen_s));
    //esta linea de codigo lo que hace es reservar memoria para la estructura screen_s
    // si no se puede reservar memoria, self es NULL
    if (digits > SCREEN_MAX_DIGITS) {
        digits = SCREEN_MAX_DIGITS;
        
    }
    
    if(self!=NULL){
        self->digits= digits;
        //esta linea de codigo lo que hace es asignar el numero de digitos a la estructura
        self->driver = driver;
        //esta linea de codigo lo que hace es asignar el driver a la estructura
        self->current_digit = 0;
        //esta linea de codigo lo que hace es asignar el digito actual a 0
        self->flashing_count = 0;
        // esta linea de codigo lo que hace es asignar el contador de parpadeo a 0
        self->flashing_frequency = 0;
        //no importa lo que digan el from y el to, si la frecuencia es 0 no parpadea
    }
    return self;
}

void screen_write_BCD(screen_t self, uint8_t value[], uint8_t size){
    memset(self->value, 0, sizeof(self->value)); 
    // Limpio el buffer de valores
    // memset es una funcion  de la stdlib para inicializar arrays

    if(size > self->digits){
        size = self->digits;
        //esta linea de codigo lo que hace es limitar el tamaño del array al numero de digitos
    }
    for(uint8_t i=0; i<size; i++){
        self->value[i]=IMAGES[value[i]];
        //esta linea de codigo lo que hace es asignar el valor de la imagen al array de valores
        // value[i] es el valor que quiero mostrar en el digito i
        // cada valor de sd lo uso para entrar a la memoria de imagenes
    }
}

void screen_refresh(screen_t self){
    uint8_t segments;
    self->driver->digits_turn_off();// Apaga los dígitos antes de actualizar
    self->current_digit = (self->current_digit + 1) % self->digits;
    // Incrementa el digito actual y lo limita al numero de digitos
    segments = self->value[self->current_digit]; 
    // Obtiene los segmentos correspondientes al digito actual
    //copio lo que deberia ser la imagen 
    if(self->flashing_frequency !=0){
        if(self->current_digit ==0){
            self->flashing_count=(self->flashing_count + 1) %(self->flashing_frequency);
            //forma implicita de volver a 0 el contador
        }
        if(self->flashing_count >= self->flashing_frequency/2){
            if(self->current_digit >= self->flashing_from && self->current_digit <= self->flashing_to) {
                segments = 0; // Si el contador de parpadeo es mayor o igual a la mitad de la frecuencia, apaga los segmentos
            
            }
            // Si el contador de parpadeo es mayor o igual a la mitad de la frecuencia, apaga los segmentos
        }
    }
    self->driver->segments_update(segments); 
    // Enciende los segmentos correspondientes al digito actual
    self->driver->digit_turn_on(self->current_digit); 
    // Enciende el digito actual

}

void display_flash_digits(screen_t self, uint8_t from, uint8_t to, uint16_t divisor) {
    int result;
    if (from > to || from >= SCREEN_MAX_DIGITS || to >= SCREEN_MAX_DIGITS) {
        result=1; // Error: los indices estan fuera de rango
    }else if(!self){
        result=-1; // Error: from es mayor que to
    } else{
        //lo que hace este else es asignar los valores de parpadeo a la estructura
        self->flashing_from = from;// Asigna el valor de inicio del parpadeo a la estructura
        self->flashing_to = to;// Asigna los valores de parpadeo a la estructura
        self->flashing_frequency = 2*divisor; // La frecuencia de parpadeo se establece como el doble del divisor
        self->flashing_count = 0;// Reinicia el contador de parpadeo
    }
    
}
void screen_add_point(screen_t screen, uint8_t digit){
    if (digit < screen->digits) {
        screen->value[digit] |= SEGMENT_P; // Agrega el punto al dígito especificado
        // SEGMENT_P es una constante que representa el segmento del punto decimal
    }
    // Esta funcion agrega un punto decimal al digito especificado
    // si el digito es mayor que la cantidad de digitos, no hace nada
    // si el digito es menor que la cantidad de digitos, agrega el punto al dígito especificado
}
/* === End of documentation ======================================================================================== */
