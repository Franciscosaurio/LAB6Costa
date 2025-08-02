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
  /*  while (true) {
        // Actualizar estado de teclas
        divisor++;
        uint8_t time[4] = {time.bcd[5], time.bcd[4], time.bcd[3], time.bcd[2]};
        uint8_t time_invalida[4] = {0};
        if(divisor==5){
            divisor=0;
            
            //si la time no es valida muestor 00.00 y parpadean todos los digitos

            //si se presiona la tecla F1 se activa el modo de ajuste de time
            set_time(digital_input_get_is_active(board->set_time),&modo,key,reloj, &time);//cambiar a set_states
            //entro al seteo de tiempo y si está inactivo por mas de 30 segundos se sale
        }
        if(modo==MODO_NORMAL){
                screen_write_BCD(board->screen, time, 4);
                screen_add_point(board->screen, 2); // Punto entre times y minutos                
            }
            if(modo==MODO_INVALIDO){
                screen_write_BCD(board->screen, time_invalida, 4);
                screen_add_point(board->screen, 2); // Punto entre times y minutos
                display_flash_digits(board->screen, 0, 3, 10); // Parpadean todos los dígitos
            }
            if(modo==MODO_SET_MINUTO){
                if(digital_input_get_is_active(board->set_time)){
                    screen_write_BCD(board->screen, time, 4);
                    screen_add_point(board->screen, 2); // Punto entre times y minutos
                    display_flash_digits(board->screen, 0, 1, 10);
                }
                
                if(digital_input_get_is_active(board->increment)){
                    key->inactivo = 0; // Reinicia el contador de inactividad
                    time_increments(&time, modo);
                }
                key->inactivo = 0; // Reinicia el contador de inactividad
                time_decrement(&time, modo);
                    if(digital_input_get_is_active(board->accept)){
                        modo=MODO_SET_time; // Cambia al modo de ajuste de time
                        key->inactivo = 0; // Reinicia el contador de inactividad
                    }
                    if(digital_input_get_is_active(board->cancel)){
                        if(!clock_time_is_valid(reloj)){
                            modo=MODO_INVALIDO;
                        }else{
                            modo=MODO_NORMAL; // Sale del modo de ajuste de time
                        }
                    }
            }
            if(modo==MODO_SET_time){
                screen_write_BCD(board->screen, time, 4);
                screen_add_point(board->screen, 2); // Punto entre times y minutos
                display_flash_digits(board->screen, 2, 3, 50); // Parpadean las times
                if(digital_input_get_is_active(board->increment)){
                    key->inactivo = 0; // Reinicia el contador de inactividad
                    time_increments(&time, modo);
                }
                if(digital_input_get_is_active(board->decrement)){
                    key->inactivo = 0; // Reinicia el contador de inactividad
                    time_decrement(&time, modo);                
                }
                if(digital_input_get_is_active(board->accept)){
                    clock_set_time(reloj, &time);
                    modo = MODO_NORMAL; // Sale del modo de ajuste de time
                    key->inactivo = 0; // Reinicia el contador de inactividad

                }
            } 
        for (int index = 0; index < 100; index++) {
            for (int delay = 0; delay < 25000; delay++) {
                __asm("NOP");
            }
            screen_refresh(board->screen);
        }
    }
*/
    while (true) {
    divisor++;

    uint8_t hora[4] = { time.bcd[2], time.bcd[3], time.bcd[4], time.bcd[5] };

    if (divisor == 5) {
        divisor = 0;
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

        }
        get_mode(digital_was_activated(board->set_time), &modo, key, reloj, &time);
    }
    if (modo == MODO_INVALIDO) {
            screen_write_BCD(board->screen, hora, 4);
            screen_add_point(board->screen, 2);
            
        }
        if (modo == MODO_NORMAL) {
            screen_write_BCD(board->screen, hora, 4);
            screen_add_point(board->screen, 2);
        }
    if(modo == MODO_SET_MINUTO){
        screen_write_BCD(board->screen, hora, 4);
        screen_add_point(board->screen, 2);
        
        if(digital_input_get_is_active(board->accept)){
            key->inactivo=0;
            modo=MODO_SET_HORA;
        }
        if(digital_input_get_is_active(board->increment)){
            key->inactivo=0;
            time_increments(&time,modo);
        }
        if(digital_input_get_is_active(board->decrement)){
            key->inactivo=0;
            time_decrement(&time,modo);
        }
        
    }
    if(digital_input_get_is_active(board->cancel)){
        if(clock_time_is_valid(reloj)){
            modo=MODO_NORMAL;
        }else{
            modo=MODO_INVALIDO;
        }
    }
    if(modo==MODO_SET_HORA){
        screen_write_BCD(board->screen, hora, 4);
        screen_add_point(board->screen, 2);
        if(digital_input_get_is_active(board->increment)){
            key->inactivo=0;
            time_increments(&time,modo);
        }
        if(digital_input_get_is_active(board->decrement)){
            key->inactivo=0;
            time_decrement(&time,modo);
        }
        if(digital_input_get_is_active(board->accept)){
            key->inactivo=0;
            validate_time(reloj,&time);
            if(clock_time_is_valid(reloj)){
                modo=MODO_NORMAL;
            }
        }
        
    }
    
    // Refresco de display respetando los 2 for
    for (int index = 0; index < 100; index++) {
        for (int delay = 0; delay < 25000; delay++) {
            __asm("NOP");
        }
        screen_refresh(board->screen);
    }
}
}


/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
