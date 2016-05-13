#include"p18f4520.h"
#include "stdio.h"
#include "LCD.h"
#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "Delay.h"
#include"USART.h"
#include "string.h"

extern BYTE LCDText[16*2+1];
unsigned int pothole=0, speed_breaker=0;

void alert_enable();
void alert_disable();
unsigned long int sqrt(unsigned long int x);
void timer_init();
unsigned long int u_sensor1();
unsigned long int u_sensor2();
void normal();
void comp(unsigned long int a, unsigned long int b);
// enable buzzer sound and led indication connect to Rb0 -->buzzer and led --> RB1
void alert_enable()
{
	PORTBbits.RB0 = 1;
	PORTBbits.RB1 = 1;
	DelayMs(500);
	PORTBbits.RB0 = 0;
	PORTBbits.RB1 = 0;
}

//	disable buzzer and led indication
void alert_disable()
{
	PORTBbits.RB0 = 0;
	PORTBbits.RB1 = 0;
}

// results square root for a given int
unsigned long int sqrt(unsigned long int x)
{
    unsigned long int prev,k = 0;
    unsigned long int s = 1;
    for(k=0;k<1000;k++)
    {
        prev = s;
        s = (s + x/s)/2;
        if(prev == s)
        {
            break;
        }
    }
    return s;
}

void normal()
{
		alert_disable();
		PORTC = 0;
		strcpypgm2ram((char*)LCDText,"Welcome");
		display_row(0);
		DelayMs(2);
		strcpypgm2ram((char*)LCDText, "Safe Drive!");
		display_row(1);	
}

void timer_init()
{
	TMR0H = 0;
	TMR0L = 0;
	T0CON = 0X07;
}

unsigned long int u_sensor1()
{
	unsigned long int a = 0;
	strcpypgm2ram((char*)LCDText,"test mode 2!");
	display_row(0);
	DelayMs(250);
	TMR0H = 0;
	TMR0L = 0;
	PORTBbits.RB2 = 1;
	DelayMs(10);
	PORTBbits.RB2 = 0;
	DelayMs(1);
	while (PORTBbits.RB4 == 0);
	T0CONbits.TMR0ON = 1;
	while (PORTBbits.RB4 == 1);
	T0CONbits.TMR0ON = 0;
	a = (TMR0L | (TMR0H << 8));
	printf("sensor_1: %ld ", a);
	return a;
	a = a / 58.82;
	a = a + 1;
	if ((a > 2) && (a < 400)) {
		DelayMs(500);
		only_4_per_row(1, a);
		DelayMs(50);
		return a;
	}
	else {
		DelayMs(500);
		only_4_per_row(1, a);
		return a;
	}
}

unsigned long int u_sensor2()
{
	unsigned long int a = 0;
			strcpypgm2ram((char*)LCDText,"test mode 1");
display_row(0);
		DelayMs(250);
	TMR0H = 0;
	TMR0L = 0;
	PORTBbits.RB3 = 1;
	DelayMs(10);
	PORTBbits.RB3 = 0;
	DelayMs(1);
	while (PORTBbits.RB5 == 0);
	T0CONbits.TMR0ON = 1;
	while (PORTBbits.RB5 == 1);
	T0CONbits.TMR0ON = 0;
	a = (TMR0L | (TMR0H << 8));
	printf("sensor_2: %ld ", a);
	return a;
	a = a / 58.82;
	a = a + 1;
	if ((a > 2) && (a < 400)) {
		DelayMs(500);
		only_4_per_row(1, a);
		DelayMs(50);
		return a;
	}
	else {
		DelayMs(500);
		only_4_per_row(1, a);
		return a;
	}
}

void main(void)
{
	unsigned long int sensor_1 = 0;
	unsigned long int sensor_2 = 0, length = 0;

	LCDinit();
	timer_init();
	InitUSART();
	DelayMs(2);
	strcpypgm2ram((char*)LCDText,"WELCOME");
	display_row(0);
	DelayMs(2);
	strcpypgm2ram((char*)LCDText, "HAPPY HEALTH");
	display_row(1);
	TRISB = 0b11110000;
	TRISC = 0x00;
	LATB = 0x00;
	LATC = 0x00;
	PORTB = 0x00;
	PORTC = 0x00;
	ADCON1 = 0x1e;
	TRISA = 0xe1;		// POPORTDbits.RTA as Output
	PORTA = 0x1e;
	LATA = 0x1e;

	while(1)
	{
		strcpypgm2ram((char*)LCDText,"test mode inside!");
		display_row(0);
		DelayMs(250);

		sensor_1 = 0;
		sensor_2 = 0;
		DelayMs(50);
		sensor_1 = u_sensor1();
		DelayMs(50);
		sensor_2 = u_sensor2();
		strcpypgm2ram((char*)LCDText,"test mode!");
		display_row(0);
		DelayMs(250);
		length = 0;
		//length = sqrt((25 + (sensor_2 * sensor_2)) - (sensor_1 * sensor_1));
		only_4_per_row(0, sensor_1);
		DelayMs(2000);
		only_4_per_row(1, sensor_2);
		DelayMs(2000);
		only_4_per_row(1, length);
		DelayMs(2000);

		
	}
}