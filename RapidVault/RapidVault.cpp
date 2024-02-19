#include <iostream>
#include "table.h"
#include "database.h"


int main() {
	rv::table test;

	test.create_column( "id" );
	test.create_column( "name" );
	test.create_column( "surname" );
	test.create_column( "money" );
	test.create_column( "country" );

	test.create_row();
	test.create_row();
	test.create_row();

	test.change_row( 0, "id", rv::cell_data( 0 ) );
	test.change_row( 0, "name", rv::cell_data( "Test object" ) );
	test.change_row( 0, uint_fast16_t( 2 ), rv::cell_data( "User" ) );
	test.change_row( 0, uint_fast16_t( 3 ), rv::cell_data( 1435901251351 ) );
	test.change_row( 0, uint_fast16_t( 4 ), rv::cell_data( "Poland" ) );

	test.change_row( 1, uint_fast16_t( 0 ), rv::cell_data( 1 ) );
	test.change_row( 1, "name", rv::cell_data( "Krzysztof" ) );
	test.change_row( 1, uint_fast16_t( 2 ), rv::cell_data( "Luczka" ) );
	test.change_row( 1, "money", rv::cell_data( -13.f ) );

	test.change_row( 2, uint_fast16_t( 0 ), "testid" );
	test.change_row( 2, uint_fast16_t( 1 ), rv::cell_data( "User2" ) );
	test.change_row( 2, uint_fast16_t( 2 ), rv::cell_data( "User3" ) );
	test.change_row( 2, uint_fast16_t( 3 ), rv::cell_data( false ) );
	test.change_row( 2, "country", rv::cell_data( 16 ) );

	test.display();
}