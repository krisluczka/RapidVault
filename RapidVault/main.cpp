#include <iostream>
#include "RapidVault.h"


int main() {
	rv::database db;

	rv::table* t;

	db.rvquery( "CREATE TABLE users; CREATE COLUMNS users name surname profession main_city;" );
	db.rvquery( "INSERT users \"Krzysztof\" \"Luczka\" 1 3;" );
	db.rvquery( "INSERT users \"Object\" \"Object\" 2 3;" );
	db.rvquery( "INSERT users \"Jane\" \"Doe\" 1 3;" );
	db.rvquery( "INSERT users \"Juan\" \"Pablo II\" 0 3;" );

	db.rvquery( "CREATE TABLE professions; CREATE COLUMNS professions id profession_name salary city;" );
	db.rvquery( "INSERT professions 0 \"Pope\" 1239 1;" );
	db.rvquery( "INSERT professions 1 \"Doctor\" 2380 0;" );
	db.rvquery( "INSERT professions 2 \"Spoon bender\" 100000 2;" );

	db.rvquery( "CREATE TABLE cities; CREATE COLUMNS cities id name;" );
	db.rvquery( "INSERT cities 0 \"Poznan\";" );
	db.rvquery( "INSERT cities 1 \"Warszawa\";" );
	db.rvquery( "INSERT cities 2 \"Gdansk\";" );
	db.rvquery( "INSERT cities 3 \"Krakow\";" );

	db.rvquery( "SELECT cities; PUSH 4;" );
	db.rvquery( "SELECT cities; PUSH 4 \"Lodz\";" );
	db.rvquery( "SELECT cities; PUSH 4 \"Lodz\" 10;");

	db.rvquery( "INSERT cities 4; SELECT cities;" );
	db.rvquery( "INSERT cities 4 \"Lodz\"; SELECT cities;" );
	db.rvquery( "INSERT cities 4 \"Lodz\" 10; SELECT cities;" );
	
	// ultimate error generator
	/*db.rvquery( "JOIN users.profession LEFT id professions; SELECT users; JOIN u.p RIGHT id professions; JOIN users.profession n1 id professions; ALIAS; QWERTY; INSERT user \"test\"; WHERE users.surname users.name; WHERE users.main_city users.profession /; WHERE users.surname users.profession >=;" );
	db.check.print_errors();
	db.check.print_warnings();*/
}