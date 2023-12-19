#pragma once

struct MojangsonToken {
	enum : char {
		COMPOUND_START = '{',
		COMPOUND_END = '}',
		ELEMENT_SEPERATOR = ',',
		ARRAY_START = '[',
		ARRAY_END = ']',
		ELEMENT_PAIR_SEPERATOR = ':',
		STRING_ESCAPE = '\\',
		STRING_QUOTES = '"',
		DOUBLE_SUFFIX = 'd',
		BYTE_SUFFIX = 'b',
		FLOAT_SUFFIX = 'f',
		SHORT_SUFFIX = 's',
		LONG_SUFFIX = 'l',
		BYTE_ARRAY_PREFIX = 'B',
		INT_ARRAY_PREFIX = 'I',
		ARRAY_TYPE_SUFFIX = ';',
		WHITE_SPACE = ' ',
	};

	static inline bool IsWhitespace(int c) {
		if (c > -1 && c <= 255)
			return std::isspace(c) || c == '\t' || c == '\n' || c == '\r';
		else
			return true;
	}
	static inline void SkipWhitespaces(const char*& text) {
		while (IsWhitespace(*text))
			text++;
	}
};
