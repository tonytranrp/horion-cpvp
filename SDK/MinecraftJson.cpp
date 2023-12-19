#include "MinecraftJson.h"

#include "../Memory/GameData.h"
#include "../Utils/Utils.h"

std::string *MinecraftJson::Value::toStyledString(std::string *out) {
	using toStyledString_t = std::string *(__fastcall *)(void *, std::string *);
	static auto JSONToStringFunc = reinterpret_cast<toStyledString_t>(FindSignature("48 89 5C 24 ? 55 56 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 48 8B DA 48 89 55 ? 0F 57 C0"));
	return JSONToStringFunc(this, out);
}

 nlohmann::json MinecraftJson::Value::toNlohmannJson() {
	std::string gt;
	nlohmann::json json = nlohmann::json::parse(this->toStyledString(&gt)->c_str());
	return json;
 }

 MinecraftJson::Value MinecraftJson::Value::fromNlohmannJson(nlohmann::json const &json) {
	 std::string t(json.dump(0));
	 MinecraftJson::Value JsonValue;
	 MinecraftJson::Reader* JsonReader = &MinecraftJson::Reader::Reader();
	 JsonReader->Parse(&t, &JsonValue);
	 return JsonValue;
 }

MinecraftJson::Reader::Reader() {
	memset(this, 0x0, sizeof(MinecraftJson::Reader));
	using JsonReader_t = void *(__fastcall *)(MinecraftJson::Reader*);
	static auto JsonReader = reinterpret_cast<JsonReader_t>(FindSignature("48 89 5C 24 ? 48 89 74 24 ? 48 89 4C 24 ? ? 48 83 EC ? 48 8B F9 ? ? 48 89 31 48 89 71 ? 48 89 71 ? 48 89 71 ? 48 89 71 ? E8 ? ? ? ? 48 8B 10 4C 8B 42 ? ? ? ? 48 8B C8"));
	if (JsonReader)
		JsonReader(this);
}

void MinecraftJson::Reader::Parse(std::string* input, MinecraftJson::Value* output) {
	using JsonReaderParse_t = void *(__fastcall *)(MinecraftJson::Reader*, std::string*, MinecraftJson::Value*, bool);
	static auto JsonReaderParse = reinterpret_cast<JsonReaderParse_t>(FindSignature("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? ? 48 83 EC ? ? ? ? ? ? 49 8B F0 48 8B 7A ? 48 8B DA 48 8B E9 ? ? 48 8B 1A 48 85 DB"));
	if (JsonReaderParse)
		JsonReaderParse(this, input, output, 0);
}