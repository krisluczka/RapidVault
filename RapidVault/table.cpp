#include "table.h"

namespace rv {
	table::table( const table& other ) {
		if ( this != &other ) {
			this->name = other.name;

			column_data* cd( nullptr );
			for ( column_whole cw : data ) {
				cd = std::get<1>( cw );
				for ( cell_data* dc : *cd ) {
					delete dc;
				}
				delete cd;
			}

			this->data.clear();
			// deep copy
			std::string n;
			column_data* cd_ptr( nullptr );
			for ( column_whole cw : other.data ) {
				// column name copy
				n = static_cast<std::string>(std::get<0>( cw ));

				// pointer for new column_data
				cd_ptr = std::get<1>( cw );

				// new column_data
				cd = new column_data;
				cd->reserve( cd_ptr->size() );

				// copying the cell_data in column_data
				for ( cell_data* ptr : *cd_ptr ) {
					cd->push_back( new cell_data( *ptr ) );
				}

				// tupling
				data.push_back( std::make_tuple( n, cd ) );
			}
		}
	}

	table& table::operator=( const table& other ) {
		if ( this != &other ) {
			this->name = other.name;

			column_data* cd( nullptr );
			for ( column_whole cw : data ) {
				cd = std::get<1>( cw );
				for ( cell_data* dc : *cd ) {
					delete dc;
				}
				delete cd;
			}

			this->data.clear();
			// deep copy
			std::string n;
			column_data* cd_ptr( nullptr );
			for ( column_whole cw : other.data ) {
				// column name copy
				n = static_cast<std::string>(std::get<0>( cw ));

				// pointer for new column_data
				cd_ptr = std::get<1>( cw );

				// new column_data
				cd = new column_data;
				cd->reserve( cd_ptr->size() );

				// copying the cell_data in column_data
				for ( cell_data* ptr : *cd_ptr ) {
					cd->push_back( new cell_data( *ptr ) );
				}

				// tupling
				data.push_back( std::make_tuple( n, cd ) );
			}
		}
		return *this;
	}

	table::~table() {
		column_data* cd( nullptr );
		cell_data* dc( nullptr );
		for ( uint_fast64_t i( 0 ); i < data.size(); ++i ) {
			cd = std::get<1>( data[i] );
			for ( uint_fast64_t j( 0 ); j < cd->size(); ++j ) {
				dc = cd->at( j );
				delete dc;
			}
			delete cd;
		}
	}

	uint_fast64_t table::create_column( std::string name, uint_fast64_t index ) {
		// checking the names
		for ( column_whole cw : data ) {
			// if such column already exists
			if ( std::get<0>( cw ) == name ) {
				// force it
				create_column( name + "-", index );
				return NULL64_INDEX;
			}
		}
		/*
			We need to check how many empty cells we need to create.
			Whenever we're creating a new column to a table with
			existing records, there should be exactly the same amount
			of cells, as there are records.
		*/
		uint_fast64_t rows( 0 );
		if ( data.size() != 0 ) {
			column_data* cd( std::get<1>( data[0] ) );
			rows = cd->size();
		}

		column_data* column( new column_data );
		cell_data* dc;
		// creating the empty cells
		for ( uint_fast64_t i( 0 ); i < rows; ++i ) {
			dc = new cell_data;
			column->push_back( dc );
		}

		if ( index == NULL64_INDEX ) {
			// inserting at the back
			data.push_back( std::tuple( name, column ) );
			return 0;
		} else {
			// inserting at given index
			data.insert( data.begin() + index, std::tuple( name, column ) );
			return index;
		}
	}

	uint_fast64_t table::delete_column( uint_fast64_t identifier ) {
		if ( identifier < data.size() ) {
			// don't forget about this!
			column_data* cd( std::get<1>( data[identifier] ) );
			for ( cell_data* dc : *cd ) {
				delete dc;
			}
			delete cd;
			data.erase( data.begin() + identifier );
			return identifier;
		}

		return NULL64_INDEX;
	}

	uint_fast64_t table::rename_column( uint_fast64_t identifier, std::string new_name ) {
		if ( identifier < data.size() )
			std::get<0>( data[identifier] ) = new_name;

		return identifier;
	}

