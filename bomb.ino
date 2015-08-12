int		bomb_times[]	= { 0, 0, 0 };
void	bomb( void )
{
	int		*temp		= NULL;
	int		t_size		= 0;
	int		t_times[]	= { 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90 };
	int		t_enable[]	= { 0, 2, 5, 10, 15, 30, 45, 60, 75, 90 };
	int		t_defuse[]	= { 0, 2, 5, 10, 15, 30, 45, 60, 75, 90 };

	lcd.clear();
	lcd.print( F( "  Mode   Bombe  " ) );

	for ( int t_step = 0; t_step < SIZEOF( bomb_times, int ); ++t_step )
	{
		lcd.setCursor( 0, 1 );
		switch ( t_step )
		{
			case 1: lcd.print( F( "Activer:    sec " ) ); temp = t_enable; t_size = SIZEOF( t_enable, int ); break;
			case 2: lcd.print( F( "Arreter:    sec " ) ); temp = t_defuse; t_size = SIZEOF( t_defuse, int ); break;
			default: lcd.print( F( "Duree:      min " ) ); temp = t_times; t_size = SIZEOF( t_times, int ); break;
		}
		print_number( temp[ 0 ], 9, 1 );
		BTN_WAIT_LOOP;
		select_number( &bomb_times[ t_step ], t_size, temp );
	}

	spaces = 1;
	lcd.clear();
	lcd.setCursor( 6, 0 );
	display_time( bomb_times[ 0 ] * 60 );
	while ( 42 )
	{
		lcd.setCursor( 0, 1 );
		for ( int i = 16; i; --i )
			lcd.write( ( uint8_t ) 6 );

		if ( progress( bomb_times[ 1 ] ) )
			break ;
	}

	bomb_run();
	return ;
}

void	bomb_run( void )
{
	int		start		= 0;

	timestamp = ( millis() / 1000 ) + ( bomb_times[ 0 ] * 60 );
	lcd.clear();
	while ( 42 )
	{
		if ( timestamp && ( millis() / 1000 ) >= timestamp )
			break ;

		lcd.setCursor( 6, 0 );
		display_time( timestamp - ( millis() / 1000 ) );
		lcd.setCursor( 0, 1 );
		for ( int i = 16; i; --i )
			lcd.write( ( uint8_t ) 6 );

		if ( !start && !( BTN_WAIT ) )
			start = 1;
		if ( start && bomb_progress() )
			break ;

		delay( 50 );
	}

	bomb_finish();
	while ( 42 )
		;
	return ;
}

int		bomb_progress( void )
{
	return ( progress( bomb_times[ 2 ], &bomb_check, timestamp ) );
}

int		bomb_check( int limit )
{
	lcd.setCursor( 6, 0 );
	display_time( limit - ( millis() / 1000 ) );

	return ( limit && ( millis() / 1000 ) >= limit );
}

void	bomb_finish( void )
{
	lcd.home();
	if ( ( millis() / 1000 ) >= timestamp )
		lcd.print( " Bombe  Explose " );
	else
		lcd.print( "Bombe  Desamorce" );

	finish();
	return ;
}
