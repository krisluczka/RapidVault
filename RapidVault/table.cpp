#include "table.h"

namespace rv {
	table::table( std::string table_name ) {
		name = table_name;
	}

	table::table() {
		name = "rapidvault_table";
	}

	table::table( const table& other ) {
		if ( this != &other ) {
			this->name = other.name;

			this->data.clear();
			// deep copy
			for ( const auto& tuple : other.data ) {
				// column name copy
				std::string n = std::get<0>( tuple );

				// pointer for old column_data
				const column_data* cd_ptr = std::get<1>( tuple );

				// new column_data
				column_data* cd = new column_data;
				cd->reserve( cd_ptr->size() );

				// copying the cell_data in column_data
				for ( cell_data* ptr : *cd_ptr ) {
					cd->push_back( new cell_data( *ptr ) );
				}

				// tupling
				this->data.push_back( std::make_tuple( n, cd ) );
			}
		}
	}

	table& table::operator=( const table& other ) {
		if ( this != &other ) {
			this->name = other.name;

			this->data.clear();
			// deep copy
			for ( const auto& tuple : other.data ) {
				// column name copy
				std::string n = std::get<0>( tuple );

				// pointer for old column_data
				const column_data* cd_ptr = std::get<1>( tuple );
				
				// new column_data
				column_data* cd = new column_data;
				cd->reserve( cd_ptr->size() );

				// copying the cell_data in column_data
				for ( cell_data* ptr : *cd_ptr ) {
					cd->push_back( new cell_data( *ptr ) );
				}

				// tupling
				this->data.push_back( std::make_tuple( n, cd ) );
			}
		}
		return *this;
	}

	table::~table() {
		column_data* cd;
		cell_data* dc;
		for ( uint_fast16_t i = 0; i < data.size(); i++ ) {
			cd = std::get<1>( this->data[i] );
			for ( uint_fast64_t j = 0; j < cd->size(); j++ ) {
				dc = cd->at( j );
				delete dc;
			}
			delete cd;
		}
	}

	uint_fast16_t table::create_column( std::string name, uint_fast16_t index ) {
		/*
			We need to check how many empty cells we need to create.
			Whenever we're creating a new column to a table with
			existing records, there should be exactly the same amount
			of cells, as there are records.
		*/
		uint_fast64_t rows = 0;
		if ( data.size() != 0 ) {
			column_data* cd = std::get<1>( data[0] );
			rows = cd->size();
		}

		column_data* column = new column_data;
		cell_data* cd;
		// creating the empty cells
		for ( uint_fast64_t i = 0; i < rows; i++ ) {
			cd = new cell_data;
			column->push_back( cd );
		}

		if ( index == NULL16_INDEX ) {
			// inserting at the back
			data.push_back( std::tuple( name, column ) );
			return 0;
		} else {
			// inserting at given index
			data.insert( data.begin() + index, std::tuple( name, column ) );
			return index;
		}
	}

	uint_fast16_t table::delete_column( std::variant<uint_fast16_t, std::string> identifier ) {
		uint_fast16_t index = NULL16_INDEX;

		// checking the column identifier
		std::visit( [this, &index]( auto arg ) {
			if constexpr ( std::is_same_v<std::decay_t<decltype(arg)>, uint_fast16_t> ) {
				index = arg;

				if ( (index != NULL16_INDEX) && (index < this->data.size()) ) {
					// don't forget about this!
					column_data* cd = std::get<1>( this->data[index] );
					delete cd;
					this->data.erase( this->data.begin() + index );
				}
			} else if constexpr ( std::is_same_v<std::decay_t<decltype(arg)>, std::string> ) {
				const std::string name = arg;

				for ( uint_fast16_t i = 0; i < this->data.size(); i++ ) {
					if ( std::get<0>( this->data[i] ) == name ) {
						index = i;

						// don't forget about this!
						column_data* cd = std::get<1>( this->data[index] );
						delete cd;
						this->data.erase( this->data.begin() + index );
						break;
					}
				}
			}
		}, identifier );

		return index;
	}

