#include "RapidVault.h"

namespace rv {
	bool RapidVault::load( std::string path ) {
		// opening a file
		std::ifstream database_file( path );

		// if it wasn't opened, return false
		if ( !database_file.is_open() ) return false;

		// we good
		database* db( new database );
		std::string line;
		std::string token;

		// checking database name
		if ( getline( database_file, line ) )
			db->name = line;
		else {
			// !!!
			delete db;
			return false;
		}

		table* t( nullptr );
		// fetching table names and their columns
		while ( getline( database_file, line ) ) {
			t = new table;
			std::istringstream iss( line );

			// if line is valid
            if ( iss >> token )
                t->name = token;
			else {
                // !!!
				delete t;
				continue;
			}

			// columns names fetching
			while ( iss >> token )
				t->create_column( token );

            // searching for table's rows file
            std::ifstream table_file( db->name + "." + t->name + ".rv", std::ios::binary );

            // if there are no rows' file
            if ( !table_file.is_open() ) continue;

            // reading binary file to specified table
            if ( !load_table( &table_file, t ) ) {
                delete t;
                continue;
            }

            // pushing the table to a database
            table_file.close();
			db->tables.push_back( t );
		}

		databases.push_back( db );
		database_file.close();
		return true;
	}

    bool RapidVault::save( uint_fast64_t index ) {
        // finding a database
        std::string name( "" );
        if ( index < databases.size() )
            name = databases[index]->name;
        else
            return false;

        // opening a database file, if not exists just create one
        std::ofstream database_file( name + ".rv", std::ios::out | std::ios::trunc );
        
        // if it wasn't opened, return false
        if ( !database_file.is_open() ) return false;

        // pushing database name
        database_file << name << "\n";

        std::string table_name( "" );
        // saving the tables
        for ( table* t : databases[index]->tables ) {
            table_name = t->name;
            // opening the table file
            std::ofstream table_file( name + "." + table_name + ".rv",  std::ios::out | std::ios::binary | std::ios::trunc );

            // data writing
            save_table( &table_file, t );

            // pushing table name
            database_file << table_name << " ";

            // columns' names
            for ( column_whole cw : t->data ) {
                database_file << std::get<0>( cw ) << " ";
            }
            database_file << "\n";
        }

        // flushing the buffer
        database_file << std::endl;

        return true;
    }

    bool RapidVault::save_table( std::ofstream* file, table* t ) {
        uint_fast64_t columns( t->data.size() );
        // return false if there is not enough columns
        if ( !columns ) return false;
        // amount of rows
        uint_fast64_t rows( std::get<1>( t->data[0] )->size() );

        // helper variables
        std::string string_value( "" );
        int_fast64_t int_value( 0 );
        long double float_value( 0 );
        uint_fast32_t length( 0 );
        char the_byte( 0 );
        cell_data dc( 0 );

        for ( uint_fast64_t row( 0 ); row < rows; ++row ) {
            for ( uint_fast64_t column( 0 ); column < columns; ++column ) {
                // the cell
                dc = t->get_cell( row, column );

                // string saving
                if ( std::holds_alternative<std::string>( dc ) ) {
                    string_value = std::get<std::string>( dc );
                    // the byte
                    the_byte = 0;
                    file->write( &the_byte, sizeof( char ) );

                    // the size
                    length = static_cast<uint_fast32_t>(string_value.length());
                    file->write( reinterpret_cast<const char*>(&length), sizeof( length ) );

                    // the string
                    file->write( string_value.c_str(), length );
                }
                // int saving
                else if ( std::holds_alternative<int_fast64_t>( dc ) ) {
                    // the byte
                    the_byte = 1;
                    file->write( &the_byte, sizeof( char ) );

                    // the data
                    int_value = std::get<int_fast64_t>( dc );
                    file->write( reinterpret_cast<const char*>(&int_value), sizeof(int_value) );
                }
                // float saving
                else if ( std::holds_alternative<long double>( dc ) ) {
                    // the byte
                    the_byte = 2;
                    file->write( &the_byte, sizeof( char ) );

                    // the data
                    float_value = std::get<long double>( dc );
                    file->write( reinterpret_cast<const char*>(&float_value), sizeof(float_value) );
                } else
                    return false;
            }
        }
        return true;
    }

    bool RapidVault::load_table( std::ifstream* file, table* t ) {
        uint_fast64_t columns( t->data.size() );
        // return false if there is not enough columns
        if ( !columns ) return false;

        bool eofReached( false );
        char firstByte( 0 );

        uint_fast64_t column( 0 );
        uint_fast64_t row( 0 );

        // helper variables
        std::string string_value( "" );
        int_fast64_t int_value( 0 );
        long double float_value( 0 );
        uint_fast32_t length( 0 );

        // looping through every saved data
        while ( !eofReached ) {
            // first byte triggers what data type we're currently managing
            if ( !file->read( &firstByte, sizeof( firstByte ) ) ) {
                eofReached = true;
                break;
            }

            // every new iteration we create a new row
            if ( column == 0 ) t->create_row();

            switch ( firstByte ) {
                // string case
                case 0:
                    // another 4 bytes are the length of a string (since we don't know its size)
                    file->read( reinterpret_cast<char*>(&length), sizeof( length ) );

                    // reading the data
                    string_value.resize( length );
                    file->read( &string_value[0], length );

                    // pushing the data to the column
                    t->change_cell(row, column, string_value);

                    break;
                // integer case
                case 1:
                    file->read( reinterpret_cast<char*>(&int_value), sizeof( int_value ) );

                    // pushing the data to the column
                    t->change_cell( row, column, int_value );

                    break;
                // floating point case
                case 2:
                    file->read( reinterpret_cast<char*>(&float_value), sizeof( float_value ) );

                    // pushing the data to the column
                    t->change_cell( row, column, float_value );

                    break;
                default:
                    return false;
                    break;
            }

            ++column;

            if ( column == columns ) {
                column = 0;
                ++row;
            }
        }

        return true;
    }
}