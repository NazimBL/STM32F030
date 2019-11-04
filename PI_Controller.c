//STM32f0 , 48 Mhz internal clock + PLLx12

const float Kp=5;
const float Ki=0.01;
float integral=0,error=0;
float output=0;

const float ISC=2;
const float R=82;
long PWM_T,duty,d;
float IREF=1.1;
float Vm,Im;

char sync=0;

void InitTimer17();
void InitTimer16();
void InitPWM();
void InitADC();
void InitUart();

//Set point ISR
void Timer16_interrupt() iv IVT_INT_TIM16 {

  TIM16_SR.UIF = 0;
  //map Im, 1 A=
  Im=(float)(3.3*ADC1_Get_Sample(0));
  Vm/=4095;
  //gain
  Vm*=12;
  //Set Point I
  IREF=Vm/R;   
}
 
//PI ISR
void Timer17_interrupt() iv IVT_INT_TIM17 {

  TIM17_SR.UIF = 0; 
  sync=1;     
}  

void Setup(){
  
  InitTimer17();
  InitTimer16();
  InitPWM(); 
  InitADC();    
}

void main() {
   
   Setup();  
   while(1){
         
     if(sync==1){
      
       //measure feedback current	
       Im=(float)(3.3*ADC1_Get_Sample(1));
       //map Im , 1 V = 1 A    
       Im/=4095;
       
       error=IREF-Im;
       integral+=error;

       //limiting integral error 	       
       if(integral>ISC)integral=2;
       if(integral<-2)integral=-2;
       output=(Kp*error)+(Ki*integral); 
       output=(float)(output*PWM_T);
       duty=(unsigned int)(output);
 
       //limiting controller output
       if(duty>PWM_T)duty=PWM_T;
       else if(duty<0)duty=0;
       PWM_TIM3_Set_Duty(duty, _PWM_NON_INVERTED, _PWM_CHANNEL1);
       sync=0;   
     }           
   }       
}

void InitPWM(){

  GPIO_Digital_Output(&GPIOA_BASE, _GPIO_PINMASK_6);
  PWM_T=PWM_TIM3_Init(2000);
  PWM_TIM3_Set_Duty(PWM_T/5, _PWM_NON_INVERTED, _PWM_CHANNEL1);
  PWM_TIM3_Start(_PWM_CHANNEL1,&_GPIO_MODULE_TIM3_CH1_PA6);  
}

void InitADC(){

  ADC_Set_Input_Channel(_ADC_CHANNEL_0);
  ADC_Set_Input_Channel(_ADC_CHANNEL_1);
  ADC1_Init();
}

void InitTimer16(){

  RCC_APB2ENR.TIM16EN = 1;       // Enable clock gating for timer module 2
  TIM16_CR1.CEN = 0;             // Disable timer 
  // 100 Hz 
  TIM16_PSC = 9;                 // Set timer prescaler.
  TIM16_ARR =23999 ;
  NVIC_IntEnable(IVT_INT_TIM16); // Enable timer interrupt
  TIM16_DIER.UIE = 1;            // Update interrupt enable
  TIM16_CR1.CEN = 1;             // Enable timer
}

void InitTimer17(){

  RCC_APB2ENR.TIM17EN = 1;
  TIM17_CR1.CEN = 0;
  // 2 KHz 
  TIM17_PSC =  9; 
  TIM17_ARR =1199;
  NVIC_IntEnable(IVT_INT_TIM17);
  TIM17_DIER.UIE = 1;
  TIM17_CR1.CEN = 1;
}