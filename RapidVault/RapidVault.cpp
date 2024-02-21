#include <iostream>
#include "table.h"
#include "database.h"


int main() {
	rv::database db;
	rv::table* t = db.create_table( "users" );

	t->create_column( "name" );
	t->create_column( "surname" );
	t->create_column( "profession" );

	t->create_row();
	t->change_row( 0, "name", "Krzysztof" );
	t->change_row( 0, "surname", "Luczka" );
	t->change_row( 0, "profession", 1 );

	t->create_row();
	t->change_row( 1, "name", "Object" );
	t->change_row( 1, "surname", "Object" );
	t->change_row( 1, "profession", 2 );

	t->create_row();
	t->change_row( 2, "name", "Jane" );
	t->change_row( 2, "surname", "Doe" );
	t->change_row( 2, "profession", 1 );

	t = db.create_table( "professions" );

	t->create_column( "id" );
	t->create_column( "profession_name" );

	t->create_row();
	t->change_row( 0, "id", 0 );
	t->change_row( 0, "profession_name", "Engineer" );

	t->create_row();
	t->change_row( 1, "id", 1 );
	t->change_row( 1, "profession_name", "Doctor" );

	t->create_row();
	t->change_row( 2, "id", 2 );
	t->change_row( 2, "profession_name", "Spoon bender" );

	db.rvquery( "SELECT users; JOIN users.profession N1 id professions;				;;;;;		; " );
}