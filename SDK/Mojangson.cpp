#include "Mojangson.h"

#include "../Utils/Logger.h"

static std::unique_ptr<Tag> ParseValue(const char*& in) {

	switch (*in) {
	case MojangsonToken::ARRAY_START:
		return Mojangson::ParseInternal(in);
	case MojangsonToken::COMPOUND_START:
		return Mojangson::ParseInternal(in);
	case MojangsonToken::STRING_QUOTES: {
		in++; //skip STRING_QUOTES
		std::unique_ptr<StringTag> stringTag = std::make_unique<StringTag>();

		bool isParsingJson = false;
		{
			const char* stringTester = in;
			MojangsonToken::SkipWhitespaces(stringTester);
			while (*stringTester == '[' || *stringTester == '{')   // json file start
				stringTester++;
			if (in != stringTester) { //if we are further than the start that means we found { or [
				MojangsonToken::SkipWhitespaces(stringTester);
				//lets see if its encoded or just raw json in a string
				if (*stringTester == '}' || *stringTester == '}') //end of json (empty)
					isParsingJson = false;
				else if (*stringTester == '\\') //encoded quote
					isParsingJson = false;  // this json is encoded already
				else if (*stringTester == MojangsonToken::STRING_QUOTES || *stringTester == '{' || *stringTester == '[')
					isParsingJson = true; //this is the old parser's mess the str is raw json
			}
		}
		if (isParsingJson) {
			int scope = 0;
			bool inJsonStr = false;
			while (true) {
				if (scope == 0 && *in == MojangsonToken::STRING_QUOTES)
					break;
				switch (*in) {
				case MojangsonToken::STRING_QUOTES:
					inJsonStr = !inJsonStr;
					stringTag->push_back(*in);
					break;
				case '\\':
					if (inJsonStr) {
						in++;
						if (*in == '\0')
							throw Mojangson::Exception(in, "Missing character to escape in json string");
					}
					stringTag->push_back(*in);
					break;
				case '{':
				case '[':
					if (!inJsonStr)
						scope++;
					stringTag->push_back(*in);
					break;
				case '}':
				case ']':
					if (!inJsonStr)
						scope--;
					stringTag->push_back(*in);
					break;
				default:
					stringTag->push_back(*in);
					break;
				}
				in++;
			}
		} else {
			while (*in != MojangsonToken::STRING_QUOTES && *in != '\0') {
				if (*in == MojangsonToken::STRING_ESCAPE) {  // escaping
					in++;                                    // skip escape character
					switch (*in) {
					case '\0':
						throw Mojangson::Exception(in, "Missing character to escape in string");
						break;
					case 'n':
					case 'N':
						stringTag->push_back('\n');
						break;
					case 'r':
					case 'R':
						stringTag->push_back('\r');
						break;
					case 't':
					case 'T':
						stringTag->push_back('\t');
						break;
					default:
						stringTag->push_back(*in);
						break;
					}
				} else 
					stringTag->push_back(*in);
				in++;
			}
		}
		if (*in == '\0')
			throw Mojangson::Exception(in, "Unexpected end of tag");
		in++; //skip MojangsonToken::STRING_QUOTES
		return stringTag;
	}
	}

	bool isNegative = *in == '-';
	in += isNegative;
	int dotCount = 0;
	std::string numberAsText;
	while (std::isdigit(*in) != 0 || *in == 'e' || *in == '+' || *in == '-') {
		numberAsText.push_back(*in);
		in++;
		while (*in == '.') {
			in++;
			numberAsText.push_back('.');
			dotCount++;
		}
	}

	if (numberAsText.empty()) {
		// lets do string then since quoteless strings are acceptable..
		std::unique_ptr<StringTag> stringTag = std::make_unique<StringTag>();
		while (*in != '\0' && *in != MojangsonToken::ARRAY_END && *in != MojangsonToken::COMPOUND_END && *in != MojangsonToken::ELEMENT_SEPERATOR) {
			if (*in == MojangsonToken::STRING_ESCAPE) {
				in++;  // skip escape character
				switch (*in) {
				case '\0':
					throw Mojangson::Exception(in, "Missing character to escape in quoteless string");
					break;
				case 'n':
				case 'N':
					stringTag->push_back('\n');
					break;
				case 'r':
				case 'R':
					stringTag->push_back('\r');
					break;
				case 't':
				case 'T':
					stringTag->push_back('\t');
					break;
				default:
					stringTag->push_back(*in);
					break;
				}
				in++;
				continue;
			}

			stringTag->push_back(*in);
			in++;
		}
		return stringTag;
	} else {
		if (dotCount > 1)
			throw Mojangson::Exception(in, "Malformed number, more than one dot");
		MojangsonToken::SkipWhitespaces(in);
		char suffix = *in;

		try {
			switch (suffix) {
			case MojangsonToken::DOUBLE_SUFFIX:
				in++; //skip suffix
				return std::unique_ptr<Tag>(new DoubleTag(isNegative ? (std::stod(numberAsText) * -1) : std::stod(numberAsText)));
			case MojangsonToken::FLOAT_SUFFIX:
				in++; //skip suffix
				return std::unique_ptr<Tag>(new FloatTag(isNegative ? ((float)std::stof(numberAsText) * -1) : (float)std::stof(numberAsText)));
			case MojangsonToken::BYTE_SUFFIX:
				in++; //skip suffix
				if (dotCount > 0)
					throw Mojangson::Exception(in, "Byte number cannot have decimals");
				return std::unique_ptr<Tag>(new ByteTag((uint8_t)(isNegative ? (std::stoi(numberAsText) * -1) : std::stoi(numberAsText))));
			case MojangsonToken::SHORT_SUFFIX:
				in++; //skip suffix
				if (dotCount > 0)
					throw Mojangson::Exception(in, "Short number cannot have decimals");
				return std::unique_ptr<Tag>(new ShortTag((int16_t)(isNegative ? (std::stoi(numberAsText) * -1) : std::stoi(numberAsText))));
			case MojangsonToken::LONG_SUFFIX:
				in++; //skip suffix
				if (dotCount > 0)
					throw Mojangson::Exception(in, "Long number cannot have decimals");
				return std::unique_ptr<Tag>(new Int64Tag((int64_t)(isNegative ? (std::stoll(numberAsText) * -1) : std::stoll(numberAsText))));

				//valid chars after non suffixed integer
			case MojangsonToken::ARRAY_END:
			case MojangsonToken::COMPOUND_END:
			case MojangsonToken::ELEMENT_SEPERATOR:
				if (dotCount > 0)
					throw Mojangson::Exception(in, "Int number cannot have decimals");
				return std::unique_ptr<Tag>(new IntTag((int32_t)(isNegative ? (std::stoi(numberAsText) * -1) : std::stoi(numberAsText))));

				//we got garbage after the number
			case '\0':
				throw Mojangson::Exception(in, "Unexpected end of tag after number");
			default:
				throw Mojangson::Exception(in, "Invalid number suffix");
			}
		} catch (const std::invalid_argument&) {
			in -= numberAsText.size(); // go back to the start of the number for error message
			throw Mojangson::Exception(in, "Invalid number");
		} catch (const std::out_of_range&) {
			in -= numberAsText.size();  // go back to the start of the number for error message
			throw Mojangson::Exception(in, "Number is out of range");
		}
	}
	throw Mojangson::Exception(in, "Internal Error 1");
}


