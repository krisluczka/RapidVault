#include "database.h"

namespace rv {
	database::database( std::string database_name ) {
		name = database_name;
	}

	database::database() {
		name = "rapidvault_database";
	}

	database::~database() {
		for ( table* t : tables ) {
			delete t;
		}
		for ( relation* r : relations ) {
			delete r;
		}
	}

	uint_fast64_t database::add_relation( std::variant<uint_fast64_t, std::string> A, std::variant<uint_fast64_t, std::string> B, uint_fast8_t type ) {
		uint_fast64_t index = NULL64_INDEX;
		uint_fast64_t index_A = NULL64_INDEX;
		uint_fast64_t index_B = NULL64_INDEX;

		// checking the table A identifier
		std::visit( [this, &index_A]( auto arg ) {
			if constexpr ( std::is_same_v<std::decay_t<decltype(arg)>, uint_fast64_t> ) {
				index_A = arg;
			} else if constexpr ( std::is_same_v<std::decay_t<decltype(arg)>, std::string> ) {
				const std::string n = arg;
				for ( uint_fast64_t i = 0; i < this->tables.size(); i++ ) {
					if ( tables[i]->name == n ) {
						index_A = i;
						break;
					}
				}
			}
		}, A );

		// checking the table B identifier
		std::visit( [this, &index_B]( auto arg ) {
			if constexpr ( std::is_same_v<std::decay_t<decltype(arg)>, uint_fast64_t> ) {
				index_B = arg;
			} else if constexpr ( std::is_same_v<std::decay_t<decltype(arg)>, std::string> ) {
				const std::string n = arg;
				for ( uint_fast64_t i = 0; i < this->tables.size(); i++ ) {
					if ( tables[i]->name == n ) {
						index_B = i;
						break;
					}
				}
			}
		}, B );

		// if it's valid, push it
		if ( (index_A != NULL64_INDEX) && (index_B != NULL64_INDEX) && (type < 4) ) {
			relation* r = new relation( index_A, index_B, type );
			relations.push_back( r );
			index = relations.size() - 1;
		}
		
		return index;
	}

	uint_fast64_t database::remove_relation( uint_fast64_t index ) {
		if ( (index != NULL64_INDEX) && (index < this->relations.size()) ) {
			relation* r = relations[index];
			delete r;
			relations.erase( relations.begin() + index );
		}
		return relations.size();
	}

	uint_fast64_t database::remove_relation( std::variant<uint_fast64_t, std::string> A, std::variant<uint_fast64_t, std::string> B, uint_fast8_t type ) {
		uint_fast64_t index_A = NULL64_INDEX;
		uint_fast64_t index_B = NULL64_INDEX;

		// checking the table A identifier
		std::visit( [this, &index_A]( auto arg ) {
			if constexpr ( std::is_same_v<std::decay_t<decltype(arg)>, uint_fast64_t> ) {
				index_A = arg;
			} else if constexpr ( std::is_same_v<std::decay_t<decltype(arg)>, std::string> ) {
				const std::string n = arg;
				for ( uint_fast64_t i = 0; i < this->tables.size(); i++ ) {
					if ( tables[i]->name == n ) {
						index_A = i;
						break;
					}
				}
			}
		}, A );

		// checking the table B identifier
		std::visit( [this, &index_B]( auto arg ) {
			if constexpr ( std::is_same_v<std::decay_t<decltype(arg)>, uint_fast64_t> ) {
				index_B = arg;
			} else if constexpr ( std::is_same_v<std::decay_t<decltype(arg)>, std::string> ) {
				const std::string n = arg;
				for ( uint_fast64_t i = 0; i < this->tables.size(); i++ ) {
					if ( tables[i]->name == n ) {
						index_B = i;
						break;
					}
				}
			}
		}, B );

		// searching for a given relation
		relation* r;
		for ( uint_fast64_t index = 0; index < relations.size(); index++ ) {
			r = relations[index];
			if ( (r->A == index_A) && (r->B == index_B) && (r->type == type) ) {
				delete r;
				relations.erase( relations.begin() + index );
				break;
			}
		}

		return relations.size();
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