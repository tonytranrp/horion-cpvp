#include "Biome.h"

std::optional<Vec3i> BiomeSourceUtil::locateBiome(std::string const& biomeName, BiomeSource* biomeSource, const Vec3i& origin, int32_t range, uint32_t stepSize) {
	using locateBiome_t = std::optional<Vec3i>*(__cdecl*)(std::optional<Vec3i>* result, void* callback, BiomeSource* src, const Vec3i& origin, const Vec3i* boundingBox, uint32_t stepSize);
	static auto locateBiomeFunc = reinterpret_cast<locateBiome_t>(FindSignature("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4D 8B E9 4C 89 4D"));

	// set scan bounds...
	// really this is like a `class BoundingBox'
	Vec3i bounds[2];
	bounds[0] = origin;
	bounds[0].x -= range;
	bounds[0].z -= range;
	bounds[1].x = bounds[0].x + range * 2;
	bounds[1].z = bounds[0].z + range * 2;
	bounds[0].y = -64;
	bounds[1].y = 320;

	std::string nameCopy = biomeName;
	BiomeCallback callback(&nameCopy);

	std::optional<Vec3i> result{};
	locateBiomeFunc(&result, &callback, biomeSource, origin, bounds, stepSize);
	return result;
}