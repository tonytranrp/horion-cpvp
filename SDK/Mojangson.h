#pragma once
#include "Tag.h"

// Collection of helpers for parsing and stringifying NBT/Mojangson.
// NOTE: This is not ready for use.
namespace Mojangson {
	class Exception : public std::exception {
	public:
		int lineNumber = 1;
		int columnNumber = 1;
		std::string MojangsonError;
		Exception(const char*& currentInput, const char* const message) { 
			const char* start = ((const char**)&currentInput)[1];
			int distance = (int)(currentInput - start);
			int currentLineChars = 1;
			for (int i = 0; i < distance; i++) {
				if (start[i] == '\n') {
					currentLineChars = 1;
					lineNumber++;
				} else if (start[i] != '\r')
					currentLineChars++;
			}
			columnNumber = currentLineChars;
			MojangsonError = std::to_string(lineNumber) + ":" + std::to_string(columnNumber) + std::string(" ") + message;
		}
		const char* what() const override { return MojangsonError.c_str(); }
	};
	//if you call this you will not remain alive.
	extern std::unique_ptr<Tag> ParseInternal(const char*& in);



	/*
	 * Parses the given string stream into a Tag object, which could be of any type.
	 * Throws Mojangson::Exception if the tag is incorrect.
	 */
	inline std::unique_ptr<Tag> Parse(const std::string_view& in) {
		std::string viewedString(in.data(), in.size());
		const char* input[2] = {viewedString.c_str(), viewedString.c_str()};
		return Mojangson::ParseInternal(*input);
	}
	/*
	 * Parses the given string stream into a Tag object, which could be of any type.
	 * Throws Mojangson::Exception if the tag is incorrect.
	 */
	inline std::unique_ptr<Tag> Parse(const char* in) {
		const char* input[2] = {in, in};
		return Mojangson::ParseInternal(*input);
	}

	/*
	 * Parses the given string stream into a Tag object, which could be of any type.
	 * Throws Mojangson::Exception if the tag is incorrect.
	 */
	inline std::unique_ptr<Tag> Parse(const std::string& in){
		const char* input[2] = {in.c_str(), in.c_str()};
		return Mojangson::ParseInternal(*input);
	}

	extern std::string FromTag(Tag* tag, bool formatted = false);
};