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
    modo_t modo = modo_create();
    board_t board = board_create();
    clock_t reloj = clock_create(TICKS_PER_SECOND);
    alarma_t alarma = ALARMA_INACTIVA;
    clock_time_t time = {
        .time = {
            .seconds = {0, 0}, // Su, Sd
            .minutes = {0, 0}, // Mu, Md
            .hours   = {0, 0}, // Hu, Hd
        }
    };
    clock_time_t tiempo_alarma = {
        .time = {
            .seconds = {0, 0}, // Su, Sd
            .minutes = {0, 0}, // Mu, Md
            .hours   = {0, 0}, // Hu, Hd
        }
    };
    bool parpadeo_activo;
    uint8_t inact = 0,cont=0;
    int divisor = 0;

    while (true) {
        clock_new_tick(reloj);// incrementa el tick del reloj
        divisor++;
        uint8_t hora_prueba[4] = {0, 0, 0, 0};
        uint8_t hora[4] = { time.bcd[2], time.bcd[3], time.bcd[4], time.bcd[5] };
        uint8_t hora_alarma[4] = { tiempo_alarma.bcd[2], tiempo_alarma.bcd[3], tiempo_alarma.bcd[4], tiempo_alarma.bcd[5] };
        // Cada segundo, solo activa flag de parpadeo
        if (divisor == 5) {
            divisor = 0;
            inact++;
            cont++;
            parpadeo_activo = true;

        } else {
            parpadeo_activo = false;
        }
        if(digital_was_activated(board->cancel)||inact>30){
            inact=0;
            clock_get_time(reloj, &time);
            if(clock_time_is_valid(reloj)){
                modo=MODO_NORMAL;
            }else{
                modo=MODO_INVALIDO;
            }
        }
        if(digital_was_activated(board->set_alarm)){
            inact=0;
            if(cont>3){
                cont=0;
                modo=MODO_SET_ALARMA_MINUTO;
            }
        }
    
        if(digital_input_get_is_active(board->set_time)){
            inact=0;
            if(cont>3){
                cont=0;
                modo=MODO_SET_MINUTO;
            }
        }
        // Dibujo de pantalla según modo
        switch (modo) {
            case MODO_INVALIDO:
                if (parpadeo_activo) display_flash_digits(board->screen, 0, 3, 50);
                for (int i = 0; i < 4; i++) hora[i] = 0;
                screen_write_BCD(board->screen, hora, 4);
                screen_add_point(board->screen, 2);
                break;

            case MODO_SET_MINUTO:
                if (parpadeo_activo) display_flash_digits(board->screen, 0, 1, 50);
                screen_write_BCD(board->screen, hora, 4);
                screen_add_point(board->screen, 2);
                if(digital_was_activated(board->increment)){
                    inact=0;
                    time_increments(&time, modo);
                }
                if(digital_was_activated(board->decrement)){
                    inact=0;
                    time_decrement(&time, modo);
                }
                if(digital_was_activated(board->accept)){
                    inact=0;
                    modo = MODO_SET_HORA;
                }
                
                break;

            case MODO_SET_HORA:
                if (parpadeo_activo) display_flash_digits(board->screen, 2, 3, 50);
                screen_write_BCD(board->screen, hora, 4);
                screen_add_point(board->screen, 2);
                if(digital_was_activated(board->increment)){
                    inact=0;
                    time_increments(&time, modo);
                }
                if(digital_was_activated(board->decrement)){
                    inact=0;
                    time_decrement(&time, modo);
                }
                if(digital_was_activated(board->accept)){
                    inact=0;
                    modo = MODO_NORMAL;
                    clock_set_time(reloj, &time);
                }
                break;

            case MODO_NORMAL:

                display_flash_digits(board->screen, 0, 0, 0);
                clock_get_time(reloj, &time);
                hora[0] = time.bcd[2];
                hora[1] = time.bcd[3];
                hora[2] = time.bcd[4];
                hora[3] = time.bcd[5];
                screen_write_BCD(board->screen, hora, 4);
                screen_add_point(board->screen, 2);
                if(digital_was_activated(board->set_alarm)){
                    inact=0;
                    if(cont>3){
                        cont=0;
                        modo=MODO_SET_ALARMA_MINUTO;
                    }
                }
                //*****************
                
            break;
            case MODO_SET_ALARMA_MINUTO:
                if (parpadeo_activo) display_flash_digits(board->screen, 0, 1, 50);
                for(int i; i < 4; i++) screen_add_point(board->screen, i);
                screen_write_BCD(board->screen, hora_prueba, 4);
            break;
            case MODO_SET_ALARMA_HORA:
                
                
            break;
        }
        
        for (int index = 0; index < 100; index++) {
            for (int delay = 0; delay < 2000; delay++) {//cambiar a 20K el delay que está en 2K
                __asm("NOP");
            }
            screen_refresh(board->screen);
        }
    }
}



/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
