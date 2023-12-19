#pragma once

#include "../Utils/Json.hpp"


namespace MinecraftJson {

class Value { // equivalent to bds' Json::Value, but we dont wanna confuse it with nlohmann json
	char pad[16];

public:
	std::string *toStyledString(std::string *out);
	nlohmann::json toNlohmannJson();
	static MinecraftJson::Value fromNlohmannJson(nlohmann::json const &json);

	Value() {
		memset(this, 0, sizeof(*this));
	}
};

class Reader {
	char pad[192];

public:
	void Parse(std::string* input, MinecraftJson::Value* output);

	Reader();
};

} // namespace MinecraftJson
static_assert(sizeof(MinecraftJson::Value) == 0x10);

//class MinecraftJson {
//public:
//
//	struct CZString {
//		const char *mStr;
//		uint32_t mIndex;
//
//		CZString() : mStr(nullptr), mIndex(0) {}
//		CZString(const char *s, uint32_t index) : mStr(s), mIndex(index) {}
//	};
//
//	union ValueHolder {
//		int64_t mInt;
//		uint64_t mUInt;
//		long double mReal;
//		bool mBool;
//		char *mString;
//		std::map<CZString, MCJson> *mMap;
//	};
//
//	ValueHolder mValue;
//	int32_t mType : 8;
//	int32_t mAllocated : 1;
//
//	MinecraftJson(std::string const &str);
//	MinecraftJson(nlohmann::json const &nonBloatedJson);
//
//	std::string *toStyledString(std::string *out);
//	nlohmann::json toNlohmannJson();
//	static MinecraftJson fromNlohmannJson(nlohmann::json const &nonBloatedJson);
//};
//
//static_assert(sizeof(MinecraftJson) == 0x10);