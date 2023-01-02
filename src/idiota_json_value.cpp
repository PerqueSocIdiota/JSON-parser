#include "idiota_json_value.hpp"


idiota::JSONValue &idiota::JSONValue::operator[](const std::string &key) {
	assert(this->_type == JSONValueType::Object);

	return this->_object[key];
}	

idiota::JSONValue &idiota::JSONValue::at(const std::string &key) {
	assert(this->_type == JSONValueType::Object);

	return this->_object[key];
}

idiota::JSONValueType &idiota::JSONValue::type() {
	return this->_type;
}

bool idiota::JSONValue::boolean() {
	assert(this->_type == JSONValueType::True || this->_type == JSONValueType::False);

	// TODO: throw exception if not a boolean type?

	return (this->_type == JSONValueType::True);
}

bool idiota::JSONValue::null() {
	assert(this->_type == JSONValueType::Null);

	// TODO: this should return nullptr?
	return (this->_type == JSONValueType::Null);
}

std::string &idiota::JSONValue::string() {
	assert(this->_type == JSONValueType::String);

	return this->_string;
}

std::vector<idiota::JSONValue> &idiota::JSONValue::array() {
	assert(this->_type == JSONValueType::Array);

	return this->_array;
}

std::map<std::string, idiota::JSONValue> &idiota::JSONValue::object() {
	assert(this->_type == JSONValueType::Object);

	return this->_object;
}

double &idiota::JSONValue::number() {
	assert(this->_type == JSONValueType::Number);

	return this->_number;
}

	
#ifdef IDIOTA_JSON_VERBOSE
std::ostream &idiota::operator<<(std::ostream &out, const idiota::JSONValueType &type) {
	switch(type) {
		case idiota::JSONValueType::String:
			out << "string";
			break;
		case idiota::JSONValueType::Number:
			out << "number";
			break;
		case idiota::JSONValueType::Object:
			out << "object";
			break;
		case idiota::JSONValueType::Array:
			out << "array";
			break;
		case idiota::JSONValueType::True:
			out << "true";
			break;
		case idiota::JSONValueType::False:
			out << "false";
			break;
		case idiota::JSONValueType::Null:
			out << "null";
			break;
		default:
			out << "null";
	}

	return out;
}
#endif