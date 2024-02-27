/*
	Single table class
*/
#pragma once
#include <iostream>
#include <variant>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#define NULL64_INDEX 18446744073709551615
#define NULL16_INDEX 65535

namespace rv {
	// table displaying types
	enum DISPLAY_TYPE : uint_fast8_t {
		NORMAL,
		RAW,
		JSON,
		JSON_INVERTED,
		NONE,
	};

	// Cell data
	typedef std::variant<int_fast64_t, long double, std::string> cell_data;
	// Column data
	typedef std::vector<cell_data*> column_data;
	// Whole column (column name, column type, pointer to column)
	typedef std::tuple<std::string, column_data*> column_whole;
	// Table data set of columns
	typedef std::vector<column_whole> table_data;

	class table {
		friend class database;
		public:
			table( std::string );
			table();
			table( const table& );
			table& operator=( const table& );
			~table();



			/*
				###############################
					Column manipulating
				###############################
			*/
			/*
				Creates a new column of specified type and name
				Returns its index
			*/
			uint_fast16_t create_column( std::string, uint_fast16_t index = NULL16_INDEX );

			/*
				Deletes specified column
				Returns new amount of columns
			*/
			uint_fast16_t delete_column( uint_fast16_t );

			/*
				Renames specified column
				Returns its index
			*/
			uint_fast16_t rename_column( uint_fast16_t, std::string );

			/*
				Returns column's index
			*/
			uint_fast16_t get_column_index( std::string ) const;


			/*
				###############################
					Rows manipulating
				###############################
			*/
			/*
				Creates a row (empty)
				Returns its index
			*/
			uint_fast64_t create_row();

			/*
				Changes values of given row
			*/
			void change_row( uint_fast64_t, uint_fast16_t, cell_data );
			void change_row( uint_fast64_t, std::string, cell_data );

			/*
				Returns a row value
			*/
			cell_data get_row( uint_fast64_t, uint_fast16_t );

			/*
				###############################
					Displaying results
				###############################
			*/
			/*
				Displays full table and it's values
				Returns number of rows
			*/
			uint_fast64_t display( DISPLAY_TYPE ) const;

		private:			
			/*
				Table name
			*/
			std::string name;

			/*
				Data stored in the table
			*/
			table_data data;
	};
}