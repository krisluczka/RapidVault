#include <iostream>
#include "RapidVault.h"


int main() {
	rv::database db;
	rv::table* t = db.create_table( "users" );

	t->create_column( "name" );
	t->create_column( "surname" );
	t->create_column( "profession" );
	t->create_column( "main_city" );

	t->create_row();
	t->change_row( 0, "name", "Krzysztof" );
	t->change_row( 0, "surname", "Luczka" );
	t->change_row( 0, "profession", 1 );
	t->change_row( 0, "main_city", 3 );

	t->create_row();
	t->change_row( 1, "name", "Object" );
	t->change_row( 1, "surname", "Object" );
	t->change_row( 1, "profession", 2 );
	t->change_row( 1, "main_city", 3 );

	t->create_row();
	t->change_row( 2, "name", "Jane" );
	t->change_row( 2, "surname", "Doe" );
	t->change_row( 2, "profession", 1 );
	t->change_row( 2, "main_city", 3 );

	t->create_row();
	t->change_row( 3, "name", "Juan" );
	t->change_row( 3, "surname", "Pablo II" );
	t->change_row( 3, "profession", 0 );
	t->change_row( 3, "main_city", 3 );

	t = db.create_table( "professions" );

	t->create_column( "id" );
	t->create_column( "profession_name" );
	t->create_column( "salary" );
	t->create_column( "city" );

	t->create_row();
	t->change_row( 0, "id", 0 );
	t->change_row( 0, "profession_name", "Pope" );
	t->change_row( 0, "salary", 1239 );
	t->change_row( 0, "city", 1 );

	t->create_row();
	t->change_row( 1, "id", 1 );
	t->change_row( 1, "profession_name", "Doctor" );
	t->change_row( 1, "salary", 2380 );
	t->change_row( 1, "city", 0);

	t->create_row();
	t->change_row( 2, "id", 2 );
	t->change_row( 2, "profession_name", "Spoon bender" );
	t->change_row( 2, "salary", 100000 );
	t->change_row( 2, "city", 2 );

	t = db.create_table( "cities" );

	t->create_column( "id" );
	t->create_column( "name" );

	t->create_row();
	t->change_row( 0, "id", 0 );
	t->change_row( 0, "name", "Poznan" );

	t->create_row();
	t->change_row( 1, "id", 1 );
	t->change_row( 1, "name", "Warszawa" );

	t->create_row();
	t->change_row( 2, "id", 2 );
	t->change_row( 2, "name", "Gdansk" );

	t->create_row();
	t->change_row( 3, "id", 3 );
	t->change_row( 3, "name", "Krakow" );

	db.rvquery( "SELECT users; JOIN users.profession N1 id professions; WHERE professions.city 2 ==; PICK users.name; ALIAS users.name Name" );
}