#include <iostream>
#include <string>
#include "RapidVault.h"

int main() {
	rv::RapidVault engine;

	/*engine.databases.push_back( new rv::database );

	engine.databases[0]->rename( "baza" );

	engine.databases[0]->rvquery( "CREATE TABLE test1; CREATE COLUMNS test1 column1 column2 column3; INSERT test1 10 \"\" -23.2; INSERT test1 -3.14159 \"Krzysztof\" 123214;" );

	std::cout << engine.save(0);*/

	engine.load( "baza.rv" );
	engine.databases[0]->display();
	rv::table* t1 = engine.databases[0]->get_table_pointer( 0 );

	t1->display( rv::DISPLAY_TYPE::NORMAL );




	/*rv::database db;

	std::string query;

	std::cout << " RapidVault pre-release 1.0\n";
	std::cout << "   made by Krzysztof Luczka\n\n";
	std::cout << " MIT license 2024\n";
	std::cout << " github.com/krisluczka/RapidVault\n";
	std::cout << " krisluczka.github.io/RapidVault/\n" << std::endl;

	while ( true ) {
		std::cout << " >> "; getline( std::cin, query );
		db.rvquery( query );
	}*/
}