#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <Utils/HMath.h>
#include <SDK/HashedString.h>

class Biome {
	uintptr_t** vtable;
public:
	HashedString hashedName;
	// ...
};

class BiomeSource {
	uintptr_t** vtable;
public:
	// ...
};

namespace BiomeSourceUtil {
std::optional<Vec3i> locateBiome(std::string const& biomeName, BiomeSource* biomeSource, const Vec3i& origin, int32_t range = 4000, uint32_t stepSize = 64);
}

class BiomeCallback {
public:
	std::string* wantedBiome{};
	char pad_0x10[0x28]{};
	void* ptrToSelf{};
	virtual void filler_0(){};
	virtual void filler_1(){};
	virtual bool doCall(Biome* biome) {
		std::string& current = biome->hashedName.text;
		return current == *wantedBiome;
	}
	virtual void filler_2(){};
	virtual void filler_3(){};

	BiomeCallback(std::string* biomeName) {
		wantedBiome = biomeName;
		ptrToSelf = this;
	}
};