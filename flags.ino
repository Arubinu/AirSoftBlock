int		flags_times[]	= { 0, 0 };
void	flags( void )
{
	int		*temp		= NULL;
	int		t_size		= 0;
	int		t_times[]	= { 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90 };
	int		t_capture[]	= { 0, 2, 5, 10, 15 };

	lcd.clear();
	lcd.print( F( " Mode  Drapeaux " ) );
	for ( int t_step = 0; t_step < SIZEOF( flags_times, int ); ++t_step )
	{
		lcd.setCursor( 0, 1 );
		switch ( t_step )
		{
			case 1: lcd.print( F( "Capture:    sec " ) ); temp = t_capture; t_size = SIZEOF( t_capture, int ); break;
			default: lcd.print( F( "Duree:      min " ) ); temp = t_times; t_size = SIZEOF( t_times, int ); break;
		}
		print_number( temp[ 0 ], 9, 1 );
		BTN_WAIT_LOOP;
		select_number( &flags_times[ t_step ], t_size, temp );
	}

	spaces = 0;
	flags_run();
	return ;
}

int		capture			= 0;
int		teams[]			= { 0, 0 };
void	flags_run( void )
{
	int		start		= 0;
	int		change		= 0;

	timestamp = 0;
	lcd.clear();
	while ( 42 )
	{
		if ( start && ( millis() / 1000 ) >= start )
			break ;

		if ( !start || change )
		{
			change = 0;
			lcd.home();

			LED_BLUE_OFF;
			LED_YELLOW_OFF;
			if ( capture )
			{
				if ( capture == 1 )
					LED_YELLOW_ON;
				else
					LED_BLUE_ON;

				lcd.print( ( capture == 1 ) ? F( "  Base   Jaune  " ) : F( "   Base  Bleu   " ) );
			}
			else
				lcd.print( F( "  Base  Neutre  " ) );
		}

		flags_teams();
		if ( !start )
		{
			start = ( millis() / 1000 ) + ( flags_times[ 0 ] * 60 );
			BTN_WAIT_LOOP;
		}
		else if ( ( change = flags_progress( start ) ) == 1 )
			timestamp = millis() / 1000;

		timer.run();
		delay( 50 );
	}

	flags_finish();
	while ( 42 )
		;
	return ;
}

void	flags_teams( void )
{
	lcd.setCursor( 0, 1 );

	lcd.print( F( "J " ) );
	display_time( teams[ 0 ] + ( ( capture == 1 ) ? ( millis() / 1000 ) - timestamp : 0 ) );

	lcd.print( F( " " ) );
	lcd.write( byte( 1 ) );

	display_time( teams[ 1 ] + ( ( capture == 2 ) ? ( millis() / 1000 ) - timestamp : 0 ) );
	lcd.print( F( " B" ) );

	return ;
}

int		flags_progress( int limit )
{
	if ( ( BTN_BLUE_ON && capture != 2 )
		|| ( BTN_YELLOW_ON && capture != 1 ) )
	{
		lcd.home();
		lcd.print( ( BTN_YELLOW_ON ) ? F( " Capture  Jaune " ) : F( "  Capture Bleu  " ) );

		if ( flags_times[ 1 ] && !progress( flags_times[ 1 ], &flags_check, limit ) )
			return ( 2 );

		if ( !capture )
		{
			if ( BTN_BLUE_ON )
				capture = 2;
			else if ( BTN_YELLOW_ON )
				capture = 1;
		}
		else
		{
			teams[ capture - 1 ] += ( ( millis() / 1000 ) - flags_times[ 1 ] ) - timestamp;
			capture = 0;
		}

		return ( 1 );
	}

	return ( 2 );
}

int		flags_check( int limit )
{
	return ( limit && ( millis() / 1000 ) >= limit );
}

void	flags_finish( void )
{
	if ( capture )
		teams[ capture - 1 ] += ( ( millis() / 1000 ) - flags_times[ 1 ] ) - timestamp;

	lcd.home();
	LED_BLUE_OFF;
	LED_YELLOW_OFF;
	if ( teams[ 0 ] == teams[ 1 ] )
	{
		LED_BLUE_ON;
		LED_YELLOW_ON;
		lcd.print( "   Match  Nul   " );
	}
	else
	{
		if ( teams[ 0 ] > teams[ 1 ] )
		{
			LED_YELLOW_ON;
			lcd.print( " Gagnant  Jaune " );
		}
		else
		{
			LED_BLUE_ON;
			lcd.print( "  Gagnant Bleu  " );
		}
	}

	finish();
	return ;
}
