#ifndef IDIOTA_JSON_VALUE_HPP
#define IDIOTA_JSON_VALUE_HPP

#include <assert.h>

#include <string>
#include <map>
#include <vector>

#ifdef IDIOTA_JSON_VERBOSE
#include <iostream>
#endif

namespace idiota {
	enum class JSONValueType : unsigned char {String, Number, Object, Array, True, False, Null};

	class JSONValue {
	private:
		JSONValueType _type {JSONValueType::Null};
		std::string _string;
		double _number;
		std::vector<JSONValue> _array;
		std::map<std::string, JSONValue> _object;
	
	public:
		JSONValue &operator[](const std::string &key);
		JSONValue &at(const std::string &key);

		JSONValueType &type();

		bool boolean();
		bool null();
		std::string &string();
		double &number();
		std::vector<JSONValue> &array();
		std::map<std::string, JSONValue> &object();
	};
	
	#ifdef IDIOTA_JSON_VERBOSE
	std::ostream &operator<<(std::ostream &out, const JSONValueType &type);
	#endif
}

#endif