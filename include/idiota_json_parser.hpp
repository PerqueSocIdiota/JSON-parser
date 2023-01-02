#ifndef IDIOTA_JSON_PARSER_HPP
#define IDIOTA_JSON_PARSER_HPP

#include <assert.h>

#ifdef IDIOTA_JSON_VERBOSE
	#include <iostream>
#endif

#include <string>
#include <fstream>

#include "idiota_json_value.hpp"

namespace idiota {
	class JSONParser {
	private:
		std::ifstream _fs;
		JSONValue _root;
		
		unsigned int skipWhitespace();

		std::string readString();
		JSONValueType readToken();
		double readNumber();

		void parseObject(JSONValue &obj);
		bool parseValue(JSONValue &obj);
		void parseArray(JSONValue &obj);

		std::string printString(JSONValue &obj);
		std::string printNumber(JSONValue &obj);
		std::string printArray(JSONValue &obj);
		std::string printValue(JSONValue &obj);
	public:
		JSONParser();
		~JSONParser();

		void open(std::string filename);

		JSONValue parse();

		void create(JSONValue &obj, const std::string &filename);
		std::string printObject(JSONValue &obj);
	};
}

#endif