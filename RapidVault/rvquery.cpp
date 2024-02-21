/*
	RVquery is a part of the RapidVault project,
	and it's distributed under the same MIT license.



	#	RVquery small documentation		#


	Queries are written by singular commands separated by a semicolon.
	For example:
		"command arg arg; command; command arg;"

	For each query, an environment (operation table) is created in which
    the result is stored. So when the query looks like this:
	'JOIN' spits out both tables joined together according to
    the specified relation rules. Adding additional commands in
	the same query after the 'JOIN' would do the operations on merged table.

	! WARNING !
	When merging tables, the columns will appear with the name
		"table_name.column_name"
	instead of simple "column_name"

		<command> - <description>
			<example of use>



	>	Operating queries	<

        SELECT <table> - selects the table as the primary operational table
            SELECT main;

		JOIN <main column> <relation> <table column> <table> - selects given tables
        and joins them to the operational table
			JOIN main.id N1 id users

            - 11 means one-to-one relation
            - 1N means one-to-many relation
            - N1 means many-to-one relation
            - NN means many-to-many relation

		PICK <col1> <col2> ... - selects given columns, discarding others
		from the final result
			PICK users.name main.title;

		WHERE <expression> - selects only these rows, where the expression is true
		! WARNING ! Expressions use Reverse Polish Notation
			WHERE main.age users.age > main.experience users.experience > &&;

		UPDATE <column> <value> - changes every value in the given column
			UPDATE users.age 18;



	>	Manipulating queries	<

		INSERT



    >   WHERE expressions   <
*/
#include "database.h"
#define isOperator(token) (token == "+" || token == "-" || token == "*" || token == "/" || token == "^" || token == "&&" || token == "||" || token == "<" || token == "<=" || token == ">" || token == ">=" || token == "==" || token == "!=" )

namespace rv {
    bool database::expression_rpn( std::string expression ) {
        std::istringstream iss( expression );
        std::stack<double> values;
        std::string token;

        while ( iss >> token ) {
            // real numbers check
            if ( isdigit( token[0] ) || (token[0] == '-' && isdigit( token[1] )) ) {
                values.push( std::stod( token ) );
            }
            // checking if we're dealing with any operator
            else if ( isOperator( token ) ) {
                if ( values.size() < 2 ) {
                    //std::cerr << "Invalid expression format!" << std::endl;
                    return NAN;
                }
                double val2 = values.top();
                values.pop();
                double val1 = values.top();
                values.pop();
                double result;
                if ( token == "+" ) {
                    result = val1 + val2;
                } else if ( token == "-" ) {
                    result = val1 - val2;
                } else if ( token == "*" ) {
                    result = val1 * val2;
                } else if ( token == "/" ) {
                    if ( val2 == 0 ) {
                        //std::cerr << "Division by zero error!" << std::endl;
                        return NAN;
                    }
                    result = val1 / val2;
                } else if ( token == "^" ) {
                    result = pow( val1, val2 );
                } else if ( token == "&&" ) {
                    result = val1 && val2;
                } else if ( token == "||" ) {
                    result = val1 || val2;
                } else if ( token == "<" ) {
                    result = val1 < val2;
                } else if ( token == "<=" ) {
                    result = val1 <= val2;
                } else if ( token == ">" ) {
                    result = val1 > val2;
                } else if ( token == ">=" ) {
                    result = val1 >= val2;
                } else if ( token == "==" ) {
                    result = val1 == val2;
                } else if ( token == "!=" ) {
                    result = val1 != val2;
                }
                values.push( result );
            } else {
                //std::cerr << "Invalid token: " << token << std::endl;
                return NAN;
            }
        }

        if ( values.size() != 1 ) {
            //std::cerr << "Invalid expression format!" << std::endl;
            return NAN;
        }

        return values.top();
    }

	void database::rvquery( std::string query ) {
        // reseting the query table
        delete operation_table;
        operation_table = new table;

        // dividing the query into lines ending with a semicolon
        std::vector<std::string*> lines;
        std::istringstream q(query);
        std::string line;
        while ( std::getline( q, line, ';' ) ) {
            lines.push_back( new std::string( line ) );
        }

        std::vector<std::string*> tokens;
        for ( std::string* l : lines ) {
            // dividing every line into tokens
            tokens.clear();
            std::istringstream iss( *l );
            std::string token;
            while ( iss >> token ) {
                tokens.push_back( new std::string( token ) );
            }

            // evaluating the RVquery
            go_go_gadget_query( tokens );

            // detokenizing :D
            for ( std::string* token : tokens ) {
                delete token;
            }
        }

        // delining :D
        for ( std::string* line : lines ) {
            delete line;
        }

        // showing results
        operation_table->display();
	}

