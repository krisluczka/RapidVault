﻿#include <iostream>
#include <string>
#include "RapidVault.h"

int main() {
	rv::database db;

	db.rvquery( "CREATE TABLE test; CREATE COLUMNS test id name surname;" );
	db.rvquery( "INSERT test 0 \"Krzysztof\" \"Luczka\"; INSERT test 1 \"Object\" \"Object\";" );

	db.rvquery( "CREATE TABLE other; CREATE COLUMNS other id name surname;" );
	db.rvquery( "INSERT other 0 \"Hercule\" \"Poirot\"; INSERT other 2 \"Sherlock\" \"Holmes\"; INSERT other 3 \"Jacques\" \"Clouseau\";" );

	// the same results
	db.rvquery( "SELECT test; JOIN test.id UNION id other;" );
	db.rvquery( "SELECT other; JOIN other.id UNION id test;" );

	// the same results
	db.rvquery( "SELECT test; JOIN test.id SYM_DIFF id other;" );
	db.rvquery( "SELECT other; JOIN other.id SYM_DIFF id test;" );


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