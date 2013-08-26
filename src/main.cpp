#include "stm32-template.h"
#include "modbus/usart-rs485-modbus.h"
#include "node.h"

int main(void) __attribute__((weak));

Gpio usart_de(GPIOA, GPIO_Pin_8, RCC_APB2Periph_GPIOA);
Gpio usart_re(GPIOA, GPIO_Pin_8, RCC_APB2Periph_GPIOA);

Tim t1(TIM1, RCC_APB2Periph_TIM1, RCC_APB2PeriphClockCmd);
UsartRs485Modbus usart(USART1, RCC_APB2Periph_USART1, RCC_APB2PeriphClockCmd,
		usart_de, usart_re, t1);

Node slave(usart, 0x02);

Gpio led_green(GPIOC, GPIO_Pin_9, RCC_APB2Periph_GPIOC);
Gpio led_blue(GPIOC, GPIO_Pin_8, RCC_APB2Periph_GPIOC);

int main(void) {
	init();
	setup();

	for (;;)
		slave.handler();
}

void init() {
	SysTick_Config(SystemCoreClock / 1000);	// Tick per ms

	Gpio usart_tx(GPIOA, GPIO_Pin_9,
	RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO);
	usart_tx.init(GPIO_Mode_AF_OD);

	Gpio usart_rx(GPIOA, GPIO_Pin_10,
	RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO);
	usart_rx.init(GPIO_Mode_IN_FLOATING);

	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	nvic.configureGroup(NVIC_PriorityGroup_1);
	nvic.configure(TIM1_UP_TIM16_IRQn, 0, 3, ENABLE);
	nvic.configure(USART1_IRQn, 1, 2, ENABLE);
}

void setup() {
	led_green.init(GPIO_Mode_Out_PP);

	nvic.configure(TIM2_IRQn, 1, 3, ENABLE);
	Tim t2(TIM2, RCC_APB1Periph_TIM2, RCC_APB1PeriphClockCmd);
	t2.init(1000, 1000);
	t2.configureIT(TIM_IT_Update);
	t2.setState();

	slave.SlaveRtu::init();
	slave.init();
}

void delay(u32 ms) {
	extern vu32 millis;
	uint32_t start = millis;

	while (ms) {
		if (millis - start) {
			ms--;
			start++;
		}
	}
}

// delayMicroseconds ref to libmaple
void delayMicroseconds(vu32 us) {
	us *= SystemCoreClock / 3000000;

	asm volatile(
			"   mov r0, %[us]          \n\t"
			"1: subs r0, #1            \n\t"
			"   bhi 1b                 \n\t"
			:
			: [us] "r" (us)
			: "r0"
	);
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line) {
	fprintf(stderr, "assert failed on %s, line # %ld\r\n", file, line);
	while (1)
		;
}

#endif
