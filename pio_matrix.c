#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"
#include "config_matriz.h"
#include "pio_matrix.pio.h"
#include "init_GPIO.h"
#include "pico/bootrom.h"

uint32_t last_time = 0;
int num_atual = 0; 

// Função de interrupção com debouncing
void gpio_irq_handler(uint gpio, uint32_t events)
{
    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    // Verifica se passou tempo suficiente desde o último evento
    if (current_time - last_time > 200000) // 200 ms de debouncing
    {
        if(gpio == BTN_A)
        {
            num_atual++;
        }
        else if (gpio == BTN_B)
        {
            num_atual--;
        }
        else if (gpio == JSTICK)
        {
            reset_usb_boot(0, 0);
        }
        last_time = current_time;
        
    }
}
    
//função principal
int main()
{
    init_gpio();
    
    PIO pio = pio0; 
    //configurações da PIO
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BTN_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(JSTICK, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    
    while (true) {

        //laço para piscar o led vermelho 5x por seg
        for(int i = 0; i < 5; i++){
            gpio_put(LED_RED, !gpio_get(LED_RED));
            sleep_ms(tempo);
       }
        switch (num_atual)
        {
        //extra para apagar os leds
        case -1:
            desenho_pio(apagar_leds, valor_led, pio, sm, r, g, b);
            break;
        case 0:
            desenho_pio(num_0, valor_led, pio, sm, r, g, b);
            break;
        case 1:
            desenho_pio(num_1, valor_led, pio, sm, r, g, b);
            break;
        case 2:
            desenho_pio(num_2, valor_led, pio, sm, r, g, b);
            break;
        case 3:
            desenho_pio(num_3, valor_led, pio, sm, r, g, b);
            break;    
        case 4:
            desenho_pio(num_4, valor_led, pio, sm, r, g, b);
            break;
        case 5:
            desenho_pio(num_5, valor_led, pio, sm, r, g, b);
            break;
        case 6:
            desenho_pio(num_6, valor_led, pio, sm, r, g, b);
            break;
        case 7:
            desenho_pio(num_7, valor_led, pio, sm, r, g, b);
            break;
        case 8:
            desenho_pio(num_8, valor_led, pio, sm, r, g, b);
            break;
        case 9:
            desenho_pio(num_9, valor_led, pio, sm, r, g, b);
            break;
        }
    }
}


    

