#define F_CPU 16000000UL
 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/myInputOutput.h>
 
#define ACK 0x7E
#define LONG_TIME 10000

//Initialize SPI Master Device
void spi_init_master (void)
{
    DDRB = (1<<5)|(1<<3)|(1<<2);              //Set MOSI, SCK as Output
    //SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0); //Enable SPI, Set as Master
    SPCR=(1<<SPE)|(1<<MSTR);
	sei();                                   //Prescaler: Fosc/16, Enable Interrupts
}
 
//Function to send and receive data
unsigned char spi_tranceiver (unsigned char data)
{
    SPDR = data;                       //Load data into the buffer
    while(!(SPSR & (1<<SPIF) ));       //Wait until transmission complete
    return(SPDR);                      //Return received data
}
 
//Function to blink LED
void led_blink (uint16_t i)
{
    //Blink LED "i" number of times
    for (; i>0; --i)
    {
        PORTD|=(1<<0);
        _delay_ms(700);
        PORTD=(0<<0);
        _delay_ms(700);
    }
}
 
//Main
int main(void)
{
    spi_init_master();                  //Initialize SPI Master
    DDRD |= 0x01;                       //PD0 as Output
 
    unsigned char data;                 //Received data stored here
    uint8_t x = 3;                      //Counter value which is sent
 	pinMode('C',4,0);
	pinMode('D',5,1);
    int input = 0;
	digitalWrite('D',2,0);
	while(1)
    {
        data = 0x00;
		input = digitalRead('C',4);
		if(input == 1) {
			digitalWrite('D',5,1);
        	_delay_ms(100);
			digitalWrite('D',5,0);
			_delay_ms(200);
			data = spi_tranceiver(x);
			/*digitalWrite('D',5,1);
        	_delay_ms(200);
			digitalWrite('D',5,0);*/
		}     //Send "x", receive ACK in "data"
        if(data == ACK) {               //Check condition
            //If received data is the same as ACK, blink LED "x" number of times
            digitalWrite('D',5,1);
        	_delay_ms(200);
			digitalWrite('D',5,0);
        }
        else {
            //If received data is not ACK, then blink LED for a long time so as to determine error
            //led_blink(LONG_TIME);
        }
        //_delay_ms(500);                 //Wait
    }
}
/*int main(void) {
	pinMode('C',4,0);
	////PORTC |= (1<<4);
	pinMode('D',5,1);
    int input = 0;
	digitalWrite('D',2,0);
	while(1) {
		input = digitalRead('C',4);
		if(input == 1) {
				digitalWrite('D',5,1);
	        	_delay_ms(200);
				digitalWrite('D',5,0);
				//data = spi_tranceiver(2);
			} 
	}
	return 0;
}*/
