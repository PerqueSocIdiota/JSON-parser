#include "idiota_json_parser.hpp"

idiota::JSONParser::JSONParser() {
}

idiota::JSONParser::~JSONParser() {
	if(this->_fs.is_open()) {
		this->_fs.close();
	}
}

unsigned int idiota::JSONParser::skipWhitespace() {
	char c; 
	unsigned int skipped_characters = 0;

	while(this->_fs.good()) {
		this->_fs.get(c);

		switch(c) {
			case ' ':
			case '\n':
			case '\r':
			case '\t':
				break;
			default:
				// Once a non-whitespace character is found
				// put it back on the stream so it can be processed
				this->_fs.putback(c);
				return skipped_characters;
		}
		skipped_characters++;
	}

	return skipped_characters;
}

std::string idiota::JSONParser::readString() {
	std::string s;
	char c;
	
	char last_c = '\0';

	while(this->_fs.good()) {
		this->_fs.get(c);

		if(last_c == '\\') {
			// If a character is precedeed by '\' it is a special character
			// and we process it separately
			switch(c) {
				case '"':
				case '\\':
				case '/':
				s.push_back(c);
				break;
				case 'b':
				s.push_back('\b');
				break;
				case 'f':
				s.push_back('\f');
				break;
				case 'n':
				s.push_back('\n');
				break;
				case 'r':
				s.push_back('\r');
				break;
				case 't':
				s.push_back('\t');
				break;
				// TODO: add the case for \u special character.
			}
		} else if(c == '"') {
			// When a " is found, it means we reached the end of the string
			break;
		} else if(c != '\\') {
			// Any character other than '\' is appended to the string.
			// We don't process the '\' character, as it is used for the special characters
			s.push_back(c);
		}

		last_c = c;
	}

	// TODO: check if the string has been properly read ('"' character was processed)
	//       or if we are at this point after reaching the end of the stream, in
	//       such case an exception should be thrown
	return s;
}

double idiota::JSONParser::readNumber() {
	std::string s;
	char c;
	double num = 0;

	// Read until whitespace, ',', '}' or ']' are found.
	// then convert the resulting string to double.
	// TODO: A number is made of digits (0 to 9), '-', '.', 'e' or 'E'
	//       check the character is one of those valid ones before
	//       appending it to the string
	while(this->_fs.good()) {
		this->_fs.get(c);

		switch(c) {
			case ',':
			case '}':
			case ']':
				// Put back the ',', '}' or ']' into the stream
				// so we can process the end of object or array
				this->_fs.putback(c);
			case ' ':
			case '\r':
			case '\n':
			case '\t':
				num = std::stod(s);
				return num;
			default:
				s.push_back(c);
		}
	}

	return num;
}

void idiota::JSONParser::parseObject(idiota::JSONValue &obj) {
	char c;
	std::string key;

	// An object is made of a key/value pair.
	// For one side we get the string corresponding to
	// the key, then we get the value
	// Once we have both elements we can update
	// the JSON object.
	while(this->_fs.good()) {
		this->_fs.get(c);

		if(c == '}') {
			// The closing brace indicates the end of the object,
			// so we stop processing it and return.
			#ifdef IDIOTA_JSON_VERBOSE
			std::cout << "CLOSING OBJECT" << std::endl;
			#endif
			return;			
		} else if(c == '\"') {
			// The '"' character means we are processing a string,
			// which corresponds to the key
			key = this->readString();
			#ifdef IDIOTA_JSON_VERBOSE
			std::cout << "FOUND KEY: \"" << key << "\"" << std::endl;
			#endif
		} else if( c == ':') {
			// Whatever after the ':' is the value associated to the key
			// At this point add a new key/value pair to the current
			// object, then get the value of this object
			obj.object().insert(std::make_pair(key, JSONValue()));

			#ifdef IDIOTA_JSON_VERBOSE
			std::cout << "PARSING VALUE FOR KEY \"" << key << "\":" << std::endl;
			#endif

			this->parseValue(obj[key]);
		}
	}
}

