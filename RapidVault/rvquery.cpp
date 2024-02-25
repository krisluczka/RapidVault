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
	instead of simple "column_name".



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

        ALIAS <column> <new_name> - selects given column and renames them
            ALIAS main.test Testing_column

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
#define isOperator(token) (token == "+" || token == "-" || token == "*" || token == "/" || token == "&&" || token == "||" || token == "<" || token == "<=" || token == ">" || token == ">=" || token == "==" || token == "!=" )

namespace rv {
    cell_data database::evaluate_operator( cell_data* A, cell_data* B, std::string token ) {
        long double a = 0, b = 0;
        std::string as = "", bs = "";
        bool a_string = false, b_string = false;
        cell_data result = 0;

        // getting the A
        if ( std::holds_alternative<int_fast64_t>( *A ) ) {
            a = static_cast<long double>(std::get<int_fast64_t>( *A ));
        } else if ( std::holds_alternative<long double>( *A )) {
            a = std::get<long double>( *A );
        } else {
            as = std::get<std::string>( *A );
            a_string = true;
        }

        // getting the B
        if ( std::holds_alternative<int_fast64_t>( *B ) ) {
            b = static_cast<long double>(std::get<int_fast64_t>( *B ));
        } else if ( std::holds_alternative<long double>( *B ) ) {
            b = std::get<long double>( *B );
        } else {
            bs = std::get<std::string>( *B );
            b_string = true;
        }

        // checking every type of operator
        if ( token == "+" ) {
            if ( a_string && b_string )       result = as + bs;
            else if ( !a_string && b_string ) result = std::to_string( a ) + bs;
            else if ( a_string && !b_string ) result = as + std::to_string( b );
            else                              result = a + b;
        } else if ( token == "-" ) {
            if ( a_string && b_string )       result = static_cast<int_fast64_t>(as.length() - bs.length());
            else if ( !a_string && b_string ) result = a - bs.length();
            else if ( a_string && !b_string ) result = as.length() - b;
            else                              result = a - b;
        } else if ( token == "*" ) {
            if ( a_string && b_string )       result = static_cast<int_fast64_t>(as.length() * bs.length());
            else if ( !a_string && b_string ) result = a * bs.length();
            else if ( a_string && !b_string ) result = as.length() * b;
            else                              result = a * b;
        } else if ( token == "&&" ) {
            if ( a_string && b_string )       result = as.length() && bs.length();
            else if ( !a_string && b_string ) result = a && bs.length();
            else if ( a_string && !b_string ) result = as.length() && b;
            else                              result = a && b;
        } else if ( token == "||" ) {
            if ( a_string && b_string )       result = as.length() || bs.length();
            else if ( !a_string && b_string ) result = a || bs.length();
            else if ( a_string && !b_string ) result = as.length() || b;
            else                              result = a || b;
        } else if ( token == "<" ) {
            if ( a_string && b_string )       result = as.length() < bs.length();
            else if ( !a_string && b_string ) result = a < bs.length();
            else if ( a_string && !b_string ) result = as.length() < b;
            else                              result = a < b;
        } else if ( token == "<=" ) {
            if ( a_string && b_string )       result = as.length() <= bs.length();
            else if ( !a_string && b_string ) result = a <= bs.length();
            else if ( a_string && !b_string ) result = as.length() <= b;
            else                              result = a <= b;
        } else if ( token == ">" ) {
            if ( a_string && b_string )       result = as.length() > bs.length();
            else if ( !a_string && b_string ) result = a > bs.length();
            else if ( a_string && !b_string ) result = as.length() > b;
            else                              result = a > b;
        } else if ( token == ">=" ) {
            if ( a_string && b_string )       result = as.length() >= bs.length();
            else if ( !a_string && b_string ) result = a >= bs.length();
            else if ( a_string && !b_string ) result = as.length() >= b;
            else                              result = a >= b;
        } else if ( token == "==" ) {
            if ( a_string && b_string )       result = as == bs;
            else if ( !a_string && b_string ) result = a == bs.length();
            else if ( a_string && !b_string ) result = as.length() == b;
            else                              result = a == b;
        } else if ( token == "!=" ) {
            if ( a_string && b_string )       result = as.length() != bs.length();
            else if ( !a_string && b_string ) result = a != bs.length();
            else if ( a_string && !b_string ) result = as.length() != b;
            else                              result = a != b;
        }

        return result;
    }