	uint_fast16_t table::rename_column( std::variant<uint_fast16_t, std::string> identifier, std::string new_name ) {
		uint_fast16_t index = NULL16_INDEX;

		// checking the column identifier
		std::visit( [this, &new_name, &index]( auto arg ) {
			if constexpr ( std::is_same_v<std::decay_t<decltype(arg)>, uint_fast16_t> ) {
				index = arg;

				if ( (index != NULL16_INDEX) && (index < this->data.size()) )
					std::get<0>( this->data[index] ) = new_name;

			} else if constexpr ( std::is_same_v<std::decay_t<decltype(arg)>, std::string> ) {
				const std::string name = arg;
				for ( uint_fast16_t i = 0; i < this->data.size(); i++ ) {
					if ( std::get<0>( this->data[i] ) == name ) {
						index = i;

						std::get<0>( this->data[i] ) = new_name;
						break;
					}
				}
			}
		}, identifier );

		return index;
	}

	uint_fast16_t table::get_column_index( std::string name ) const {
		uint_fast16_t index = NULL16_INDEX;
		for ( uint_fast16_t i = 0; i < data.size(); i++ ) {
			if ( std::get<0>( data[i] ) == name ) {
				index = i;
				break;
			}
		}
		return index;
	}

	uint_fast64_t table::create_row() {
		if ( data.size() ) {
			column_data* cd = std::get<1>( data[0] );

			// filling row with zeroes
			for ( uint_fast16_t column_index = 0; column_index < data.size(); column_index++ ) {
				cd = std::get<1>( data[column_index] );
				cell_data* d = new cell_data( 0 );
				cd->push_back( d );
			}

			return cd->size();
		} else return NULL64_INDEX;
	}

	void table::change_row( uint_fast64_t index, std::variant<uint_fast16_t, std::string> identifier, cell_data d ) {
		if ( data.size() ) {
			// checking the amount of rows (based on first column)
			column_data* cd = std::get<1>( this->data[0] );
			uint_fast64_t rows = cd->size();
			// checking the row identifier (only when the row exists)
			if ( index < rows )
			std::visit( [this, &cd, &d, &index]( auto arg ) {
				if constexpr ( std::is_same_v<std::decay_t<decltype(arg)>, uint_fast16_t> ) {
					if ( (arg != NULL16_INDEX) && (arg < this->data.size()) ) {
						cd = std::get<1>( this->data[arg] );
						*cd->at( index ) = d;
					}
				} else if constexpr ( std::is_same_v<std::decay_t<decltype(arg)>, std::string> ) {
					const std::string name = arg;
					for ( uint_fast16_t i = 0; i < this->data.size(); i++ ) {
						if ( std::get<0>( this->data[i] ) == name ) {
							cd = std::get<1>( this->data[i] );
							*cd->at( index ) = d;
							break;
						}
					}
				}
			}, identifier );
		}
	}

	cell_data table::get_row( uint_fast64_t index, std::variant<uint_fast16_t, std::string> identifier ) {
		cell_data d = 0;
		if ( data.size() ) {
			// checking the amount of rows (based on first column)
			column_data* cd = std::get<1>( this->data[0] );
			uint_fast64_t rows = cd->size();
			// checking the row identifier (only when the row exists)
			if ( index < rows )
				std::visit( [this, &cd, &d, &index]( auto arg ) {
				if constexpr ( std::is_same_v<std::decay_t<decltype(arg)>, uint_fast16_t> ) {
					if ( (arg != NULL16_INDEX) && (arg < this->data.size()) ) {
						cd = std::get<1>( this->data[arg] );
						d = *cd->at( index );
					}
				} else if constexpr ( std::is_same_v<std::decay_t<decltype(arg)>, std::string> ) {
					const std::string name = arg;
					for ( uint_fast16_t i = 0; i < this->data.size(); i++ ) {
						if ( std::get<0>( this->data[i] ) == name ) {
							cd = std::get<1>( this->data[i] );
							d = *cd->at( index );
							break;
						}
					}
				}
			}, identifier );
		}

		return d;
	}

