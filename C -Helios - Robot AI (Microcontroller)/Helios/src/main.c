/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>

#define F_CPU 1000000UL // 1 MHz
#include <util/delay.h>

//int const LDRRESPONSE = 2; //8-bit resolution
int const LDRRESPONSE = 6; //10-bit resolution

int const DARKNESSPAUSE = 16; // 8-bit
//int const DARKNESSPAUSE = 64; // 10-bit

int AVOIDANCE_SENSITIVITY = 12; // level of light from the LDR that the robot will perform a menuver to avoid an object

int const DEG90_LEFT = 14;
int const DEG90_RIGHT = 16;

int const DEG5_LEFT = 1;
int const DEG5_RIGHT = 1;

int const FORWARD1 = 1;

int leftLdrOffset = 0;


void initialisePins(void);
void moveForward(int cycles);
void reverse(int cycles);
void turnRight(int cycles);
void turnLeft(int cycles);
int ADCConvert(int port);
void calibrateLDRs(void);
void avoidObstacle(void);

int main (void)
{
	// Insert system clock initialization code here (sysclk_init()).

	board_init();
	
	initialisePins();
	//setupADC();
	//calibrateLDRs();
	
	PORTD |= (1 << PD0) | (1 << PD1);
	_delay_ms(1.5);
	PORTD &= ~(1 << PD0) & ~(1 << PD1);
	_delay_ms(19.9);


	// Insert application code here, after the board has been initialized.

	int leftLDR;
	int rightLDR;
	ADCConvert(5);
	while(true)
	{
		_delay_ms(25);
		
		leftLDR = ADCConvert(5); // retrieve leftLDR data
		rightLDR = ADCConvert(4); // retrieve rightLDR data
		
		if(ADCConvert(3) < AVOIDANCE_SENSITIVITY) // if the level of light on the front LDR is below the avoidance setting
		{
			avoidObstacle(); // perform avoidance menuver
		}
		
		if(ADCConvert(4) < DARKNESSPAUSE && ADCConvert(5) < DARKNESSPAUSE)
		{
			//pause
		}
		else if(leftLDR > rightLDR)
		{
			if((leftLDR - rightLDR) > LDRRESPONSE)
			{
				turnLeft(DEG5_LEFT);
			}
			else
			{
				moveForward(FORWARD1);
			}
		}
		else if(rightLDR > leftLDR)
		{
			if ((rightLDR - leftLDR) > LDRRESPONSE)
			{
				turnRight(DEG5_RIGHT);
			} 
			else
			{
				moveForward(FORWARD1);
			}
		}
		else
		{
			moveForward(FORWARD1);
		}
	}
}

void initialisePins()
{
	DDRD = 0b00000111; // sets PD0 and PD1 to output
	PORTD = 0b00000100;
	
	DDRC = 0b00000000; // sets PC4 to input
	PORTC = 0b00110000; // enable pullup of PC4
}

int ADCConvert(int port)
{				
	ADMUX = port; // selects the pin to collect analogue data from and convert into digital data
		
	ADMUX |= (1 << REFS0); //Sets reference to AVcc with external capacitor on AREF pin
		
	//ADMUX |= (1 << ADLAR); // set ADLAR to high to enable 8-bit resolution, the res at which the digital signal will be converted into
	ADMUX &= ~(1 << ADLAR); //16 bit resolution
		
	ADCSRA |= (1 << ADPS2); //16 prescale for 1Mhz
		
	ADCSRA |= (1 << ADEN); // enable the ADC
	
	ADCSRA |= (1 << ADSC); //begin conversion
	while(ADCSRA & (1 << ADSC)); // waits for conversion to finish
	//int rightLDR = ADCH; // stores digital data
	
	return (ADCL + (ADCH << 8)); //return 10 bit resolution
}

void avoidObstacle()
{
	reverse(24);
	
	int leftLDR = ADCConvert(5); // retrieve leftLDR data
	int rightLDR = ADCConvert(4); // retrieve rightLDR data
	
	if(leftLDR > rightLDR)
	{
		turnLeft(DEG90_LEFT*2);
	}
	else if(rightLDR > leftLDR)
	{
		turnRight(DEG90_RIGHT*2);
	}
	else
	{
		turnLeft(DEG90_LEFT*2);
	}
}

void moveForward(int cycles)
{
	while(cycles > 0)
	{
		//left servo
		PORTD |= (1 << PD0);
		_delay_ms(0.9);
		PORTD &= ~(1 << PD0);
		_delay_ms(15);
		
		
		//right servo
		PORTD |= (1 << PD1);
		_delay_ms(2.1);
		PORTD &= ~(1 << PD1);
		_delay_ms(15);
		cycles-=1;
	}
}

void reverse(int cycles)
{
	while(cycles > 0)
	{
		//left servo
		PORTD |= (1 << PD0);
		_delay_ms(2.1);
		PORTD &= ~(1 << PD0);
		_delay_ms(15);
		
		
		//right servo
		PORTD |= (1 << PD1);
		_delay_ms(0.9);
		PORTD &= ~(1 << PD1);
		_delay_ms(15);
		cycles-=1;
	}
}

void turnLeft(int cycles)
{
	for(int i = cycles; i > 0; i--)
	{
		PORTD |= (1 << PD0);
		_delay_ms(2.1);
		PORTD &= ~(1 << PD0);
		_delay_ms(15);
				
				
		PORTD |= (1 << PD1);
		_delay_ms(2.1);
		PORTD &= ~(1 << PD1);
		_delay_ms(15);
	}
}

void turnRight(int cycles)
{
	for(int i = cycles; i > 0; i--)
	{
		PORTD |= (1 << PD0);
		_delay_ms(0.9);
		PORTD &= ~(1 << PD0);
		_delay_ms(15);
		
		
		PORTD |= (1 << PD1);
		_delay_ms(0.9);
		PORTD &= ~(1 << PD1);
		_delay_ms(15);
	}
}
