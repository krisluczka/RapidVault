#include "database.h"

namespace rv {
	database::~database() {
		for ( table* t : tables ) {
			delete t;
		}
		delete operation_table;
	}

	void database::display() const {
		uint_fast16_t table_amount( tables.size() );
		uint_fast64_t row_amount( 0 );
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
}