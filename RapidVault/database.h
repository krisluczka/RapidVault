/*
	Database class
*/
#pragma once
#include <iostream>
#include "table.h"
#include <variant>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#define NULL64_INDEX 18446744073709551615

namespace rv {
	/*
		Relation types
			0 - none
			1 - 1:1
			2 - N:1
			3 - N:M
	*/
	struct relation {
		relation( uint_fast64_t a, uint_fast64_t b, uint_fast8_t t ) : A( a ), B( b ), type( t ) {}
		relation() : A( NULL64_INDEX ), B( NULL64_INDEX ), type(0) {}
		uint_fast64_t A, B;
		uint_fast8_t type = 0;
	};

	class database {
		public:
			database( std::string );
			database();
			~database();



			/*
				###############################
					Relations manipulating
				###############################
			*/
			/*
				Adds a new relation between two distinct tables
				Returns its index
			*/
			uint_fast64_t add_relation( std::variant<uint_fast64_t, std::string>, std::variant<uint_fast64_t, std::string>, uint_fast8_t );
			/*
				Removes a relation
				Returns new amount of relations
			*/
			uint_fast64_t remove_relation( uint_fast64_t );
			uint_fast64_t remove_relation( std::variant<uint_fast64_t, std::string>, std::variant<uint_fast64_t, std::string>, uint_fast8_t );

			/*
				###############################
					Tables manipulating
				###############################
			*/
			/*
				Creates a table
				Returns its pointer
			*/
			table* create_table( std::string );
			uint_fast64_t rename_table();
			uint_fast64_t remove_table();

			/*
				Returns a pointer to given table
			*/
			table* get_table_pointer( std::variant<uint_fast64_t, std::string> ) const;

			/*
				Returns an index to given table with certain name
			*/
			uint_fast64_t get_table_index( std::string ) const;
		private:
			/*
				Database name
			*/
			std::string name;
			
			/*
				Tables stored in a database
			*/
			std::vector<table*> tables;

			/*
				Tables' relations
			*/
			std::vector<relation*> relations;
	};
}