    bool database::evaluate_expression( std::vector<std::string*>& tokens, uint_fast64_t row ) {
        std::stack<cell_data*> values;
        bool result = true;

        for ( std::string *token : tokens ) {
            // checking if we're dealing with a number
            if ( isdigit( token->at(0) ) || (token->at(0) == '-' && isdigit(token->at(1))) ) {
                cell_data* cd = new cell_data( stod( *token ) );
                values.push( cd );
            }

            // checking if we're dealing with any operator
            else if ( isOperator( *token ) ) {
                if ( values.size() < 2 ) {
                    std::cerr << "ERR: Invalid expression format!" << std::endl;
                    return NAN;
                }
                cell_data *B = values.top();
                values.pop();
                cell_data *A = values.top();
                values.pop();

                // evaluating the operator
                cell_data* cd = new cell_data( evaluate_operator( A, B, *token ) );
                values.push( cd );

                // !!!
                delete A;
                delete B;
            }
            // any other type of token means a column name for a given row or a string
            else {
                // checking if the column exists (for now assuming it's a column)
                uint_fast16_t index = operation_table->get_column_index( *token );
                if ( index != NULL16_INDEX ) {
                    cell_data* cd = new cell_data( operation_table->get_row( row, index ) );
                    values.push( cd );
                } else {
                    std::cerr << "ERR: Invalid column name!" << std::endl;
                    return NAN;
                }
            }
        }

        if ( values.size() != 1 ) {
            std::cerr << "ERR: Invalid expression format!" << std::endl;
            // !!!
            for ( uint_fast64_t i = 0; i < values.size(); i++ ) {
                delete values.top();
                values.pop();
            }
            return NAN;
        }
        
        // false (empty string or 0), true (anything else)
        if ( std::holds_alternative<std::string>( *values.top() ) )
            result = !std::get<std::string>( *values.top() ).empty();
        else if ( std::holds_alternative<int_fast64_t>( *values.top() ) )
            result = std::get<int_fast64_t>( *values.top() );
        else if ( std::holds_alternative<long double>( *values.top() ) )
            result = std::get<long double>( *values.top() );

        // !!!
        for ( uint_fast64_t i = 0; i < values.size(); i++ ) {
            delete values.top();
            values.pop();
        }

        return result;
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
                if ( tokens.size() > 4 ) {
                    // checking whether the 'SELECT' occurred
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
            } else if ( *tokens[0] == "ALIAS" ) {
                if ( tokens.size() > 2 ) {
                    // renaming the column
                    operation_table->rename_column( *tokens[1], *tokens[2] );
                } else {
                    std::cerr << "ERR: Not enough arguments!" << std::endl;
                }
            } else if ( *tokens[0] == "PICK" ) {
                if ( tokens.size() > 1 ) {
                    // deleting the "PICK"
                    delete tokens[0];
                    tokens.erase( tokens.begin() );
                    table* ot = new table;

                    // selecting the columns and pushing them to new table
                    uint_fast16_t index = NULL16_INDEX;
                    for ( std::string* token : tokens ) {
                        index = operation_table->get_column_index( *token );
                        if ( index != NULL16_INDEX ) {
                            // 1.5 hours of trying to find a solution (alongside with table constructor)
                            column_data *new_data = new column_data;
                            std::string new_name = std::get<0>( operation_table->data[index] );
                            
                            // deep copy
                            for ( cell_data* dc : *std::get<1>(operation_table->data[index]) ) {
                                new_data->push_back( new cell_data( *dc ) );
                            }

                            column_whole cw = std::make_tuple( new_name, new_data );
                            ot->data.push_back( cw );
                        }
                    }

                    // replacing the tables
                    *operation_table = *ot;

                    delete ot;
                } else {
                    std::cerr << "ERR: Not enough arguments!" << std::endl;
                }
            } else if ( *tokens[0] == "WHERE" ) {
                if ( tokens.size() > 1 ) {
                    // checking whether the 'SELECT' occurred
                    if ( operation_table->data.size() ) {
                        // deleting the "WHERE"
                        delete tokens[0];
                        tokens.erase( tokens.begin() );
                        // the amount of rows based on the first column
                        uint_fast64_t rows = std::get<1>( operation_table->data[0] )->size();
                        bool check = true;

                        for ( uint_fast64_t row = 0; row < rows; row++ ) {
                            check = evaluate_expression( tokens, row );
                            // if row does not match the expressions, we erase it
                            if ( !check ) {
                                column_data* cd;
                                for ( column_whole cw : operation_table->data ) {
                                    cd = std::get<1>( cw );
                                    delete cd->at( row );
                                    cd->erase( cd->begin() + row );
                                }
                                // when erasing the number of rows decreases
                                // and the index stays the same
                                rows--;
                                row--;
                            }
                        }
                    } else {
                        std::cerr << "ERR: Didn't specify a starting table!" << std::endl;
                    }
                } else {
                    std::cerr << "ERR: Not enough arguments!" << std::endl;
                }
            } else if ( *tokens[0] == "UPDATE" ) {

            }
        }
    }

    void database::rvquery( std::string query ) {
        // reseting the query table
        delete operation_table;
        operation_table = new table;

        // dividing the query into lines ending with a semicolon
        std::vector<std::string*> lines;
        std::istringstream q( query );
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
}