	uint_fast64_t table::get_column_index( std::string name ) const {
		for ( uint_fast64_t i( 0 ); i < data.size(); ++i ) {
			if ( std::get<0>( data[i] ) == name ) {
				return i;
				break;
			}
		}
		return NULL64_INDEX;
	}

	uint_fast64_t table::create_row() {
		if ( data.size() ) {
			column_data* cd( std::get<1>( data[0] ) );
			cell_data* dc( nullptr );
			// filling row with zeroes
			for ( uint_fast64_t column_index( 0 ); column_index < data.size(); ++column_index ) {
				cd = std::get<1>( data[column_index] );
				dc = new cell_data( 0 );
				cd->push_back( dc );
			}

			return cd->size() - 1;
		} else return NULL64_INDEX;
	}

	void table::change_cell( uint_fast64_t index, uint_fast64_t identifier, cell_data d ) {
		if ( data.size() ) {
			// checking the amount of rows (based on first column)
			column_data* cd( std::get<1>( data[0] ) );
			uint_fast64_t rows( cd->size() );
			// checking the row identifier (only when the row exists)
			if ( index < rows )
				if ( identifier < data.size() ) {
					cd = std::get<1>( data[identifier] );
					*cd->at( index ) = d;
				}
		}
	}

	void table::change_cell( uint_fast64_t index, std::string identifier, cell_data d ) {
		uint_fast64_t identi( get_column_index( identifier ) );
		if ( data.size() ) {
			// checking the amount of rows (based on first column)
			column_data* cd( std::get<1>( data[0] ) );
			uint_fast64_t rows( cd->size() );
			// checking the row identifier (only when the row exists)
			if ( index < rows )
				if ( identi < data.size() ) {
					cd = std::get<1>( data[identi] );
					*cd->at( index ) = d;
				}
		}
	}

	cell_data table::get_cell( uint_fast64_t index, uint_fast64_t identifier ) {
		cell_data d( 0 );
		if ( data.size() ) {
			// checking the amount of rows (based on first column)
			column_data* cd( std::get<1>( data[0] ) );
			uint_fast64_t rows( cd->size() );
			// checking the row identifier (only when the row exists)
			if ( identifier < data.size() && index < rows ) {
				cd = std::get<1>( data[identifier] );
				d = *cd->at( index );
			}
		}

		return d;
	}

	uint_fast64_t table::delete_row( uint_fast64_t index ) {
		uint_fast64_t rows( NULL64_INDEX );
		if ( data.size() ) {
			// checking the amount of rows (based on first column)
			column_data* cd( std::get<1>( data[0] ) );
			rows = cd->size();
			// removing specified row
			if ( index < rows ) {
				cell_data* dc;
				for ( column_whole cw : data ) {
					cd = std::get<1>( cw );
					dc = cd->at( index );
					delete dc;
					cd->erase( cd->begin() + index );
				}
				--rows;
			}
		}
		return rows;
	}

