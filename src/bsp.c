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

/** @file bsp.c
 ** @brief codigo fuente del modulo para soporte de la placa de hardware
 **/

/* === Headers files inclusions ==================================================================================== */

#include "bsp.h"
#include "chip.h"
#include <stdlib.h>
#include <string.h>
#include "ciaa.h"
#include "poncho.h"
#include <stddef.h>
#include "clock.h"
#include "digital.h"
#include "screen.h"
#include "events.h"

/* === Macros definitions ========================================================================================== */

/* === Private data type declarations ============================================================================== */

/* === Private function declarations =============================================================================== */

void digits_turn_off(void);
void segments_update(uint8_t value);
void digit_turn_on(uint8_t digit);

/* === Private variable definitions ================================================================================ */
static const struct screen_driver_s screen_driver = {
    .digits_turn_off = digits_turn_off,
    .segments_update = segments_update,
    .digit_turn_on = digit_turn_on,
};
/* === Public variable definitions ================================================================================= */


/* === Private function definitions ================================================================================ */
// inicializa los 4 digitos del display
void digits_init(void){
    Chip_SCU_PinMuxSet(DIGIT_1_PORT, DIGIT_1_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_1_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, DIGIT_1_GPIO, DIGIT_1_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, DIGIT_1_GPIO, DIGIT_1_BIT, true);

    Chip_SCU_PinMuxSet(DIGIT_2_PORT, DIGIT_2_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_2_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, DIGIT_2_GPIO, DIGIT_2_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, DIGIT_2_GPIO, DIGIT_2_BIT, true);

    Chip_SCU_PinMuxSet(DIGIT_3_PORT, DIGIT_3_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_3_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, DIGIT_3_GPIO, DIGIT_3_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, DIGIT_3_GPIO, DIGIT_3_BIT, true);

    Chip_SCU_PinMuxSet(DIGIT_4_PORT, DIGIT_4_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_4_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, DIGIT_4_GPIO, DIGIT_4_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, DIGIT_4_GPIO, DIGIT_4_BIT, true);


}
// inicio los displays de 7 segmentos
// y el punto decimal
void segments_init(void){ 
    Chip_SCU_PinMuxSet(SEGMENT_A_PORT, SEGMENT_A_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_A_FUNC);
    // inicializo los digitos, siendo SEGMENT_A_PORT el puerto, SEGMENT_A_PIN el pin, y 
    //SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_A_FUNC la funcion
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_A_GPIO, SEGMENT_A_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_A_GPIO, SEGMENT_A_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_B_PORT, SEGMENT_B_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_B_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_B_GPIO, SEGMENT_B_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_B_GPIO, SEGMENT_B_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_C_PORT, SEGMENT_C_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_C_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_C_GPIO, SEGMENT_C_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_C_GPIO, SEGMENT_C_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_D_PORT, SEGMENT_D_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_D_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_D_GPIO, SEGMENT_D_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_D_GPIO, SEGMENT_D_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_E_PORT, SEGMENT_E_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_E_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_E_GPIO, SEGMENT_E_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_E_GPIO, SEGMENT_E_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_F_PORT, SEGMENT_F_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_F_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_F_GPIO, SEGMENT_F_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_F_GPIO, SEGMENT_F_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_G_PORT, SEGMENT_G_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_G_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_G_GPIO, SEGMENT_G_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_G_GPIO, SEGMENT_G_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_P_PORT, SEGMENT_P_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_P_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_P_GPIO, SEGMENT_P_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_P_GPIO, SEGMENT_P_BIT, true);
}
void key_init(void){
    Chip_SCU_PinMuxSet(KEY_F1_PORT, KEY_F1_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | KEY_F1_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, KEY_F1_GPIO, KEY_F1_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, KEY_F1_GPIO, KEY_F1_BIT, false);

    Chip_SCU_PinMuxSet(KEY_F2_PORT, KEY_F2_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | KEY_F2_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, KEY_F2_GPIO, KEY_F2_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, KEY_F2_GPIO, KEY_F2_BIT, false);

    Chip_SCU_PinMuxSet(KEY_F3_PORT, KEY_F3_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | KEY_F3_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, KEY_F3_GPIO, KEY_F3_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, KEY_F3_GPIO, KEY_F3_BIT, false); 

    Chip_SCU_PinMuxSet(KEY_F4_PORT, KEY_F4_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | KEY_F4_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, KEY_F4_GPIO, KEY_F4_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, KEY_F4_GPIO, KEY_F4_BIT, false);

    Chip_SCU_PinMuxSet(KEY_CANCEL_PORT, KEY_CANCEL_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | KEY_CANCEL_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, KEY_CANCEL_GPIO, KEY_CANCEL_BIT, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, KEY_CANCEL_GPIO, KEY_CANCEL_BIT, false);

    Chip_SCU_PinMuxSet(KEY_ACCEPT_PORT, KEY_ACCEPT_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | KEY_ACCEPT_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, KEY_ACCEPT_GPIO, KEY_ACCEPT_BIT,false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, KEY_ACCEPT_GPIO, KEY_ACCEPT_BIT, false);
}
void digits_turn_off(void) {
    Chip_GPIO_ClearValue(LPC_GPIO_PORT, DIGITS_GPIO, DIGITS_MASK);
    /*
    Chip_GPIO_ClearValue(LPC_GPIO_PORT, DIGIT_1_GPIO, DIGIT_1_BIT);
    Chip_GPIO_ClearValue(LPC_GPIO_PORT, SEGMENTS_GPIO, SEGMENTS_MASK);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_P_GPIO,SEGMENT_P_BIT, false);
    */
}
void segments_update(uint8_t value) {
    Chip_GPIO_ClearValue(LPC_GPIO_PORT, SEGMENTS_GPIO, SEGMENTS_MASK);  // Limpia todos los segmentos
    Chip_GPIO_SetValue(LPC_GPIO_PORT, SEGMENTS_GPIO, value & SEGMENTS_MASK);  // Enciende los necesarios
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_P_GPIO, SEGMENT_P_BIT, value & SEGMENT_P);  // Punto decimal
/*
    //esta fucion actualiza los segmentos del display de la siguiente manera:
    // 1. Limpia los segmentos
    Chip_GPIO_SetValue(LPC_GPIO_PORT, SEGMENTS_GPIO, (value&SEGMENTS_MASK));
    // 2. Activa el punto decimal si es necesario
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_P_GPIO, SEGMENT_P_BIT, (value & SEGMENT_P));*/
}    
void digit_turn_on(uint8_t digit) {
    Chip_GPIO_SetValue(LPC_GPIO_PORT, DIGITS_GPIO, (1<<(digit)&DIGITS_MASK));
    // esta funcion setea el valor del gpio de los digitos
    // el 3-digit es para que el digito 1 sea el bit 0
}
void buzz_turn_on(void) {
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, BUZZER_GPIO, BUZZER_BIT, true);
}
void buzz_turn_off(void) {
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, BUZZER_GPIO, BUZZER_BIT, false);
}

