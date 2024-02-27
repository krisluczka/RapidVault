#include "error_handler.h"

namespace rv {
	error_handler::error_handler() { }

	error_handler::~error_handler() {
		for ( std::string* e : errors_ids ) {
			delete e;
		}
		for ( std::string* w : warnings_ids ) {
			delete w;
		}
	}

	void error_handler::print_errors() {
		if ( errors.size() ) std::cerr << "Query failed! " << errors.size() << " errors in total!\n";
		else std::cerr << "Query run successfully.\n";
		for ( uint_fast64_t i = 0; i < errors.size(); i++ ) {
			std::cerr << "    " << *errors_ids[i] << ": ";
			switch ( errors[i] ) {
				case ERROR_TYPE::INVALID_COLUMN_NAME:
					std::cerr << "Invalid column name!\n";
					break;
				case ERROR_TYPE::INVALID_TABLE_NAME:
					std::cerr << "Invalid table name!\n";
					break;
				case ERROR_TYPE::INVALID_EXPRESSION_FORMAT:
					std::cerr << "Invalid expression format!\n";
					break;
				case ERROR_TYPE::NO_STARTING_TABLE:
					std::cerr << "Didn't specify a starting table!\n";
					break;
				case ERROR_TYPE::NOT_ENOUGH_ARGUMENTS:
					std::cerr << "Not enough arguments!\n";
					break;
				case ERROR_TYPE::INVALID_INSTRUCTION:
					std::cerr << "Invalid instruction!\n";
					break;
				default:
					std::cerr << "Unexpected error!\n";
					break;
			}
		}
		std::cerr << std::endl;
	}

	void error_handler::print_warnings() {
		if ( warnings.size() ) std::cerr << warnings.size() << " warnings in total!\n";
		else std::cerr << "No query warnings.\n";
		for ( uint_fast64_t i = 0; i < warnings.size(); i++ ) {
			std::cerr << "    " << *warnings_ids[i] << ": ";
			switch ( warnings[i] ) {
				case WARNING_TYPE::DIVISION_BY_ZERO:
					std::cerr << "Division by zero leads to unexpected behavior.\n";
					break;
				case WARNING_TYPE::TYPES_MIXUP:
					std::cerr << "Comparing string to number leads to a behavior described in documentation.\n";
					break;
				default:
					std::cerr << "Unexpected warning!\n";
					break;
			}
		}
		std::cerr << std::endl;
	}
}