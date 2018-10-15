#include <stdlib.h>
#include <stdio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>

void tim2_isr(void)
{
    printf("Timer ISR\n");
    if (timer_get_flag(TIM2, TIM_SR_CC1IF)) {

        /* Clear compare interrupt flag. */
        timer_clear_flag(TIM2, TIM_SR_CC1IF);

        uint16_t compare_time = timer_get_counter(TIM2);
        printf("comp_time %d\n", compare_time);
    }
}

void exti1_isr(void)
{
    exti_reset_request(EXTI1);
    printf("EXTI1 ISR\n");
}

static void setup_exti_pe1(void)
{
    rcc_periph_clock_enable(RCC_GPIOE);
    nvic_enable_irq(NVIC_EXTI1_IRQ);
    nvic_set_priority(NVIC_EXTI1_IRQ, 0);
    gpio_mode_setup(GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO1);
    exti_select_source(EXTI1, GPIOE);
    exti_set_trigger(EXTI1, EXTI_TRIGGER_RISING);
    exti_enable_request(EXTI1);
}

static void setup_trig_pin(void)
{
    rcc_periph_clock_enable(RCC_GPIOE);
    gpio_mode_setup(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);
    gpio_set(GPIOE, GPIO12);
}

static void setup_tim2_timer(void)
{
    rcc_periph_clock_enable(RCC_TIM2);
    /* Enable TIM2 interrupt. */
    nvic_enable_irq(NVIC_TIM2_IRQ);

    /* Reset TIM2 peripheral to defaults. */
    rcc_periph_reset_pulse(RST_TIM2);

    timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT,
            TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

    timer_set_prescaler(TIM2, ((rcc_apb1_frequency * 2) / 5000));

    /* Disable preload. */
    timer_disable_preload(TIM2);
    timer_continuous_mode(TIM2);

    /* count full range, as we'll update compare value continuously */
    timer_set_period(TIM2, 65535);

    /* Set the initual output compare value for OC1. */
    timer_set_oc_value(TIM2, TIM_OC1, 500);

    /* Counter enable. */
    timer_enable_counter(TIM2);

    /* Enable Channel 1 compare interrupt to recalculate compare values */
    timer_enable_irq(TIM2, TIM_DIER_CC1IE);
}

static void delay_ccs(int ccs)
{
    int i;
    for (i = 0; i < ccs; i++)
        __asm__("nop");
}

int main(void)
{
    printf("BEGIN PROGRAM\n");
    /* setup_tim2_timer(); */
    setup_exti_pe1();
    setup_trig_pin();

    while (1) {
        gpio_set(GPIOE, GPIO12);
        delay_ccs(10);
        gpio_clear(GPIOE, GPIO12);
        delay_ccs(72000);
    }
}
