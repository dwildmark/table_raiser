#include <stdlib.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

static void rcc_init(void)
{
	rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_UART4);
}

static void uart_init(void)
{
    gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO10 | GPIO11);
    gpio_set_af(GPIOC, GPIO_AF5, GPIO10 | GPIO11);
    gpio_set_output_options(GPIOC, GPIO_OTYPE_PP, 
                            GPIO_OSPEED_50MHZ, GPIO10);
    usart_set_baudrate(UART4, 115200);
    usart_set_databits(UART4, 8);
    usart_set_stopbits(UART4, USART_STOPBITS_1);
    usart_set_parity(UART4, USART_PARITY_NONE);
    usart_set_mode(UART4, USART_MODE_TX_RX);
    usart_set_flow_control(UART4, USART_FLOWCONTROL_NONE);
    usart_enable(UART4);
}

int main(void)
{
    rcc_init();
    uart_init();

    while (1) {
        usart_send_blocking(UART4, usart_recv_blocking(UART4));
    }
}
