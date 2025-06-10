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
    uint8_t digits;
    uint8_t current_digit;
    uint8_t flashing_from;
    uint8_t flashing_to;
    uint8_t flashing_count;
    uint16_t flashing_frequency;
    screen_driver_t driver;
    uint8_t value[SCREEN_MAX_DIGITS];

};

/* === Private function declarations =============================================================================== */

/* === Private variable definitions ================================================================================ */

/* === Public variable definitions ================================================================================= */

/* === Private function definitions ================================================================================ */
// array constante donde esta la traduccion de cada uno
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
    if (digits > SCREEN_MAX_DIGITS) {
        digits = SCREEN_MAX_DIGITS;
        
    }
    
    if(self!=NULL){
        self->digits= digits;
        self->driver = driver;
        self->current_digit = 0;
        self->flashing_count = 0;
        
        self->flashing_frequency = 0;//no importa lo que digan el from y el to, si la frecuencia es 0 no parpadea

    }
    //return self;
    //se elimina en el video 24, consultar
   /* if(self != NULL) {
        self->digits = digits;
        self->driver=driver;
        self->current_digit = 0;
        digits_init();// declararla como funcion de callback, como el del driver, se debe ecargar el bsp
        segments_init();
    }
    */return self;
}

void screen_write_BCD(screen_t self, uint8_t value[], uint8_t size){
    memset(self->value, 0, sizeof(self->value)); 
    // Limpio el buffer de valores
    // memset es una funcion  de la stdlib para inicializar arrays

    if(size > self->digits){
        size = self->digits;
    }
    for(uint8_t i=0; i<size; i++){
        self->value[i]=IMAGES[value[i]];// cada valor de sd lo uso para entrar a la memoria de imagenes
    }
}

void screen_refresh(screen_t self){
    uint8_t segments;
    self->driver->digits_turn_off();
    self->current_digit = (self->current_digit + 1) % self->digits; // Incrementa el digito actual y lo limita al numero de digitos
    //
    segments = self->value[self->current_digit]; // Obtiene los segmentos correspondientes al digito actual
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
    self->driver->segments_update(segments); // Enciende los segmentos correspondientes al digito actual
    self->driver->digit_turn_on(self->current_digit); // Enciende el digito actual

}

void display_flash_digits(screen_t self, uint8_t from, uint8_t to, uint16_t divisor) {
    int result;
    if (from > to || from >= SCREEN_MAX_DIGITS || to >= SCREEN_MAX_DIGITS) {
        result=1; // Error: los indices estan fuera de rango
    }else if(!self){
        result=-1; // Error: from es mayor que to
    } else{
        self->flashing_from = from;
        self->flashing_to = to;
        self->flashing_frequency = 2*divisor; // La frecuencia de parpadeo se establece como el doble del divisor
        self->flashing_count = 0;
    }
    
}

/* === End of documentation ======================================================================================== */
