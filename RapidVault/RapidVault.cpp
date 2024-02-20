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

	rv::table t2 = *t;

	t2.create_column( "money" );
	
	t2.change_row( 0, "name", "Krzysztof2" );
	t2.change_row( 0, "surname", "Luczka2" );
	t2.change_row( 0, "age", 18 );
	t2.change_row( 0, "money", -13 );

	t2.change_row( 1, "name", "Object2" );
	t2.change_row( 1, "surname", "Object2" );
	t2.change_row( 1, "age", 19 );
	t2.change_row( 1, "money", false );

	t2.rename_column( "money", "cash" );

	t2.create_row();
	t2.change_row( 2, "name", "Object3" );
	t2.change_row( 2, "surname", "Object3" );
	t2.change_row( 2, "age", 0.5 );
	t2.change_row( 2, "cash", 12831293 );

	t->display();
	std::cout << std::endl;
	t2.display();
}