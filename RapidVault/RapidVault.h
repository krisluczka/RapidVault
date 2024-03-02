/*
	
	MIT License

	Copyright (c) 2024, Krzysztof £uczka

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.



		https://github.com/krisluczka/RapidVault

*/
#pragma once
#include <fstream>
#include "database.h"
#include "table.h"
#include "error_handler.h"

namespace rv {
	class RapidVault {
		public:
			//RapidVault();
			//~RapidVault();



			/*
				Loading a database
			*/
			bool load( std::string );

			/*
				Saving a database
			*/
			bool save( uint_fast64_t );

		//private:
			std::vector<database*> databases;

			/*
				Saves table data to binary file
			*/
			bool save_table( std::ofstream*, table* );
			/*
				Reads a binary file and puts data in a table
			*/
			bool load_table( std::ifstream*, table* );
	};
}