/*
	RVquery is a part of the RapidVault project,
	and it's distributed under the same MIT license.



	#	RVquery small documentation		#


	Queries are written by singular commands separated by a semicolon.
	For example:
		"command arg arg; command; command arg;"

	For each query, an environment is created in which the result
	is stored. So when the query looks like this:
		"MERGE table1 table2;"
	It spits out both tables joined together
	according to the relation rules. Adding additional commands in
	the same query after the 'MERGE' would do the operations on joined table.

	! WARNING !
	When merging tables, the columns will appear with the name
		"table_name.column_name"
	instead of simple "column_name"

		<command> - <description>
			<example of use>



	>	Operating queries	<

		MERGE <arg1> <arg2>, ... - selects given tables and joins them
			MERGE table1, table2, table3;

		PICK <arg1> <arg2> ... - selects given columns, discarding others
		from the final result
			PICK table1.column1, table2.column6;

		WHERE <expression> - selects only these rows, where the expression is true
		! WARNING ! Expressions use Reverse Polish Notation (more details at the end)
			WHERE column2 column1 > column3 column1 > &&;

		UPDATE <column> <value> - changes every value in the given column
			UPDATE users.age 18;

	>	Manipulating queries	<

		INSERT
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
	}

    void database::go_go_gadget_query( std::vector<std::string*>& tokens ) {
        if ( *tokens[0] == "MERGE" ) {
            
        } else if ( *tokens[0] == "PICK" ) {

        } else if ( *tokens[0] == "WHERE" ) {

        } else if ( *tokens[0] == "UPDATE" ) {

        }
    }
}