    void database::go_go_gadget_query( std::vector<std::string*>& tokens ) {
        if ( tokens.size() ) { // ignoring empty lines
            if ( *tokens[0] == "SELECT" ) {
                if ( tokens.size() > 1 ) {
                    uint_fast64_t index = get_table_index( *tokens[1] );
                    if ( index != NULL64_INDEX ) {
                        // copy, if exists
                        *operation_table = *tables[index];
                        // renaming the columns to 'table.column'
                        for ( uint_fast16_t i = 0; i < operation_table->data.size(); i++ ) {
                            std::get<0>( operation_table->data[i] ) = operation_table->name + '.' + std::get<0>(operation_table->data[i]);
                        }
                    } else {
                        std::cerr << "ERR: Invalid table name!" << std::endl;
                    }
                } else {
                    std::cerr << "ERR: Not enough arguments!" << std::endl;
                }
            } else if ( *tokens[0] == "JOIN" ) {
                /*
                    DONT USE IT YET !!! NOI DEBUGGED !
                */
                if ( tokens.size() > 4 ) {
                    // if 'SELECT' occured
                    if ( operation_table->data.size() ) {
                        // decoding the tokens
                        uint_fast16_t main_column_index = operation_table->get_column_index( *tokens[1] );
                        uint_fast16_t main_column_amount = operation_table->data.size();
                        uint_fast64_t oti = get_table_index( *tokens[4] );
                    
                        // if the table exists
                        if ( oti != NULL64_INDEX ) {
                            // further token decoding
                            table* other_table = tables[oti];
                            uint_fast16_t other_column_index = other_table->get_column_index( *tokens[3] );
                            uint_fast16_t other_column_amount = other_table->data.size();
                            // if the columns exist
                            if ( (main_column_index != NULL16_INDEX) && (other_column_index != NULL16_INDEX) ) {
                                // table rows
                                uint_fast64_t main_rows = std::get<1>( operation_table->data[0] )->size();
                                uint_fast64_t other_rows = std::get<1>( other_table->data[0] )->size();
                                // pointer to any column data
                                column_data* main_cd = nullptr;
                                column_data* other_cd = nullptr;

                                // many-to-one relation
                                if ( *tokens[2] == "N1" ) {
                                    // operation table column data
                                    main_cd = std::get<1>( operation_table->data[main_column_index] );
                                    other_cd = std::get<1>( other_table->data[other_column_index] );
                                    // merging two tables with new names
                                    std::string column_name = "";
                                    for ( column_whole cw : other_table->data ) {
                                        column_name = other_table->name + '.' + std::get<0>( cw );
                                        operation_table->create_column( column_name );
                                    }

                                    // checking the specified relation
                                    for ( uint_fast64_t mrow = 0; mrow < main_rows; mrow++ ) {
                                        for ( uint_fast64_t orow = 0; orow < other_rows; orow++ ) {
                                            // if there is a match
                                            if ( *main_cd->at( mrow ) == *other_cd->at( orow ) ) {
                                                // copy the values to the main table
                                                for ( uint_fast16_t col = 0; col < other_column_amount; col++ ) {
                                                    main_cd = std::get<1>( operation_table->data[main_column_amount + col] );
                                                    other_cd = std::get<1>( other_table->data[col] );
                                                    *main_cd->at( mrow ) = *other_cd->at( orow );
                                                }
                                                // switching back to the compared columns
                                                main_cd = std::get<1>( operation_table->data[main_column_index] );
                                                other_cd = std::get<1>( other_table->data[other_column_index] );
                                            }
                                        }
                                    }

                                    // deleting the columns on which the JOIN was based
                                    other_column_index += main_column_amount;
                                    operation_table->delete_column( other_column_index );
                                    operation_table->delete_column( main_column_index );
                                }
                            } else {
                                std::cerr << "ERR: Invalid column name!" << std::endl;
                            }
                        } else {
                            std::cerr << "ERR: Invalid table name!" << std::endl;
                        }
                    } else {
                        std::cerr << "ERR: Didn't specify a starting table!" << std::endl;
                    }
                } else {
                    std::cerr << "ERR: Not enough arguments!" << std::endl;
                }
            } else if ( *tokens[0] == "PICK" ) {

            } else if ( *tokens[0] == "WHERE" ) {

            } else if ( *tokens[0] == "UPDATE" ) {

            }
        }
    }
}