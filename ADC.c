//STM32f0 , 48 Mhz internal clock + PLLx12

float Im=0;

void main() {
  
  ADC_Set_Input_Channel(_ADC_CHANNEL_1);
  ADC_Set_Input_Channel(_ADC_CHANNEL_2);
  ADC1_Init(); 
  UART1_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART1_PA9_10);
  UART1_Write_Text("Begin:\n");
   
   while(1){
   
        //map Im , 1 V = 1 A
        Im=(float)(3.3*ADC1_Get_Sample(1));
        Im/=4095;
       
	sprintf(txt, "%12f", Im);      
        UART1_Write_Text("im:");       // Write message on UART
        UART1_Write_Text(txt); 
        UART1_Write_Text("\n");           
       }       

}