bool idiota::JSONParser::parseValue(idiota::JSONValue &obj) {
	char c;
	// A JSON value can be a object, array, string, number, true, false or null
	// Then, we find out the type of the object and call the right method to
	// parse it.
	while(this->_fs.good()) {
		this->_fs.get(c);

		switch(c) {
			case '}':
			case ']':
			case ',':
				// If an end of object ('}'), end of list (']'), or end of value (',')
				// is found, stop parsing value
				// TODO: it shouldn't reach this point, but it does
				//       this has to be checked as there is something potentially wrong
				//       here
				this->_fs.putback(c);
				return false;
			case '{':
				// The curly bracket indicates the beginning of an object.
				// An object is a collection (map) of key/values pairs.
				obj.type() = JSONValueType::Object;
				#ifdef IDIOTA_JSON_VERBOSE
				std::cout << "TYPE IS " << obj.type() << std::endl;
				#endif
				parseObject(obj);
				return true;
			case '[':
				// The square bracket indicates the beginning of an array
				// An array is a list (vector) of values.
				obj.type() = JSONValueType::Array;
				#ifdef IDIOTA_JSON_VERBOSE
				std::cout << "TYPE IS " << obj.type() << std::endl;
				#endif
				parseArray(obj);
				return true;
			case '"':
				// '"' indicates the beginning of a string.
				// We read the string from the stream and set it as the
				// current value.
				obj.type() = JSONValueType::String;
				obj.string() = readString();
				#ifdef IDIOTA_JSON_VERBOSE
				std::cout << "TYPE IS " << obj.type() << ": " << obj.string() << std::endl;
				#endif
				return true;
			case 't':
			case 'f':
			case 'n':
				// Parsing token (true, false or null)
				// Tokens are underscore and are not enclosed within '"'.
				// Then when we found a potential token (a word that starts
				// with t, f, or n) we call readToken to obtain it's value
				this->_fs.putback(c);
				obj.type() = this->readToken();
				#ifdef IDIOTA_JSON_VERBOSE
				std::cout << "TYPE IS " << obj.type() << std::endl;
				#endif
				return true;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '-':
				// A number starts with a digit or '-', then either one of those
				// characters is found we call readNumber to get the next number
				// from the stream.
				this->_fs.putback(c);
				obj.type() = JSONValueType::Number;
				obj.number() = this->readNumber();
				#ifdef IDIOTA_JSON_VERBOSE
				std::cout << "TYPE IS " << obj.type() << ": " << obj.number() << std::endl;
				#endif
				return true;
		}
	}

	return false;
}

void idiota::JSONParser::parseArray(idiota::JSONValue &obj) {
	char c;

	// An array is a list of values. To get them, we iterate
	// over the stream and append the different values to the
	// current object until we reach the end of the list
	while(this->_fs.good()) {
		JSONValue array_item;

		this->_fs.get(c);

		switch(c) {
			case ']':
				// The square bracket indicates the end of the list
				// When reaching this point, stop processing the array
				#ifdef IDIOTA_JSON_VERBOSE
				std::cout << "CLOSING ARRAY" << std::endl;
				#endif
				return;
			case ',':
				break;
			default:
				this->_fs.putback(c);
				if(parseValue(array_item)) {
					obj.array().push_back(array_item);
					#ifdef IDIOTA_JSON_VERBOSE
					std::cout << "ADDING VALUE OF TYPE " << array_item.type() << " TO ARRAY" << std::endl;
					#endif
				}
				break;
		}
	}
}

idiota::JSONValueType idiota::JSONParser::readToken() {
	std::string s;
	char c;
	// Read until whitespace, ',', '}' or ']'
	while(this->_fs.get(c)) {
		switch(c) {
			case ',':
			case '}':
			case ']':
			this->_fs.putback(c);
			case ' ':
			case '\r':
			case '\n':
			case '\t':
				// TODO: Instead of default to null, check if value is valid
				//       and throw an error otherwise
				if(s == "true") {
					return JSONValueType::True;
				} else if(s == "false") {
					return JSONValueType::False;
				} else {
					return JSONValueType::Null;
				}
			break;
			default:
			s.push_back(c);
		}
	}

	return JSONValueType::Null;
}

std::string idiota::JSONParser::printArray(JSONValue &obj) {
	std::string s;

	s = "[";
	for(unsigned int i = 0; i < obj.array().size(); ++i) {
		s += printValue(obj.array().at(i));

		if(i < (obj.array().size()-1)) {
			s += ", ";
		}
	}
	s += "]";

	return s;
}

std::string idiota::JSONParser::printObject(JSONValue &obj) {
	std::string s;

	std::vector<std::string> keys;
	for(auto i = obj.object().begin(); i != obj.object().end(); ++i) {
		keys.push_back(i->first);
	}

	s = "{";
	for(unsigned int i = 0; i < keys.size(); ++i) {
		std::string key = keys[i];
		s += "\"" + key + "\": ";
		s += printValue(obj[key]);

		if(i < (keys.size()-1)) {
			s += ", ";
		}
	}
	s += "}";

	return s;
}

std::string idiota::JSONParser::printValue(JSONValue &obj) {

	switch (obj.type()) {
	case JSONValueType::Array:
		return this->printArray(obj);
	case JSONValueType::False:
		return "false";
	case JSONValueType::True:
		return "true";
	case JSONValueType::Null:
		return "null";
	case JSONValueType::Number:
		return this->printNumber(obj);
	case JSONValueType::Object:
		return this->printObject(obj);
	case JSONValueType::String:
		return this->printString(obj);
	default:
		return "";
	}
}

std::string idiota::JSONParser::printString(idiota::JSONValue &obj) {
	std::string s;
	// TODO: special characters are printed properly?
	s = "\"" + obj.string() + "\"";

	return s;	
}

std::string idiota::JSONParser::printNumber(idiota::JSONValue &obj) {
	// TODO: should check if it returns "inf" or "nan"
	return std::to_string(obj.number());
}

void idiota::JSONParser::open(std::string filename) {
	this->_fs.open(filename);

	assert(this->_fs.is_open());
}

idiota::JSONValue idiota::JSONParser::parse() {
	assert(this->_fs.is_open());

	this->_root.type() = JSONValueType::Object;

	parseObject(this->_root);

	return this->_root;
}

void idiota::JSONParser::create(idiota::JSONValue &obj, const std::string &filename) {
	std::ofstream out(filename);

	if(out.is_open()) {
		out << this->printObject(obj);
		out.close();
	}
}