std::unique_ptr<Tag> Mojangson::ParseInternal(const char*& in) {
	MojangsonToken::SkipWhitespaces(in);
	if (*in == '\0')
		throw Mojangson::Exception(in, "Tag cannot be empty");


	switch (in[0]) {
	case MojangsonToken::COMPOUND_START: {
		if (in[1] == '\0')
			throw Mojangson::Exception(in, "Invalid compound tag");
		in++; //skip COMPOUND_START
		std::unique_ptr<CompoundTag> tag = std::make_unique<CompoundTag>();
		while (*in != '\0' && *in != MojangsonToken::COMPOUND_END) {
			
			MojangsonToken::SkipWhitespaces(in);
			bool inQuotes = *in == MojangsonToken::STRING_QUOTES;
			in += inQuotes;
			std::string key;

			while (true) {
				if (*in == '\0')
					break;
				else if (inQuotes) {
					if (*in == MojangsonToken::STRING_QUOTES)
						break;
				} else if (*in == MojangsonToken::ELEMENT_PAIR_SEPERATOR && key != "minecraft")
					break;
				if (*in == MojangsonToken::STRING_ESCAPE) {  // escaping
					in++; //skip to next character 
					switch (*in) {
					case '\0':
						throw Mojangson::Exception(in, "Missing character to escape in key");
						break;
					case 'n':
					case 'N':
						key.push_back('\n');
						break;
					case 'r':
					case 'R':
						key.push_back('\r');
						break;
					case 't':
					case 'T':
						key.push_back('\t');
						break;
					default:
						key.push_back(*in);
						break;
					}
					in++;
					continue;
				} 
				
				key.push_back(*in);
				in++; //next char
			}
			if (inQuotes && *in == MojangsonToken::STRING_QUOTES) {
				in++;  // skip STRING_QUOTES
				MojangsonToken::SkipWhitespaces(in);
			}
			if (*in == MojangsonToken::ELEMENT_PAIR_SEPERATOR) 
				in++;  // skip ELEMENT_PAIR_SEPERATOR
			else if (*in == '\0')
				throw Mojangson::Exception(in, "Key is missing a value");
			MojangsonToken::SkipWhitespaces(in);

			auto valueTag = ParseValue(in);
			tag->putMove(key, valueTag.get());

			//an old parser error made it so you had to put ,, so to keep that compatibility just ignore all of the now useless ones
			while (MojangsonToken::IsWhitespace(*in) || *in == MojangsonToken::ELEMENT_SEPERATOR)
				in++;
		}
		if (*in == '\0')
			throw Mojangson::Exception(in, "Unmatched {");
		in++; //skip COMPOUND_END
		return tag;
	};
	case MojangsonToken::ARRAY_START: {
		in++; //skip ARRAY_START
		MojangsonToken::SkipWhitespaces(in);
		if (std::toupper(*in) == MojangsonToken::BYTE_ARRAY_PREFIX) {
			in++; //skip type descriptor
			MojangsonToken::SkipWhitespaces(in);
			if (*in == MojangsonToken::ARRAY_TYPE_SUFFIX)
				in++;
			std::vector<uint8_t> values;

			MojangsonToken::SkipWhitespaces(in);
			while (*in != '\0' && *in != MojangsonToken::ARRAY_END) {
				std::string numberAsText;
				int multiplier = 1;
				if (*in == '-') {
					multiplier = -1;
					in++;
				}
				while (std::isdigit(*in)) {
					numberAsText.push_back(*in);
					in++;
				}
				
				if (*in == '.')
					throw Mojangson::Exception(in, "Byte arrays cannot have decimals");
				else if (*in == '\0')
					throw Mojangson::Exception(in, "Unexpected end of tag");
				else if (numberAsText.empty())
					throw Mojangson::Exception(in, "Unreadable number in byte array");

				values.push_back((uint8_t) (std::stoi(numberAsText)) * multiplier);
				MojangsonToken::SkipWhitespaces(in);
				if (*in == MojangsonToken::BYTE_SUFFIX)
					in++;
				while (MojangsonToken::IsWhitespace(*in) || *in == MojangsonToken::ELEMENT_SEPERATOR)
					in++;
				if (*in == '\0') throw Mojangson::Exception(in, "Unexpected end of tag");
			}
			if (*in == '\0') throw Mojangson::Exception(in, "Unexpected end of tag");
			in++;  // means we are skiping array end
			MojangsonToken::SkipWhitespaces(in);

			return std::make_unique<ByteArrayTag>(values);
		} else if (std::toupper(*in) == MojangsonToken::INT_ARRAY_PREFIX) {
			in++; //skip type descriptor
			MojangsonToken::SkipWhitespaces(in);
			if (*in == MojangsonToken::ARRAY_TYPE_SUFFIX)
				in++;
			std::vector<int32_t> values;

			MojangsonToken::SkipWhitespaces(in);
			while (*in != '\0' && *in != MojangsonToken::ARRAY_END) {
				std::string numberAsText;
				int multiplier = 1;
				if (*in == '-') {
					multiplier = -1;
					in++;
				}
				while (std::isdigit(*in)) {
					numberAsText.push_back(*in);
					in++;
				}
				
				if (*in == '.')
					throw Mojangson::Exception(in, "Int arrays cannot have decimals");
				else if (*in == '\0')
					throw Mojangson::Exception(in, "Unexpected end of tag");
				else if (numberAsText.empty())
					throw Mojangson::Exception(in, "Unreadable number in int array");

				values.push_back((int)std::stoi(numberAsText) * multiplier);
				MojangsonToken::SkipWhitespaces(in);
				while (MojangsonToken::IsWhitespace(*in) || *in == MojangsonToken::ELEMENT_SEPERATOR)
					in++;
				if (*in == '\0') throw Mojangson::Exception(in, "Unexpected end of tag");
			}
			if (*in == '\0') throw Mojangson::Exception(in, "Unexpected end of tag");
			in++;  // means we are skiping array end
			MojangsonToken::SkipWhitespaces(in);

			return std::make_unique<IntArrayTag>(values);
		} else {
			if (*in == MojangsonToken::ARRAY_TYPE_SUFFIX)
				in++;
			MojangsonToken::SkipWhitespaces(in);
			std::unique_ptr<ListTag> tag = std::make_unique<ListTag>();
			while (*in != MojangsonToken::ARRAY_END && *in != '\0') {
				const char* prevIn = in;
				std::unique_ptr<Tag> parsedTag = ParseValue(in);
				if (in == prevIn)
					throw Mojangson::Exception(in, "Invalid tag, unterminated array");
				tag->emplace_back(std::move(parsedTag));
				// an old parser error made it so you had to put ,, so to keep that compatibility just ignore all of the now useless ones
				while (MojangsonToken::IsWhitespace(*in) || *in == MojangsonToken::ELEMENT_SEPERATOR)
					in++;
				if (*in == '\0') throw Mojangson::Exception(in, "Unexpected end of tag");
			}
			if (*in == '\0') throw Mojangson::Exception(in, "Unexpected end of tag");
			in++; //means we are skiping array end
			MojangsonToken::SkipWhitespaces(in);
			return tag;
		}
	} break;
	}

	return nullptr;
}

