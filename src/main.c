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
#define TICKS_PER_SECOND 1
/* === Private data type declarations ========================================================== */

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

/* === Public function implementation ========================================================= */
int main(void) {
    int divisor = 0;

    // Inicialización de la placa y obtención de los periféricos
    board_t board = board_create();

    // Crea el reloj con una estimación de 5 actualizaciones por segundo
    clock_t reloj = clock_create(TICKS_PER_SECOND); 
    
    while (true) {
        divisor++;
        if (divisor == 5) {
            divisor = 0;
            // Obtener hora actual
            clock_time_t tiempo={0};
            
            if(!clock_get_time(reloj, &tiempo)){
            screen_write_BCD(board->screen, tiempo.bcd, 4);
            // Encender punto entre horas y minutos (después del segundo dígito)
            screen_add_point(board->screen, 2);
            clock_set_time(reloj, &tiempo);
            }else{
                uint8_t hora[4]={tiempo.bcd[2], tiempo.bcd[3], tiempo.bcd[4], tiempo.bcd[5]};
                screen_write_BCD(board->screen, hora, 4);
            // Encender punto entre horas y minutos (después del segundo dígito)
            screen_add_point(board->screen, 2);
            }
            clock_new_tick(reloj);
        }
        // Delay y refresco del display
        for (int index = 0; index < 100; index++) {
            for (int delay = 0; delay < 2500; delay++) {
                __asm("NOP");
            }
            screen_refresh(board->screen);
        }
    }
}
/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