	uint_fast64_t table::display( DISPLAY_TYPE type ) const {
		uint_fast64_t rows( 0 );
		if ( data.size() ) {
			// checking the amount of rows (based on first column)
			column_data* cd( std::get<1>( data[0] ) );
			rows = cd->size();
			// normal, report style
			if ( type == DISPLAY_TYPE::NORMAL ) {
				// searching for the longest string in every column and it's name
				std::vector<uint_fast64_t> column_width( data.size(), 0 );
				std::stringstream parser;
				std::string string_parser( "" );

				for ( uint_fast64_t column_index( 0 ); column_index < data.size(); ++column_index ) {
					// checking whether the column's name is the longest
					string_parser = std::get<0>( data[column_index] );
					column_width[column_index] = std::max( column_width[column_index], string_parser.length() );
			
					for ( uint_fast64_t row( 0 ); row < rows; ++row ) {
						// pointer to current column data
						cd = std::get<1>( data[column_index] );

						// using stringstream because we need to check for its length in characters
						std::visit( [&parser, &string_parser, &column_width, &column_index]( auto arg ) {
							parser << arg;
							string_parser = parser.str();
							column_width[column_index] = std::max( column_width[column_index], string_parser.length() );
							parser.str( "" );
						}, *cd->at( row ) );
					}
				}

				// drawing the column's names
				std::cout << 'o';
				for ( uint_fast64_t cw( 0 ); cw < column_width.size(); ++cw ) {
					std::cout << std::string( column_width[cw] + 2, '-' ) << 'o';
				}
				std::cout << "\n|";
				for ( uint_fast64_t column_index( 0 ); column_index < data.size(); ++column_index ) {
					string_parser = std::get<0>( data[column_index] );
					std::cout << " " << std::left << std::setw( column_width[column_index] ) << string_parser << " |";
				}
				std::cout << "\n";

				// drawing the table values
				std::cout << '+';
				for ( uint_fast64_t cw( 0 ); cw < column_width.size(); ++cw ) {
					std::cout << std::string( column_width[cw] + 2, '-' ) << '+';
				}
				std::cout << "\n";
				for ( uint_fast64_t row( 0 ); row < rows; ++row ) {
					std::cout << "|";
					for ( uint_fast64_t column_index( 0 ); column_index < data.size(); ++column_index ) {
						// pointer to current column data
						cd = std::get<1>( data[column_index] );

						// displaying data
						std::visit( [&column_index, &column_width]( auto arg ) {
							std::cout << " " << std::left << std::setw(column_width[column_index]) << arg << " |";
						}, *cd->at( row ) );
					}
					std::cout << "\n+";
					for ( uint_fast64_t cw( 0 ); cw < column_width.size(); ++cw ) {
						std::cout << std::string( column_width[cw] + 2, '-' ) << '+';
					}
					std::cout << "\n";
				}
			}
			// raw style
			else if ( type == DISPLAY_TYPE::RAW ) {
				for ( uint_fast64_t row( 0 ); row < rows; ++row ) {
					for ( uint_fast64_t column_index( 0 ); column_index < data.size(); ++column_index ) {
						// pointer to current column data
						cd = std::get<1>( data[column_index] );

						// displaying data
						std::visit( [this, &column_index]( auto arg ) {
							if ( column_index == data.size() - 1 ) std::cout << arg;
							else std::cout << arg << " ";
						}, *cd->at( row ) );
					}
					std::cout << std::endl;
				}
			}
			// json (by rows) style
			else if ( type == DISPLAY_TYPE::JSON ) {
				cell_data* dc( nullptr );
				uint_fast64_t columns( data.size() );
				uint_fast64_t max_columns( 0 );
				std::cout << "{\n";
				for ( uint_fast64_t row( 0 ); row < rows; ++row ) {
					std::cout << "    " << row << ": [ ";
					for ( uint_fast64_t column_index( 0 ); column_index < columns; ++column_index ) {
						// pointer to current column data
						cd = std::get<1>( data[column_index] );
						dc = cd->at( row );

						// displaying data
						if ( std::holds_alternative<int_fast64_t>( *dc ) )
							std::cout << std::get<int_fast64_t>( *dc );
						else if ( std::holds_alternative<long double>( *dc ) )
							std::cout << std::get<long double>( *dc );
						else
							std::cout << "\"" << std::get<std::string>( *dc ) << "\"";
						
						if ( max_columns < columns - 1 ) std::cout << ", ";
						++max_columns;
					}
					max_columns = 0;
					std::cout << " ],\n";
				}
				std::cout << "}" << std::endl;
			}
			// inverted json (by columns) style
			else if ( type == DISPLAY_TYPE::JSON_INVERTED ) {
				uint_fast64_t max_rows( 0 );
				std::cout << "{\n";
				for ( uint_fast64_t column_index( 0 ); column_index < data.size(); ++column_index ) {
					std::cout << "    \"" << std::get<0>( data[column_index] ) << "\": [ ";
					cd = std::get<1>( data[column_index] );
					for ( cell_data* dc : *cd ) {
						if ( std::holds_alternative<int_fast64_t>( *dc ) )
							std::cout << std::get<int_fast64_t>( *dc );
						else if ( std::holds_alternative<long double>( *dc ) )
							std::cout << std::get<long double>( *dc );
						else
							std::cout << "\"" << std::get<std::string>( *dc ) << "\"";
						if ( max_rows < rows - 1 ) std::cout << ", ";
						++max_rows;
					}
					max_rows = 0;
					std::cout << " ],\n";
				}
				std::cout << "}" << std::endl;
			}
		}
		return rows;
	}
}