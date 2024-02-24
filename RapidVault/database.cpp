#include "database.h"

namespace rv {
	database::database( std::string database_name ) {
		name = database_name;
		operation_table = nullptr;
	}

	database::database() {
		name = "rapidvault_database";
		operation_table = nullptr;
	}

	database::~database() {
		for ( table* t : tables ) {
			delete t;
		}
		delete operation_table;
	}

	void database::display() {
		uint_fast16_t table_amount;
		uint_fast64_t row_amount = 0;
		table_amount = tables.size();
		std::cout << " # " << name << " ( " << table_amount << " tables in total )\n";
		for ( table* t : tables ) {
			if ( t->data.size() ) row_amount = std::get<1>( t->data[0] )->size();
			std::cout << "    " << t->name << " ( " << row_amount << " rows in total )\n";
			for ( column_whole cw : t->data ) {
				std::cout << "     * " << std::get<0>( cw ) << "\n";
			}
			row_amount = 0;
			std::cout << "\n";
		}
	}

	table* database::create_table( std::string name ) {
		table* t = new table;
		t->name = name;
		tables.push_back( t );
		return t;
	}

	table* database::get_table_pointer( std::variant<uint_fast64_t, std::string> identifier ) const {
		// checking the table identifier
		std::visit( [this]( auto arg ) {
			if constexpr ( std::is_same_v<std::decay_t<decltype(arg)>, uint_fast64_t> ) {
				if ( (arg != NULL64_INDEX) && (arg < this->tables.size()) ) {
					return tables[arg];
				}
			} else if constexpr ( std::is_same_v<std::decay_t<decltype(arg)>, std::string> ) {
				const std::string n = arg;
				for ( uint_fast64_t i = 0; i < this->tables.size(); i++ ) {
					if ( tables[i]->name == n ) {
						return tables[i];
						break; // :o
					}
				}
			}
		}, identifier );

		return nullptr;
	}

	uint_fast64_t database::get_table_index( std::string identifier ) const {
		// checking the table identifier
		for ( uint_fast64_t i = 0; i < this->tables.size(); i++ ) {
			if ( tables[i]->name == identifier ) {
				return i;
				break; // :o
			}
		}
		return NULL64_INDEX;
	}
}