/**
 * Combined DHT11 temperature/humidity sensor and light sensor (LSENS) test program
 * for STM32 microcontrollers
 *
 * This program initializes and reads from both a DHT11 temperature/humidity sensor
 * and a light sensor, displaying values on an LCD screen and using LED0 as a
 * system status indicator.
 */

/* Include necessary header files */
#include "led.h"       // LED control functions
#include "delay.h"     // Delay functions
#include "key.h"       // Button input functions
#include "sys.h"       // System configuration
#include "lcd.h"       // LCD display functions
#include "usart.h"     // UART communication 
#include "dht11.h"     // DHT11 temperature/humidity sensor functions
#include "adc.h"       // Analog-to-Digital Converter functions
#include "lsens.h"     // Light sensor functions
#include "atim.h"
#include "stdio.h"     // Added for printf functionality

void motorinit()
{    // Initialize PA8 and PA7 as outputs
    GPIO_InitTypeDef GPIO_InitStructure;
    // Enable GPIOA clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    // Push-pull output
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // 50MHz speed
    GPIO_Init(GPIOA, &GPIO_InitStructure);  // Initialize GPIOA
}

int main(void)
{	 
    /* Variable declarations */
    u8 t=0;            // Counter variable for timing   
    u8 temperature;    // Variable to store temperature reading    
    u8 humidity;       // Variable to store humidity reading
    u8 adcx;           // Variable to store light sensor ADC value   
    u32 data_seq = 0;  // Data sequence counter
    
    /* System initializations */
    delay_init();      // Initialize delay functions
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // Configure Nested Vectored Interrupt Controller: 2 bits for preemption priority, 2 bits for sub-priority
    uart_init(115200); // Initialize UART communication at 115200 baud rate
	
    LED_Init();        // Initialize LEDs
    LCD_Init();        // Initialize LCD display
    Lsens_Init();      // Initialize light sensor

    motorinit();
    /* Set text color to red and display header information on LCD */
    POINT_COLOR=RED;   // Set text color to red for headers
    LCD_ShowString(30,50,200,16,16,"WarShip STM32");	          // Display board name
    LCD_ShowString(30,70,200,16,16,"DHT11 & LSENS TEST");       // Display test title
    LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");            // Display manufacturer
    LCD_ShowString(30,110,200,16,16,"2025/2/28");               // Display date
    
    // Print startup message and data header to serial
    printf("SYSTEM_INIT\r\n");
    printf("DATA_SEQ;TEMP;HUMI;LIGHT\r\n");
    
    /* DHT11 initialization with error handling */
    while(DHT11_Init())  // Attempt DHT11 initialization, loop if returns error (non-zero)
    {
        LCD_ShowString(30,130,200,16,16,"DHT11 Error");        // Display error message
        printf("DHT11_ERROR\r\n");                             // Serial output for error
        delay_ms(200);                                         // Wait 200ms
        LCD_Fill(30,130,239,130+16,WHITE);                     // Clear error message by filling area with white
        delay_ms(200);                                         // Wait another 200ms (creates blinking effect)
    }								   
    LCD_ShowString(30,130,200,16,16,"DHT11 OK");               // Display success message once initialized
    printf("DHT11_OK\r\n");                                    // Serial output for success
    
    /* Set text color to blue and display labels for sensor readings */
    POINT_COLOR=BLUE;  // Change text color to blue for sensor readings
    LCD_ShowString(30,150,200,16,16,"Temp:  C");               // Temperature label with Celsius unit
    LCD_ShowString(30,170,200,16,16,"Humi:  %");               // Humidity label with percent unit
    LCD_ShowString(30,190,200,16,16,"LSENS_VAL:");             // Light sensor value label
    
    /* Main program loop */
    while(1)
    {	    	    
        if(t%10==0)    // Execute every 10 iterations (approximately every 100ms since each iteration has 10ms delay)
        {									  
            DHT11_Read_Data(&temperature,&humidity);           // Read temperature and humidity from DHT11 sensor
            adcx=Lsens_Get_Val();                              // Read light sensor value
            
            // Output data in simple array format: sequence;temperature;humidity;light
            printf("%lu;%u;%u;%u\r\n", data_seq++, temperature, humidity, adcx);
            
            if(temperature>28) // Temperature above 30, turn on heater
            {
                GPIO_SetBits(GPIOA, GPIO_Pin_8); 
            }
            else
            {
                GPIO_ResetBits(GPIOA, GPIO_Pin_8); 
            }
			
            LCD_ShowNum(30+40,150,temperature,2,16);               // Display temperature (2 digits, font size 16)
            LCD_ShowNum(30+40,170,humidity,2,16);                  // Display humidity (2 digits, font size 16)
            
            if(adcx<80) // Light level below 80, turn on light
            {
                GPIO_SetBits(GPIOA, GPIO_Pin_7); 
            }
            else
            {
                GPIO_ResetBits(GPIOA, GPIO_Pin_7); 
            }
			
            LCD_ShowxNum(30+10*8,190,adcx,3,16,0);                 // Display light sensor value (3 digits, font size 16)
        }				   
        delay_ms(10);  // 10ms delay per iteration
        t++;           // Increment counter
        if(t==20)      // Reset counter and toggle LED0 every 20 iterations (200ms)
        {
            t=0;               // Reset counter
            LED0=!LED0;        // Toggle LED0 as a system running indicator
        }
    }
}