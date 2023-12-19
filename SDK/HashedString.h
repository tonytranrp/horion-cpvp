#pragma once
#include "../Utils/HMath.h"
#include "../Utils/Utils.h"

class HashedString {
public:
	uint64_t hash;
	std::string text;
	HashedString* lastCmp = nullptr;

	static constexpr inline uint64_t computeHash(const char* str) {  // xref: HashedString::computeHash
		if (!str || !(*str)) return 0;
		uint8_t c = static_cast<uint8_t>(*str);
		const char* nextData = str + 1;
		constexpr uint64_t VAL_64 = 0xCBF29CE484222325, PRIME_64 = 0x100000001B3;
		uint64_t result = VAL_64;
		do {
			result = c ^ (PRIME_64 * result);
			c = *nextData++;
		} while (c);
		return result;
	}

	HashedString() : hash(0) {}
	HashedString(uint64_t inputhash, const std::string& inputText) : hash(inputhash), text(inputText) {}
	HashedString(const std::string& text) : text(text) {
		this->hash = HashedString::computeHash(text.c_str());
	}

	inline bool operator==(const HashedString& rhs) const {
		return (this->text == rhs.text) && (this->hash == rhs.hash);
	}
	inline bool operator!=(const HashedString& rhs) const { return !(*this == rhs); }
};

namespace std {
template <>
struct hash<HashedString> {
	size_t operator()(const HashedString& k) const {
		return k.hash;
	}
};
}  // namespace std