//********************* */
/* === Public function implementation ============================================================================== */
board_t board_create(void) {
    struct board_s * board = malloc(sizeof(struct board_s));
    // esta linea de codigo lo que hace es reservar memoria para la estructura board_s
    if (board!= NULL) {
        //si la memoria es distinta de 0 entonces inicializo los digitos y los segmentos
        digits_init();
        segments_init();
        board->screen = screen_create(4, &screen_driver);// a board
        key_init();
        board->set_time = digital_input_create(KEY_F1_GPIO, KEY_F1_BIT, false);
        board->set_alarm = digital_input_create(KEY_F2_GPIO, KEY_F2_BIT, false);
        board->decrement = digital_input_create(KEY_F3_GPIO, KEY_F3_BIT, false);
        board->increment = digital_input_create(KEY_F4_GPIO, KEY_F4_BIT, false);
        board->accept = digital_input_create(KEY_ACCEPT_GPIO, KEY_ACCEPT_BIT, false);
        board->cancel = digital_input_create(KEY_CANCEL_GPIO, KEY_CANCEL_BIT, false);
        
        board->buzzer=digital_output_create(BUZZER_GPIO, BUZZER_BIT);
        // inicializo el buzzer
    }
    return board;
}
/* === End of documentation ======================================================================================== */