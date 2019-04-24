// Functions to manage RF functions via AVR

#include <avr/io.h>

#include <util/delay.h>

#ifndef F_CPU

//define cpu clock speed if not defined

#define F_CPU 8000000

#endif

//set desired baud rate

#define BAUDRATE 1200

//calculate UBRR value

#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)

//define receive parameters

#define SYNC 0XAA// synchro signal

#define RADDR 0x44

#define LEDON 0x11//switch led on command

#define LEDOFF 0x22//switch led off command

void USART_Init(void)

{

	//Set baud rate

	UBRRL=(uint8_t)UBRRVAL;		//low byte

	UBRRH=(UBRRVAL>>8);	//high byte

	//Set data frame format: asynchronous mode,no parity, 1 stop bit, 8 bit size

	UCSRC=(1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|

		(0<<USBS)|(0<<UCSZ2)|(1<<UCSZ1)|(1<<UCSZ0);	

	//Enable Transmitter and Receiver and Interrupt on receive complete

	UCSRB=(1<<TXEN);

}

void USART_vSendByte(uint8_t u8Data)

{

    // Wait if a byte is being transmitted

    while((UCSRA&(1<<UDRE)) == 0);

    // Transmit data

    UDR = u8Data;  

}

void Send_Packet(uint8_t addr, uint8_t cmd)

{

	USART_vSendByte(SYNC);//send synchro byte	

	USART_vSendByte(addr);//send receiver address

	USART_vSendByte(cmd);//send increment command

	USART_vSendByte((addr+cmd));//send checksum

}

void delayms(uint8_t t)//delay in ms

{

uint8_t i;

for(i=0;i<t;i++)

	_delay_ms(1);

}

int main(void)

{

USART_Init();

while(1)

	{//endless transmission

	//send command to switch led ON

	Send_Packet(RADDR, LEDON);

	delayms(100);

	//send command to switch led ON

	Send_Packet(RADDR, LEDOFF);

	delayms(100);

	}

	return 0;

}

 

In my case I used UART 1200 baud rate. It may be increased or decreased depending on distance and environment. For longer distances lower baud rates works better as there is bigger probability for transmission errors. Maximum bit rate of transmitter is 8kbits/s what is about 2400 baud. But what works in theory usually do not work in practice. So 1200 baud is maximum what I could get working correctly.

Transmitter sends two commands (LEDON and LEDOFF) to receiver with 100ms gaps. Receiver recognizes these commands and switches LED on or off depending on received command. This way I can monitor if data transfer works correctly. If LED blink is periodical – then transmission goes without errors. If there is an error in received data then LED gives shorter blink.

Receiver program code:

 

#include <avr/io.h>

#include <avr/interrupt.h>

#include <util/delay.h>

#ifndef F_CPU

//define cpu clock speed if not defined

#define F_CPU 4000000

#endif

//set desired baud rate

#define BAUDRATE 1200

//calculate UBRR value

#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)

//define receive parameters

#define SYNC 0XAA// synchro signal

#define RADDR 0x44

#define LEDON 0x11//LED on command

#define LEDOFF 0x22//LED off command

void USART_Init(void)

{

	//Set baud rate

	UBRRL=(uint8_t)UBRRVAL;		//low byte

	UBRRH=(UBRRVAL>>8);	//high byte

	//Set data frame format: asynchronous mode,no parity, 1 stop bit, 8 bit size

	UCSRC=(1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|

		(0<<USBS)|(0<<UCSZ2)|(1<<UCSZ1)|(1<<UCSZ0);	

	//Enable Transmitter and Receiver and Interrupt on receive complete

	UCSRB=(1<<RXEN)|(1<<RXCIE);//|(1<<TXEN);

	//enable global interrupts

}

uint8_t USART_vReceiveByte(void)

{

    // Wait until a byte has been received

    while((UCSRA&(1<<RXC)) == 0);

    // Return received data

    return UDR;

}

ISR(USART_RXC_vect)

{

	//define variables

	uint8_t raddress, data, chk;//transmitter address

	//receive destination address

	raddress=USART_vReceiveByte();

	//receive data

	data=USART_vReceiveByte();

	//receive checksum

	chk=USART_vReceiveByte();

	//compare received checksum with calculated

	if(chk==(raddress+data))//if match perform operations

	{

		//if transmitter address match

		if(raddress==RADDR)

			{

				if(data==LEDON)

					{

						PORTC&=~(1<<0);//LED ON

					}

				else if(data==LEDOFF)

					{

						PORTC|=(1<<0);//LED OFF

					}

				else

				{

					//blink led as error

					PORTC|=(1<<0);//LED OFF

					_delay_ms(10);

					PORTC&=~(1<<0);//LED ON	

				}

			}

	}

}

void Main_Init(void)

{

	PORTC|=(1<<0);//LED OFF

	DDRC=0X001;//define port C pin 0 as output;

	//enable global interrupts

	sei();

}

int main(void)

{

	Main_Init();

	USART_Init();

	while(1)

	{

	}

	//nothing here interrupts are working

	return 0;

}

