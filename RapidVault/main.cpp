#include <iostream>
#include <string>
#include "RapidVault.h"


int main() {
	rv::database db;

	db.rvquery( "CREATE TABLE d; CREATE COLUMNS d t; INSERT d 4; SELECT d; WHERE 12 13 |;" );

	/*std::string query;

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