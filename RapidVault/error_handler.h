#pragma once
#include <iostream>
#include <vector>

namespace rv {
	enum ERROR_TYPE : uint_fast8_t {
		INVALID_EXPRESSION_FORMAT,
		INVALID_COLUMN_NAME,
		INVALID_TABLE_NAME,
		NOT_ENOUGH_ARGUMENTS,
		NO_STARTING_TABLE,
		INVALID_INSTRUCTION,
	};

	enum WARNING_TYPE : uint_fast8_t {
		DIVISION_BY_ZERO,
		TYPES_MIXUP,
	};

	class error_handler {
		public:
			friend class database;
			error_handler();
			~error_handler();



			/*
				###############################
					Information displaying
				###############################
			*/
			/*
				Displays every error catched while evaluating a query
			*/
			void print_errors();

			/*
				Displays every warning catched while evaluating a query
			*/
			void print_warnings();
		private:
			/*
				The errors and warnings infos
			*/
			std::vector<ERROR_TYPE> errors;
			std::vector<std::string*> errors_ids;
			std::vector<WARNING_TYPE> warnings;
			std::vector<std::string*> warnings_ids;



			/*
				###############################
					Information manipulating
				###############################
			*/
			/*
				Resets whole error handler
			*/
			inline void clear() {
				errors.clear();
				warnings.clear();
				for ( std::string* e : errors_ids ) {
					delete e;
				}
				for ( std::string* w : warnings_ids ) {
					delete w;
				}
				errors_ids.clear();
				warnings_ids.clear();
			}

			/*
				Pushes a new error
			*/
			inline void push_error( ERROR_TYPE error, std::string error_id ) {
				errors.push_back( error );
				errors_ids.push_back( new std::string( error_id ) );
			}

			/*
				Pushes a new warning
			*/
			inline void push_warning( WARNING_TYPE warning, std::string warning_id ) {
				warnings.push_back( warning );
				warnings_ids.push_back( new std::string( warning_id ) );
			}
	};
}