#pragma once

#include "../Utils/Logger.h"
#include <array>
#include <cstdint>
#include <string_view>

enum class OptionType : int32_t {
	Bool = 0,
	InputModeBool = 1,
	Float = 2,
	InputModeFloat = 3,
	String = 4,
	Int = 5,
	Enum = 6
};

class OptionInfo {
	uint8_t pad_0000[0x108]{};
public:

	std::string name;          // 0x108, "gfx_field_of_view"
	std::string friendlyName;  // 0x128, "fieldOfView"
	int32_t id;
	OptionType type;

private:
	uint8_t pad_0x148[0x2]{};

public:
	std::string translationKey;  // 0x158, "options.fov"
};

static_assert(offsetof(OptionInfo, name) == 0x108);
static_assert(offsetof(OptionInfo, friendlyName) == 0x128);
static_assert(offsetof(OptionInfo, translationKey) == 0x158);

class Option {
	void* vtbl;
public:
	OptionInfo* info;
	union {
		int32_t valueI;
		float valueF;
	};
	union {
		int32_t minI;
		float minF;
	};
	union {
		int32_t maxI;
		float maxF;
	};
};

class Options {
	uint8_t pad[0x18]{};
public:
	static constexpr inline int32_t MAX_OPTION_ID = 604; // approximate?

	inline bool hasOption(int32_t id) {
		return Utils::CallVFunc<10, bool>(this, id);
	}

	inline Option* getOption(int32_t id) {
		Option* out;
		Utils::CallVFunc<11, void*>(this, &out, id);
		return out;
	}

	inline Option* getOptionByName(std::string_view name) {
		for (int32_t i = 0; i <= MAX_OPTION_ID; i++) {
			if (this->hasOption(i)) {
				auto opt = this->getOption(i);
				if (opt && opt->info) {
					if (opt->info->name == name) {
						return opt;
					}
				}
			}
		}
		return nullptr;
	}

	inline Option* getOptionByFriendlyName(std::string_view friendlyName) {
		for (int32_t i = 0; i <= MAX_OPTION_ID; i++) {
			if (this->hasOption(i)) {
				auto opt = this->getOption(i);
				if (opt && opt->info) {
					if (opt->info->friendlyName == friendlyName) {
						return opt;
					}
				}
			}
		}
		return nullptr;
	}

	inline Option* getOptionByTranslatedName(std::string_view translatedName) {
		for (int32_t i = 0; i <= MAX_OPTION_ID; i++) {
			if (this->hasOption(i)) {
				auto opt = this->getOption(i);
				if (opt && opt->info) {
					if (opt->info->translationKey == translatedName) {
						return opt;
					}
				}
			}
		}
		return nullptr;
	}

	void printOptions() {
		for (int32_t i = 0; i <= MAX_OPTION_ID; i++) {
			if (this->hasOption(i)) {
				auto opt = this->getOption(i);
				if (opt && opt->info) {
					logF("%d, %s, %s, %s",
						i,
						opt->info->name.c_str(),
						opt->info->friendlyName.c_str(),
						opt->info->translationKey.c_str()
					);
				}
			}
		}
	}
};