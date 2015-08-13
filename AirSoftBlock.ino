/*** Préconfiguration ***/
#define			SI					signed int
#define			SL					signed long
#define			UI					unsigned int
#define			UL					unsigned long

/*** Ecran ***/
#define			LCD_D4				2
#define			LCD_D5				3
#define			LCD_D6				4
#define			LCD_D7				5

#define			LCD_A				9
#define			LCD_V0				10
#define			LCD_E				11
#define			LCD_RS				12

#define			LCD_BACLIGHT_ON		analogWrite( LCD_A, 150 )
#define			LCD_BACLIGHT_OFF	digitalWrite( LCD_A, 0 )

#define			LCD_INIT			lcd.begin( 16, 2 ); for ( int i = 0; i <= 6; ++i ) lcd.createChar( ( uint8_t ) i, ( uint8_t * ) chars[ i ] )
#define			LCD_A_INIT			pinMode( LCD_A, OUTPUT ); LCD_BACLIGHT_ON
#define			LCD_V0_INIT			pinMode( LCD_V0, OUTPUT ); analogWrite( LCD_V0, 100 )

#include		<LiquidCrystal.h>
LiquidCrystal	lcd( LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7 );

/*** LEDs ***/
#define			LED_BLUE			A3
#define			LED_YELLOW			A2

#define			LED_BLUE_INIT		pinMode( LED_BLUE, OUTPUT )
#define			LED_YELLOW_INIT		pinMode( LED_YELLOW, OUTPUT )

#define			LED_BLUE_ON			analogWrite( LED_BLUE, 150 )
#define			LED_YELLOW_ON		analogWrite( LED_YELLOW, 150 )
#define			LED_BLUE_OFF		digitalWrite( LED_BLUE, LOW )
#define			LED_YELLOW_OFF		digitalWrite( LED_YELLOW, LOW )

/*** Boutons ***/
#define			BTN_BLUE			A0
#define			BTN_YELLOW			A1

#define			BTN_BLUE_INIT		pinMode( BTN_BLUE, INPUT )
#define			BTN_YELLOW_INIT		pinMode( BTN_YELLOW, INPUT )

#define			BTN_BLUE_ON			digitalRead( BTN_BLUE )
#define			BTN_YELLOW_ON		digitalRead( BTN_YELLOW )
#define			BTN_BLUE_OFF		!digitalRead( BTN_BLUE )
#define			BTN_YELLOW_OFF		!digitalRead( BTN_YELLOW )

#define			BTN_WAIT			BTN_BLUE_ON || BTN_YELLOW_ON
#define			BTN_WAIT_LOOP		while ( BTN_WAIT ) delay( 50 )

/*** Buzzer ***/
#include		<MyTone.h>
Tone			buzzer;
#include		<SimpleTimer.h>
SimpleTimer		timer;
#define			BUZZER_INIT			buzzer.begin( A4 ); timer.setInterval( 1000, callback )
#define			BUZZER_ON			beep = 1
#define			BUZZER_OFF			beep = 0

/*** Autre ***/
#define			SIZEOF( var, type )	sizeof( var ) / sizeof( type )

int				beep				= 0;
int				spaces				= 0;
int				timestamp			= 0;

uint8_t			chars[][ 8 ]		= {
	{  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0 },
	{ 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 },
	{ 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 },
	{ 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C },
	{ 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E },
	{ 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F },
	{  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0, 0x1F },
};

void	setup()
{
	BUZZER_INIT;

	LED_BLUE_INIT;
	LED_YELLOW_INIT;

	LCD_INIT;
	LCD_A_INIT;
	LCD_V0_INIT;

	lcd.clear();
	lcd.print( F( "  AirSoft Bomb  " ) );
	lcd.setCursor( 0, 1 );
	lcd.print( F( "   by Arubinu   " ) );

	delay( 2000 );
	return ;
}