static std::string FromTagImpl(Tag* tag, bool formatted, const std::string& tabs) {
	switch (tag->getType()) {
	case Tag::Type::Byte:
		return std::to_string(((ByteTag*)tag)->value) + std::string({MojangsonToken::BYTE_SUFFIX});
	case Tag::Type::Short:
		return std::to_string(((ShortTag*)tag)->value) + std::string({MojangsonToken::SHORT_SUFFIX});
	case Tag::Type::Int:
		return std::to_string(((ShortTag*)tag)->value);
	case Tag::Type::Int64:
		return std::to_string(((Int64Tag*)tag)->value) + std::string({MojangsonToken::LONG_SUFFIX});
	case Tag::Type::Float:
		return std::to_string(((FloatTag*)tag)->value) + std::string({MojangsonToken::FLOAT_SUFFIX});
	case Tag::Type::Double:
		return std::to_string(((DoubleTag*)tag)->value) + std::string({MojangsonToken::DOUBLE_SUFFIX});
	case Tag::Type::ByteArray: {
		std::string byteArray({MojangsonToken::ARRAY_START, MojangsonToken::BYTE_ARRAY_PREFIX, MojangsonToken::ARRAY_TYPE_SUFFIX});
		std::string values;
		if (formatted) {
			for (auto byte : *(ByteArrayTag*)tag)
				values += std::to_string(byte) + std::string({MojangsonToken::BYTE_SUFFIX, MojangsonToken::ELEMENT_SEPERATOR, MojangsonToken::WHITE_SPACE});
			if (values.size() > 2)  // erase last ", "
				values.erase(values.end() - 2, values.end());
			byteArray += values;
		} else {
			for (auto byte : *(ByteArrayTag*)tag)
				values += std::to_string(byte) + std::string({MojangsonToken::BYTE_SUFFIX, MojangsonToken::ELEMENT_SEPERATOR});
			if (values.size() > 1)  // erase last ","
				values.erase(values.end() - 1, values.end());
			byteArray += values;
		}
		byteArray.push_back(MojangsonToken::ARRAY_END);
		return byteArray;
	}
	case Tag::Type::String: {
		std::string result;
		result.reserve(((StringTag*)tag)->size() + 10);
		result.push_back(MojangsonToken::STRING_QUOTES);
		for (const char c : *(StringTag*)tag) {
			//some strings contain '\0'
			if (c == '\0') break;
			switch (c) {
			case MojangsonToken::STRING_QUOTES:
				result.push_back('\\');
				result.push_back(MojangsonToken::STRING_QUOTES);
				break;
			case MojangsonToken::STRING_ESCAPE:
				result.push_back(MojangsonToken::STRING_ESCAPE);
				result.push_back(MojangsonToken::STRING_ESCAPE);
				break;
			case '\n':
				result.push_back('\\');
				result.push_back('n');
				break;
			case '\r':
				result.push_back('\\');
				result.push_back('r');
				break;
			case '\t':
				result.push_back('\\');
				result.push_back('t');
				break;
			default:
				result.push_back(c);
				break;
			}
		}
		
		result.push_back(MojangsonToken::STRING_QUOTES);
		return result;
	}
	case Tag::Type::IntArray: {
		std::string intArray({MojangsonToken::ARRAY_START, MojangsonToken::INT_ARRAY_PREFIX, MojangsonToken::ARRAY_TYPE_SUFFIX});
		std::string values;
		if (formatted) {
			for (auto num : *(IntArrayTag*)tag)
				values += std::to_string(num) + std::string({MojangsonToken::ELEMENT_SEPERATOR, MojangsonToken::WHITE_SPACE});
			if (values.size() > 2)  // erase last ", "
				values.erase(values.end() - 2, values.end());
			intArray += values;
		} else {
			for (auto num : *(IntArrayTag*)tag)
				values += std::to_string(num) + std::string({MojangsonToken::ELEMENT_SEPERATOR});
			if (values.size() > 1)  // erase last ","
				values.erase(values.end() - 1, values.end());
			intArray += values;
		}
		intArray.push_back(MojangsonToken::ARRAY_END);
		return intArray;
	}
	case Tag::Type::List: {
		std::string arrayTag;
		std::string newTabs = tabs + std::string({MojangsonToken::WHITE_SPACE, MojangsonToken::WHITE_SPACE});
		std::string values;
		arrayTag.push_back(MojangsonToken::ARRAY_START);
		if (formatted) {
			arrayTag += tabs;
			arrayTag.push_back('\n');
			for (std::unique_ptr<Tag>& tag : *(ListTag*)tag) 
				values += newTabs + FromTagImpl(tag.get(), formatted, newTabs) + std::string({MojangsonToken::ELEMENT_SEPERATOR, MojangsonToken::WHITE_SPACE, '\n'});
			if (values.size() > 3)
				values.erase(values.end() - 3, values.end());  // remove ", \n"
			arrayTag += values + "\n" + tabs;
		} else {
			for (std::unique_ptr<Tag>& tag : *(ListTag*)tag)
				values += FromTagImpl(tag.get(), formatted, newTabs) + std::string({MojangsonToken::ELEMENT_SEPERATOR});
			if (values.size() > 1)
				values.erase(values.end() - 1, values.end());  // remove ","
			arrayTag += values;
		}
		arrayTag.push_back(MojangsonToken::ARRAY_END);
		return arrayTag;
	}
	case Tag::Type::Compound: {
		std::string compoundTag;
		std::string newTabs = tabs + std::string({MojangsonToken::WHITE_SPACE, MojangsonToken::WHITE_SPACE});
		std::string values;
		compoundTag.push_back(MojangsonToken::COMPOUND_START);
		if (formatted) {
			compoundTag += tabs;
			compoundTag.push_back('\n');
			for (auto& [key, value] : *(CompoundTag*)tag) 
				values += newTabs + std::string({MojangsonToken::STRING_QUOTES}) + key + std::string({MojangsonToken::STRING_QUOTES, MojangsonToken::ELEMENT_PAIR_SEPERATOR, MojangsonToken::WHITE_SPACE}) + FromTagImpl((Tag*)&value, formatted, newTabs) + std::string({MojangsonToken::ELEMENT_SEPERATOR, MojangsonToken::WHITE_SPACE, '\n'});
			if (values.size() > 3)
				values.erase(values.end() - 3, values.end());  // remove ", \n"
			compoundTag += values + "\n" + tabs;
		} else {
			for (auto& [key, value] : *(CompoundTag*)tag)
				values += std::string({MojangsonToken::STRING_QUOTES}) + key + std::string({MojangsonToken::STRING_QUOTES, MojangsonToken::ELEMENT_PAIR_SEPERATOR}) + FromTagImpl((Tag*)&value, formatted, newTabs) + std::string({MojangsonToken::ELEMENT_SEPERATOR});
			if (values.size() > 1)
				values.erase(values.end() - 1, values.end());  // remove ","
			compoundTag += values;
		}
		compoundTag.push_back(MojangsonToken::COMPOUND_END);
		return compoundTag;
	}
	default:
		break;
	}
	return "";
}


std::string Mojangson::FromTag(Tag* tag, bool formatted) {
	return FromTagImpl(tag, formatted, std::string());
}
