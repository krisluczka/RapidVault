/*
	Database class
*/
#pragma once
#include <iostream>
#include <cstdlib>
#include <variant>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <stack>
#include <algorithm>
#include "table.h"
#include "error_handler.h"

namespace rv {
	class database {
		public:
			database( std::string database_name ) : name( database_name ), operation_table( nullptr ),
				evaluation_format_error( false ), evaluation_division_warning( false ), evaluation_mixed_warning( false ) {};
			database() : name( "rapidvault_database" ), operation_table( nullptr ), evaluation_format_error( false ),
				evaluation_division_warning( false ), evaluation_mixed_warning( false ) {};
			~database();



			/*
				###############################
					Database manipulating
				###############################
			*/
			/*
				Displays a database structure
			*/
			void display() const;

			/*
				Renames a database
			*/
			inline void rename( std::string new_name ) {
				name = new_name;
			}

			/*
				###############################
					Tables manipulating
				###############################
			*/
			/*
				Creates a table
				Returns its index
			*/
			inline uint_fast64_t create_table( std::string name ) {
				table* t( new table );
				t->name = name;
				tables.push_back( t );
				return tables.size() - 1;
			}

			/*
				Renames specified table
				Returns its index
			*/
			inline uint_fast64_t rename_table( uint_fast64_t identifier, std::string new_name ) {
				if ( identifier < tables.size() ) {
					tables[identifier]->name = new_name;
					return identifier;
				}
				return NULL64_INDEX;
			}

			/*
				Deletes specified table
				Returns new amount of tables
			*/
			inline uint_fast64_t delete_table( uint_fast64_t identifier ) {
				if ( identifier < tables.size() ) {
					delete tables[identifier];
					tables.erase( tables.begin() + identifier );
					return identifier;
				}
				return NULL64_INDEX;
			}

			/*
				Returns an index to specified table
			*/
			inline uint_fast64_t get_table_index( std::string identifier ) const {
				for ( uint_fast64_t i( 0 ); i < this->tables.size(); ++i ) {
					if ( tables[i]->name == identifier ) {
						return i;
						break;
					}
				}
				return NULL64_INDEX;
			}

			/*
				Returns a pointer to specified table
			*/
			inline table* get_table_pointer( uint_fast64_t identifier ) const {
				if ( identifier < tables.size() )
					return tables[identifier];
				else
					return nullptr;
			}

			/*
				Default error handler for the queries
			*/
			error_handler check;

			/*
				###############################
					RVquery execution
				###############################
			*/
			bool rvquery( std::string, DISPLAY_TYPE type = DISPLAY_TYPE::NORMAL );

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
				Evaluation error flag
			*/
			bool evaluation_format_error;
			bool evaluation_division_warning;
			bool evaluation_mixed_warning;

			/*
				Calculates an cell_data value
				Returns a proper cell_data
			*/
			cell_data evaluate_operator( cell_data*, cell_data*, std::string );

			/*
				Evaluates mathematical/logical expressions for WHERE directive
				Returns boolean type variable
			*/
			bool evaluate_expression( std::vector<std::string*>&, uint_fast64_t );

			/*
				Modulo operator for floating point
			*/
			static inline long double modulo( long double a, long double b ) {
				long double x( std::floor(a / b));
				return a - b * x;
			}

			/*
				The heart of RVquery

				Evaluates the query based on the tokens
			*/
			void go_go_gadget_query( std::vector<std::string*>& );

			/*
				Query instruction functions
			*/
			void SELECT_query( std::vector<std::string*>&, const uint_fast64_t );
			void JOIN_query( std::vector<std::string*>&, const uint_fast64_t );
			void ALIAS_query( std::vector<std::string*>&, const uint_fast64_t );
			void PICK_query( std::vector<std::string*>&, const uint_fast64_t );
			void WHERE_query( std::vector<std::string*>&, const uint_fast64_t );
			void PUSH_query( std::vector <std::string*>&, const uint_fast64_t );

			void INSERT_query( std::vector<std::string*>&, const uint_fast64_t );
			void CREATE_query( std::vector<std::string*>&, const uint_fast64_t );
			
			void DISTINCT_query( std::vector<std::string*>&, const uint_fast64_t );
	};
}