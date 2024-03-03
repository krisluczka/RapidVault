#include <iostream>
#include <string>
#include "RapidVault.h"

int main() {
	rv::database db;

	db.rvquery( "CREATE TABLE users; CREATE COLUMNS users name surname city" );
	db.rvquery( "INSERT users \"Krzysztof\" \"Luczka\" 0" );
	db.rvquery( "INSERT users \"Hercule\" \"Poirot\" 2" );
	db.rvquery( "INSERT users \"Hercule2\" \"Poirot\" 2" );
	db.rvquery( "INSERT users \"Tony\" \"Stark\" 1" );
	db.rvquery( "INSERT users \"Tony\" \"Stark2\" 1" );
	db.rvquery( "INSERT users \"Tony\" \"Stark3\" 1" );
	db.rvquery( "INSERT users \"Tony\" \"Stark4\" 1" );
	db.rvquery( "INSERT users \"Jack\" \"Sparrow\" 3" );
	db.rvquery( "INSERT users \"Jack3\" \"Sparrow\" 3" );

	db.rvquery( "CREATE TABLE cities; CREATE COLUMNS cities id name country" );
	db.rvquery( "INSERT cities 0 \"Poznan\" 1" );
	db.rvquery( "INSERT cities 1 \"New York\" 3" );
	db.rvquery( "INSERT cities 2 \"Spa\" 0" );
	db.rvquery( "INSERT cities 4 \"Moscow\" 4" );

	db.rvquery( "CREATE TABLE countries; CREATE COLUMNS countries id name" );
	db.rvquery( "INSERT countries 0 \"Belgium\"" );
	db.rvquery( "INSERT countries 1 \"Poland\"" );

	db.rvquery( "SELECT countries; JOIN countries.id SYM_DIFF country cities; PUSH ROW 0 \"test\" -123.2; PUSH COLUMNS test.column; \
		ALIAS countries.name Country; ALIAS cities.id CityID; ALIAS cities.name City; ALIAS test.column NewColumn; SAVE STATIC users;" );
	db.check.print_warnings();
	db.rvquery( "SELECT users-; PUSH ROW 2 1 0 -1; ALIAS users-.Country Country; ALIAS users-.CityID CityID; ALIAS users-.City City; ALIAS users-.NewColumn OldColumn; SAVE STATIC test_table;" );
	db.rvquery( "SELECT test_table; " );

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