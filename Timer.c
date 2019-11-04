//STM32f0 , 48 Mhz internal clock + PLLx12

//Timer ISR
void Timer16_interrupt() iv IVT_INT_TIM16 {

  TIM16_SR.UIF = 0;
  GPIOA_ODRbits.ODR7=~GPIOA_ODRbits.ODR7;   
}

void main() {
   
  InitTimer16();
  GPIO_Digital_Output(&GPIOA_BASE, _GPIO_PINMASK_5);
  GPIOA_ODRbits.ODR5=1;
  GPIO_Digital_Output(&GPIOA_BASE, _GPIO_PINMASK_7);
  GPIOA_ODRbits.ODR7=1;
  
  while(1); 
}

void InitTimer16(){

  RCC_APB2ENR.TIM16EN = 1;       // Enable clock gating for timer module 16
  TIM16_CR1.CEN = 0;             // Disable timer 
  // 100 Hz 
  TIM16_PSC = 9;                 // Set timer prescaler.
  TIM16_ARR =23999 ;
  NVIC_IntEnable(IVT_INT_TIM16); // Enable timer interrupt
  TIM16_DIER.UIE = 1;            // Update interrupt enable
  TIM16_CR1.CEN = 1;             // Enable timer
}