int			mode			= 1;
void	loop()
{
	if ( mode )
	{
		mode = 0;
		lcd.clear();

		lcd.print( F( "Bleu:   Bombe   " ) );
		lcd.setCursor( 0, 1 );
		lcd.print( F( "Jaune:  Drapeaux" ) );

		LED_BLUE_OFF;
		LED_YELLOW_OFF;
		BTN_WAIT_LOOP;
		delay( 500 );
	}

	if ( BTN_BLUE_ON && ( mode = 1 ) )
		bomb();
	else if ( BTN_YELLOW_ON && ( mode = 2 ) )
		flags();

	timer.run();
	delay( 50 );
	return ;
}

void  display_tone( unsigned int freq, unsigned long millis )
{
	display_tone( freq, millis, false );

	return ;
}

void  display_tone( unsigned int freq, unsigned long millis, bool wait )
{
	buzzer.play( freq, millis );
	if ( wait )
		delay( millis * 1.30 );

	return ;
}

void	callback( void )
{
	UI		note			= 4978;

	if ( !beep )
		return ;

	if ( ( millis() / 1000 ) % 2 )
	{
		display_tone( note - 100, 30 );
		delay( 25 );
		display_tone( note + 100, 50 );
	}
	else
		display_tone( note, 75 );

	return ;
}

int		progress( int secs, int ( *check )( int ), int limit )
{
	UL		start;
	UL		finish;
	int		temp;
	int		period;
	int		pressed;

	temp = 0;
	period = 0;
	start = millis();
	finish = start + ( secs * 1000 );

	if ( BTN_BLUE_ON )
		pressed = BTN_BLUE;
	else if ( BTN_YELLOW_ON )
		pressed = BTN_YELLOW;
	else
		return ( 0 );

	while ( digitalRead( pressed ) )
	{
		if ( millis() >= finish )
			return ( 1 );
		else if ( check != NULL && check( limit ) )
			break ;

		if ( temp != ( millis() / 450 ) % 10 )
		{
			period = !period;
			temp = ( millis() / 450 ) % 10;
			buzzer.play( period ? 831 : 3322, 330 );
		}
		display_progress( start / 100, finish / 100, millis() / 100 );

		timer.run();
		delay( 5 );
	}

	return ( 0 );
}

int		progress( int secs )
{
	return ( progress( secs, NULL, 0 ) );
}

void	display_progress( int start, int finish, int current )
{
	int		temp			= 0;
	int		divs			= ( finish - start ) / 16;

	current -= start;
	lcd.setCursor( 0, 1 );

	for ( int columns = 0; columns < 16; ++columns )
	{
		temp = divs * columns;
		if ( current > ( temp + columns ) )
			lcd.write( ( uint8_t ) 5 );
		else
		{
			temp = current - temp;
			temp = ( temp < 0 ) ? 0 : ( ( temp * 5 ) / divs );
			temp = ( temp > 5 ) ? 5 : temp;

			if ( spaces && !temp )
				lcd.write( ( uint8_t ) 6 );
			else
				lcd.write( ( uint8_t ) ( temp ) );
		}
	}

	return ;
}

void	display_time( int sec )
{
	int		mins			= ( sec / 60 ) % 100;
	int		secs			= sec % 60;

	if ( mins < 10 )
		lcd.print( "0" );
	lcd.print( mins );

	lcd.print( ":" );

	if ( secs < 10 )
		lcd.print( "0" );
	lcd.print( secs );

	return ;
}

void	print_number( int val, int x, int y )
{
	lcd.setCursor( x, y );
	if ( val < 10 )
		lcd.print( F( " " ) );
	lcd.print( val );

	return ;
}

void	select_number( int *times, int size, int *numbers )
{
	*times = 0;
	while ( BTN_BLUE_OFF )
	{
		if ( BTN_YELLOW_ON )
		{
			*times = ( ( *times + 1 ) >= size ) ? 0 : *times + 1;
			print_number( numbers[ *times ], 9, 1 );

			BTN_WAIT_LOOP;
		}
	}

	*times = numbers[ *times ];
	return ;
}

void	finish( void )
{
	buzzer.play( 4000 );
	for ( int i = 10; i; --i )
	{
		LCD_BACLIGHT_OFF;
		delay( 250 );
		LCD_BACLIGHT_ON;
		delay( 500 );
	}
	buzzer.stop();

	return ;
}
