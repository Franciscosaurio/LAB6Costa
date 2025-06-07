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
#include "poncho"
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
    screen_driver_t driver;
    uint8_t value(SCREEN_MAX_DIGITS);

}

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
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G               // 9
}
    



/* === Public function definitions ============================================================================== */

screen_t screen_create(uint8_t digits, screen_driver_t driver) {
    screen_t self = malloc(sizeof(struct screen_s));
    if (digits > SCREEN_MAX_DIGITS) {
        digits = SCREEN_MAX_DIGITS
        
    }
    if(self != NULL) {
        self->digits = digits;
        self->driver=driver;
        self->current_digit = 0;
        digits_init();
        segments_init();
        /*Chip_GPIO_Clear_Value(LPC_GPIO_PORT, SEGMENTS_GPIO, SEGMENTS_MASK);

        Chip_SCU_PinMuxSet(DIGIT_1_PORT, DIGIT_1_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_1_FUNC);
        Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, DIGIT_1_PIN, DIGIT_1_BIT, true);
        
        Chip_SCU_PinMuxSet(DIGIT_2_PORT, DIGIT_2_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_2_FUNC);
        Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, DIGIT_2_PIN, DIGIT_2_BIT, true);

        Chip_SCU_PinMuxSet(DIGIT_3_PORT, DIGIT_3_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_3_FUNC);
        Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, DIGIT_3_PIN, DIGIT_3_BIT, true);

        Chip_SCU_PinMuxSet(DIGIT_4_PORT, DIGIT_4_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_4_FUNC);
        Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, DIGIT_4_PIN, DIGIT_4_BIT, true);
        */
        //Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_P_GPIO, SEGMENT_P_BIT, false);
        //Chip_GPIO_SetPinState(LPC_GPIO_PORT, DIGITS_GPIO, LED_G_BIT, true);
        //Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, LED_G_GPIO, LED_G_BIT, true);
    }
    return self;
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
    self->driver->digits_turn_off();
    self->current_digit = (self->current_digit + 1) % self->digits; // Incrementa el digito actual y lo limita al numero de digitos
    self->driver->segments_update(self->value[self->current_digit]); // Enciende los segmentos correspondientes al digito actual
    self->driver->digit_turn_on(self->current_digit); // Enciende el digito actual
}

/* === End of documentation ======================================================================================== */
