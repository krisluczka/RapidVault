#include <iostream>
#include "table.h"
#include "database.h"


int main() {
	rv::database db;
	rv::table* t = db.create_table( "test_table" );

	t->create_column( "name" );
	t->create_column( "surname" );
	t->create_column( "age" );

	t->create_row();
	t->change_row( 0, "name", "Krzysztof" );
	t->change_row( 0, "surname", "Luczka" );
	t->change_row( 0, "age", 20 );

	t->create_row();
	t->change_row( 1, "name", "Object" );
	t->change_row( 1, "surname", "Object" );
	t->change_row( 1, "age", 0 );

	t->display();

	db.rvquery( "MERGE test test3; PICK token1 token2;" );
}