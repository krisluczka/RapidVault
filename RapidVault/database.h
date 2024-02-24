/*
	Database class
*/
#pragma once
#include <iostream>
#include "table.h"
#include <cstdlib>
#include <variant>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <stack>

namespace rv {
	class database {
		public:
			database( std::string );
			database();
			~database();



			/*
				###############################
					Database manipulating
				###############################
			*/
			/*
				Displays a database structure
			*/
			void display();

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

			/*
				###############################
					RVquery execution
				###############################
			*/
			void rvquery( std::string );

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
				###############################
					RVquery
				###############################
			*/
			/*
				Operation table (where all the queries are evaluated)
			*/
			table* operation_table;

			/*
				Evaluates a cell_data operations
				Returns a proper calculation
			*/
			cell_data evaluate_operator( cell_data*, cell_data*, std::string );

			/*
				Evaluates mathematical/logical expressions for WHERE directive
				Returns boolean type variable
			*/
			bool evaluate_expression( std::vector<std::string*>&, uint_fast64_t );

			/*
				The heart of RVquery

				Function evaluates the query based on the tokens
			*/
			void go_go_gadget_query( std::vector<std::string*>& );
	};
}