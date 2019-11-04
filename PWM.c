//STM32f0 , 48 Mhz internal clock + PLLx12

long frequency=2000,PWM_T,duty,d;

void main() {
   
   InitADC();  
   InitPWM();

   while(1){
     
     duty=ADC1_Get_Sample(2);
     frequency=ADC1_Get_Sample(3);
     //set PWM frequency to vary from 100 Hz to 10 Khz
     frequency=(long)(9900*(long)frequency);
     frequency/=4095;
     frequency+=100;
     
     PWM_T=PWM_TIM3_Init(frequency);
     d=(long)((long)PWM_T*(long)duty);
     d/=4095;
     PWM_TIM3_Set_Duty(d, _PWM_NON_INVERTED, _PWM_CHANNEL1);             
   }       
}

void InitPWM(){

  //init PWM of 20% duty cycle at 2 Khz	
  GPIO_Digital_Output(&GPIOA_BASE, _GPIO_PINMASK_6);
  PWM_T=PWM_TIM3_Init(frequency);
  PWM_TIM3_Set_Duty(PWM_T/5, _PWM_NON_INVERTED, _PWM_CHANNEL1);
  PWM_TIM3_Start(_PWM_CHANNEL1,&_GPIO_MODULE_TIM3_CH1_PA6);  
}

void InitADC(){

  ADC_Set_Input_Channel(_ADC_CHANNEL_2);
  ADC_Set_Input_Channel(_ADC_CHANNEL_3);
  ADC1_Init();
}
