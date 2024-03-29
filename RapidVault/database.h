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
#include <regex>
#include "table.h"
#include "error_handler.h"

namespace rv {
	class database {
		friend class RapidVault;
		public:
			database( std::string database_name ) : name( database_name ), operation_table( nullptr ),
				evaluation_format_error( false ), evaluation_division_warning( false ), evaluation_mixed_warning( false ), evaluation_regex_warning( false ){};
			database() : name( "rapidvault_database" ), operation_table( nullptr ), evaluation_format_error( false ),
				evaluation_division_warning( false ), evaluation_mixed_warning( false ), evaluation_regex_warning( false ) {};
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
				// checking the names
				for ( table* t : tables ) {
					// if such table already exists
					if ( t->name == name ) {
						// force it
						create_table( name + "-" );
						return NULL64_INDEX;
					}
				}
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
			bool evaluation_regex_warning;

			/*
				Operator number assigning
			*/
			static inline uint_fast8_t is_operator( std::string* token ) {
					 if ( *token == "+" )		return 1;
				else if ( *token == "-" )		return 2;
				else if ( *token == "*" )		return 3;
				else if ( *token == "/" )		return 4;
				else if ( *token == "%" )		return 5;
				else if ( *token == "&&" )		return 6;
				else if ( *token == "||" )		return 7;
				else if ( *token == "<" )		return 8;
				else if ( *token == "<=" )		return 9;
				else if ( *token == ">" )		return 10;
				else if ( *token == ">=" )		return 11;
				else if ( *token == "==" )		return 12;
				else if ( *token == "!=" )		return 13;
				else if ( *token == "<<" )		return 14;
				else if ( *token == ">>" )		return 15;
				else if ( *token == "|" )		return 16;
				else if ( *token == "&" )		return 17;
				else if ( *token == "^" )		return 18;
				else if ( *token == "REGEX" )   return 19;
				else							return 0;
			}

			/*
				Calculates an cell_data value
				Returns a proper cell_data
			*/
			cell_data evaluate_operator( cell_data*, cell_data*, uint_fast8_t );

			/*
				Evaluates mathematical/logical expressions for WHERE directive
				Returns boolean type variable
			*/
			bool evaluate_expression( std::vector<std::string*>&, uint_fast64_t );

			/*
				Regular expression validator
			*/
			inline std::string validate_regex( const std::string& input ) {
				try {
					std::regex regexPattern( input );
					return input;
				} catch ( std::regex_error& ) {
					evaluation_regex_warning = true;
					return ".*";
				}
			}

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
			void APPEND_query( std::vector <std::string*>&, const uint_fast64_t );

			void INSERT_query( std::vector<std::string*>&, const uint_fast64_t );
			void CREATE_query( std::vector<std::string*>&, const uint_fast64_t );
			void DELETE_query( std::vector<std::string*>&, const uint_fast64_t );
			void SAVE_query( std::vector<std::string*>&, const uint_fast64_t );
			
			void DISTINCT_query( std::vector<std::string*>&, const uint_fast64_t );
	};
}