	uint_fast64_t table::display() const {
		uint_fast64_t rows = 0;
		if ( data.size() ) {
			// checking the amount of rows (based on first column)
			column_data* cd = std::get<1>( data[0] );
			rows = cd->size();

			// searching for the longest string in every column and it's name
			std::vector<uint_fast64_t> column_width( data.size(), 0 );
			std::stringstream parser;
			std::string string_parser = "";

			for ( uint_fast16_t column_index = 0; column_index < data.size(); column_index++ ) {
				// checking whether the column's name is the longest
				string_parser = std::get<0>( data[column_index] );
				column_width[column_index] = std::max( column_width[column_index], string_parser.length() );
			
				for ( uint_fast64_t row = 0; row < rows; row++ ) {
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
			for ( uint_fast64_t cw = 0; cw < column_width.size(); cw++ ) {
				std::cout << std::string( column_width[cw] + 2, '-' ) << 'o';
			}
			std::cout << "\n|";
			for ( uint_fast16_t column_index = 0; column_index < data.size(); column_index++ ) {
				string_parser = std::get<0>( data[column_index] );
				std::cout << " " << std::left << std::setw( column_width[column_index] ) << string_parser << " |";
			}
			std::cout << "\n";

			// drawing the table values
			std::cout << '+';
			for ( uint_fast64_t cw = 0; cw < column_width.size(); cw++ ) {
				std::cout << std::string( column_width[cw] + 2, '-' ) << '+';
			}
			std::cout << "\n";
			for ( uint_fast64_t row = 0; row < rows; row++ ) {
				std::cout << "|";
				for ( uint_fast16_t column_index = 0; column_index < data.size(); column_index++ ) {
					// pointer to current column data
					cd = std::get<1>( data[column_index] );

					// displaying data
					std::visit( [&column_index, &column_width]( auto arg ) {
						std::cout << " " << std::left << std::setw(column_width[column_index]) << arg << " |";
					}, *cd->at( row ) );
				}
				std::cout << "\n+";
				for ( uint_fast64_t cw = 0; cw < column_width.size(); cw++ ) {
					std::cout << std::string( column_width[cw] + 2, '-' ) << '+';
				}
				std::cout << "\n";
			}
		}
		return rows;
	}

	uint_fast64_t table::display_raw() const {
		uint_fast64_t rows = 0;
		if ( data.size() > 0 ) {
			// checking the amount of rows (based on first column)
			column_data* cd = std::get<1>( data[0] );
			rows = cd->size();

			for ( uint_fast64_t row = 0; row < rows; row++ ) {
				for ( uint_fast16_t column_index = 0; column_index < data.size(); column_index++ ) {
					// pointer to current column data
					cd = std::get<1>( data[column_index] );

					// displaying data
					std::visit( [this, &column_index]( auto arg ) {
						if ( column_index == data.size() - 1 ) std::cout << arg << ";";
						else std::cout << arg << ",";
					}, *cd->at( row ) );
				}
				std::cout << std::endl;
			}
		}
		return rows;
	}
}

/*
	template<typename... Arguments>
	uint_fast64_t [[deprecated]]table::add_full_row( Arguments... args ) {
		// column index
		column_data* cd;
		uint_fast16_t index = 0;

		for ( const auto& value : { args... } ) {
			cd = std::get<1>( data[index] );
			cd->push_back( args );
			index++;
		}

		((
			cd = std::get<1>( data[index] );
			cd->push_back( args );
			index++;
		), ...);


		return cd->size() - 1;
	}
*/