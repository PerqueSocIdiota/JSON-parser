
#include <iostream>

#include "idiota_json_parser.hpp"

int main() {
	std::cout << "JSON TEST" << std::endl;

	idiota::JSONParser parser;

	parser.open("data/data.json");
	std::cout << "READ FILE" << std::endl;
	idiota::JSONValue json = parser.parse();
	std::cout << "FILE PARSED" << std::endl;

	std::cout << "STRING: " << json["string"].string() << std::endl;
	std::cout << "ARRAY OF OBJECTS: " << std::endl;
	for(auto i = json["array-of-objects"].array().begin(); i != json["array-of-objects"].array().end(); ++i) {
		std::cout << "  NUMBER: " << i->at("number").number() << std::endl;
		std::cout << "  VALID: " << i->at("valid").boolean() << std::endl;
	}
	
	std::cout << "NULL OBJECT: " << (json["null-object"].null() ? "null" : false) << std::endl;
	std::cout << "NUMBERS: " << std::endl;
	std::cout << "  VALUES: " << std::endl;
	for(auto i = json["numbers"]["values"].array().begin(); i != json["numbers"]["values"].array().end(); ++i) {
		std::cout << "    : " << i->number() << std::endl;
	}
	for(auto i = json["numbers"]["names"].array().begin(); i != json["numbers"]["names"].array().end(); ++i) {
		std::cout << "    : " << i->string() << std::endl;
	}

	std::cout << "JSON: " << parser.printObject(json) << std::endl;

	parser.create(json, "data/out.json");

	return 0;
}