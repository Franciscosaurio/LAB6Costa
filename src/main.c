/* Copyright 2025, Costa Francisco Lucas Sebastian
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** \brief Simple sample of use LPC HAL gpio functions
 **
 ** \addtogroup samples Sample projects
 ** \brief Sample projects to use as a starting point
 ** @{ */

/* === Headers files inclusions =============================================================== */

#include <stdbool.h>
#include <stdint.h>
#include "bsp.h"
#include "clock.h"
#include "screen.h"
#include "poncho.h"
#include "events.h"
/* === Macros definitions ====================================================================== */
#define TICKS_PER_SECOND 5
/* === Private data type declarations ========================================================== */

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

/* === Public function implementation ========================================================= */

int main(void) {
    // Inicialización de hardware
    modo_t modo=modo_create();
    key_t key=key_create();
    board_t board = board_create();
    clock_t reloj = clock_create(TICKS_PER_SECOND); // Crea el reloj con una estimación de 5 actualizaciones por segundo
    clock_time_t time={
        .time={
            .seconds={0,0},//Su,Sd
            .minutes={0,0},//Mu,Md
            .hours={0,0},//Hu,Hd
        }
    }; // Inicializa la estructura de tiempo
    // Valores a mostrar
    
    int divisor = 0;
    uint8_t inact=0;
    while (true) {
    divisor++;

    uint8_t hora[4] = { time.bcd[2], time.bcd[3], time.bcd[4], time.bcd[5] };
    uint8_t hora_prueba[4] = { 1,2,3,4 };
    if (divisor == 5) {
        divisor = 0;
        inact++;

        switch (modo)
        {
        case MODO_INVALIDO:
            display_flash_digits(board->screen, 0, 3, 50);
            break;
        case MODO_SET_MINUTO:
            display_flash_digits(board->screen, 0, 1, 50);
        break;
        case MODO_SET_HORA:
            display_flash_digits(board->screen, 2, 3, 50);
        break;
        case MODO_SET_ALARMA_HORA:
        break;
        case MODO_SET_ALARMA_MINUTO:
        break;
        case MODO_NORMAL:
            screen_add_point(board->screen, 2);
            screen_write_BCD(board->screen, hora_prueba, 4);
            break;
        break;
        }
        //get_mode(digital_was_activated(board->set_time), &modo, key, reloj, &time);
    }
    if(digital_was_activated(board->cancel) || inact > 30){
        inact = 0;
        if(clock_time_is_valid(reloj)){
            modo=MODO_NORMAL;
        }else{
            modo=MODO_INVALIDO;
        }
    }
    if(digital_was_activated(board->set_time)||modo==MODO_SET_MINUTO){
        modo=MODO_SET_MINUTO;
        inact=0;
        screen_write_BCD(board->screen, hora, 4);
        screen_add_point(board->screen, 2);

        if (digital_was_activated(board->accept)) {
            inact = 0;
            modo = MODO_SET_HORA;
        }

        if (digital_was_activated(board->increment)) {
            inact = 0;
            time_increments(&time, modo);
        }

        if (digital_was_activated(board->decrement)) {
            inact = 0;
            time_decrement(&time, modo);
        }
    }
    if(modo==MODO_SET_HORA&&modo!=MODO_NORMAL){
        screen_write_BCD(board->screen, hora, 4);
        screen_add_point(board->screen, 2);

        if (digital_was_activated(board->accept)) {
            inact = 0;
            validate_time(reloj, &time);
            
            if (clock_time_is_valid(reloj)) {
                modo = MODO_NORMAL;

            }
        }

        if (digital_was_activated(board->increment)) {
            inact = 0;
            time_increments(&time, modo);
        }

        if (digital_was_activated(board->decrement)) {
            inact = 0;
            time_decrement(&time, modo);
        }
    }
    if(modo==MODO_INVALIDO){
        for (int i = 0; i < 4; i++) hora[i] = 0;
        screen_write_BCD(board->screen, hora, 4);
        screen_add_point(board->screen, 2);

    }
    if(modo==MODO_NORMAL){
        screen_write_BCD(board->screen, hora_prueba, 4);
        screen_add_point(board->screen, 2);
    }
    /*
    if(digital_was_activated(board->cancel)||inact>30){
        inact=0;
        if(clock_time_is_valid(reloj)){
            modo=MODO_NORMAL;
        }else{
            modo=MODO_INVALIDO;
        }
    }
    // Lógica por modo
        switch (modo) {
            case MODO_INVALIDO:
                for (int i = 0; i < 4; i++) hora[i] = 0;
                screen_write_BCD(board->screen, hora, 4);
                screen_add_point(board->screen, 2);
                break;

            case MODO_NORMAL:
                screen_write_BCD(board->screen, hora, 4);
                screen_add_point(board->screen, 2);
                break;

            case MODO_SET_MINUTO:
                screen_write_BCD(board->screen, hora, 4);
                screen_add_point(board->screen, 2);

                if (digital_was_activated(board->accept)) {
                    key->inactivo = 0;
                    modo = MODO_SET_HORA;
                }

                if (digital_was_activated(board->increment)) {
                    key->inactivo = 0;
                    time_increments(&time, modo);
                }

                if (digital_was_activated(board->decrement)) {
                    key->inactivo = 0;
                    time_decrement(&time, modo);
                }
                break;

            case MODO_SET_HORA:
                screen_write_BCD(board->screen, hora, 4);
                screen_add_point(board->screen, 2);

                if (digital_was_activated(board->accept)) {
                    key->inactivo = 0;
                    validate_time(reloj, &time);
                    if (clock_time_is_valid(reloj)) {
                        modo = MODO_NORMAL;
                    }
                }

                if (digital_was_activated(board->increment)) {
                    key->inactivo = 0;
                    time_increments(&time, modo);
                }

                if (digital_was_activated(board->decrement)) {
                    key->inactivo = 0;
                    time_decrement(&time, modo);
                }
                break;

        }
    */
    // Refresco de display respetando los 2 for
    for (int index = 0; index < 100; index++) {
        for (int delay = 0; delay < 20000; delay++) {
            __asm("NOP");
        }
        screen_refresh(board->screen);
    }
}
}


/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
