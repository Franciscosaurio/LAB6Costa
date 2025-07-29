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
#include "screen.h"
/* === Macros definitions ====================================================================== */
#define TICKS_PER_SECOND 5
/* === Private data type declarations ========================================================== */

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

/* === Public function implementation ========================================================= */

/*int main(void) {

    int divisor = 0;

    // Inicialización de la placa y obtención de los periféricos
    board_t board = board_create();

    // Crea el reloj con una estimación de 5 actualizaciones por segundo
    clock_t reloj = clock_create(TICKS_PER_SECOND); 
   digital_input_t F1 = digital_input_create(5,12,false);
   
   digital_input_t F2 = digital_input_create(5,13,false);
   //digital_input_t F3 = digital_input_create(5,14,false);
   //digital_input_t F4 = digital_input_create(5,15,false);
   //digital_input_t CANCEL = digital_input_create(5,8,false);
   //digital_input_t ACCEPT = digital_input_create(5,9,false);
   
   bool parpadeo, F1C=true;
   bool en_ajuste=false;
   //bool chochox=false;
   uint8_t control;
    while (true) {
        divisor++;
        if (divisor == 5) {
            divisor = 0;
            // Obtener hora actual
            clock_time_t tiempo={0};// Inicializar la estructura de tiempo
            
            uint8_t hora[4]={tiempo.bcd[2],  tiempo.bcd[3], tiempo.bcd[4],tiempo.bcd[5]};
            //parpadeo de la hora y los puntos             
            if(!clock_time_is_valid(reloj)) {
                //implemento parpadeo de la hora
                if(parpadeo){
                    screen_write_BCD(board->screen, hora, 4);
                    // Encender punto entre horas y minutos (después del segundo dígito)
                    screen_add_point(board->screen, 2);
                    parpadeo=false;
                }else{
                    parpadeo=true;
                }
    
            if((F1C)){
                control=0;
                F1C=true;
                clock_set_time(reloj, &tiempo);

            }
            control++;
            }else{
                screen_write_BCD(board->screen, hora, 4);
                // Encender punto entre horas y minutos (después del segundo dígito)
                screen_add_point(board->screen, 2);
            }
            clock_new_tick(reloj);
        }
        // Delay y refresco del display
        if(digital_input_get_is_active(F1)){
                control++;
                if(control>2 && F1C){
                    //si se presiona F1 mas de 2 segundos se activa el modo de ajuste
                    
                    F1C=false;
                    control=0;
                    }

                }  
        
        for (int index = 0; index < 100; index++) {
            for (int delay = 0; delay < 25000; delay++) {
                //le quite un 0 a delay para que vaya mas rapido(agregar al presentar)
                __asm("NOP");
            }
            screen_refresh(board->screen);
        }
    }
}
*/
int main(void) {
    int divisor=0,control_F1 = 0;
    bool ajustar_hora=false,hora_valida = false;
    // Inicialización de la placa y obtención de los periféricos
    board_t board = board_create();
    digital_input_t F1 = digital_input_create(5,12,false);
    // Crea el reloj con una estimación de 5 actualizaciones por segundo
    clock_t reloj = clock_create(TICKS_PER_SECOND); 
    
    while (true) {
        divisor++;
        if (divisor == 5) {
            divisor = 0;
            // Obtener hora actual
            clock_time_t tiempo={0};
            uint8_t hora[4]={tiempo.bcd[2], tiempo.bcd[3], tiempo.bcd[4], tiempo.bcd[5]};
            uint8_t hora_crota[4]={1,2,3,4};
            //ajusto la hora

            if(digital_was_activated(F1)||hora_valida){
                if(digital_input_get_is_active(F1)){
                    // Si se presiona el botón de ajuste de hora, se activa el modo de ajuste
                    screen_write_BCD(board->screen, hora_crota, 4);
                    // Encender punto entre horas y minutos (después del segundo dígito)
                    screen_add_point(board->screen, 2);
                    control_F1 = 0;
                    //ajustar_hora=(board->set_time);
                    hora_valida=true;
                }
                
                
            } 
            control_F1++;
            if(!hora_valida) {
                //como la hora no es valida, se muestra 00.00
                screen_write_BCD(board->screen, hora, 4);
                // Encender punto entre horas y minutos (después del segundo dígito)
                screen_add_point(board->screen, 2);
                //ahora tiene que parpadear, uso la funcion definida en screen.c
                display_flash_digits(board->screen, 0, 3, 50);
            }else{
                screen_write_BCD(board->screen, hora_crota, 4);
                // Encender punto entre horas y minutos (después del segundo dígito
                screen_add_point(board->screen, 2);
            }
            clock_new_tick(reloj);
        }
        // Delay y refresco del display
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
