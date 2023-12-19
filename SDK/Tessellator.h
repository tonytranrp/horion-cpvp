#pragma once

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"

#include "BlockLegacy.h"
#include "ClientInstance.h"
#include "ScreenContext.h"

class Block;
class Mesh;
class ScreenContext;

class MaterialPtr {
public:
	std::shared_ptr<MaterialPtr> ptr;
	HashedString name{};

	MaterialPtr(std::string materialName, bool switchable = false);
};

class BedrockTextureData {
private:
	void *idk; // I don't think these are owned, so won't free them off the memory
	void *idk2;

public:
	std::shared_ptr<ResourceLocation> resource;

private:
	void* empty[4];

	~BedrockTextureData() {
	}
};

static_assert(sizeof(BedrockTextureData) == 64);

class ResourceLocation {
private:
	uint64_t type;  // 0x0000
public:
	std::string filePath;  // 0x0008

	ResourceLocation(std::string filePath, bool external) {
		memset(this, 0, sizeof(ResourceLocation));
		this->filePath = filePath;
		if (external)
			this->type = 2;
	};
};

class TexturePtr {
public:
	std::shared_ptr<BedrockTextureData> textureData;
	std::shared_ptr<ResourceLocation> location;

	~TexturePtr() {
	}
};

enum class VertexFormat {
	QUAD = 1,
	TRIANGLE_LIST,
	TRIANGLE_STRIP,
	LINE_LIST,
	LINE_STRIP
};

class Tessellator {
private:
	char pad_0000[0x15C];
	Vec3 transformOffset; // 0x15C

public:
	void addPostTransformOffset(int x, int y, int z) {
		addPostTransformOffset((float)x, (float)y, (float)z);
	}
	void addPostTransformOffset(float x, float y, float z);

	inline void setTransformOffset(const Vec3& offset) {
		this->transformOffset = offset;
	}
	inline void setTransformOffset(float x, float y, float z) {
		new (&this->transformOffset) Vec3(x, y, z);
	}

	inline void begin(VertexFormat format, int reservedVertices = 0) {
		using tess_begin_t = void(__fastcall *)(Tessellator *, VertexFormat, int);
		static tess_begin_t tess_begin = reinterpret_cast<tess_begin_t>(FindSignature("48 89 5C 24 ?? 55 48 83 EC ?? 80 B9 ?? ?? ?? ?? 00 45"));
		tess_begin(this, format, reservedVertices);
	}
	inline void vertex(float x, float y, float z) {
		using tess_vertex_t = void(__fastcall *)(Tessellator *, float, float, float);
		static tess_vertex_t tess_vertex = reinterpret_cast<tess_vertex_t>(FindSignature("40 57 48 81 EC ? ? ? ? 0F 29 7C 24 ? 48 8B 05"));
		tess_vertex(this, x, y, z);
	}
	inline void color(float r, float g, float b, float a) {
		using tess_color_t = void(__fastcall *)(Tessellator *, float, float, float, float);
		static tess_color_t tess_color = reinterpret_cast<tess_color_t>(FindSignature("80 B9 E4 ? ? ? ? 4C 8B C1 75 ? ? ? ? 25 ? ? ? ?"));
		tess_color(this, r, g, b, a);
	}
	inline void setRotation(float angle, Vec3 pivot) {
		using tess_setRotation_t = void(__fastcall *)(Tessellator *, float, Vec3);
		static tess_setRotation_t tess_setRotation = reinterpret_cast<tess_setRotation_t>(FindSignature("40 53 48 81 EC ? ? ? ? F3 ? 0F ? ? 4C 8D 4C 24 ? F3 ? 0F ? ? ? 48 8D 54 24 ?"));
		tess_setRotation(this, angle, pivot);
	}
	inline void resetTransform(bool a1) {
		using tess_resetTransform_t = void(__fastcall *)(Tessellator *, bool);
		static tess_resetTransform_t tess_resetTransform = reinterpret_cast<tess_resetTransform_t>(FindSignature("40 53 48 81 EC ? ? ? ? C6 81 A0 ? ? ? ? 48 8B D9 ? ? ? ? 48 C7 81 A4 ? ? ? ? 00 80 3F"));
		tess_resetTransform(this, a1);
	}
	inline void renderMeshImmediately(ScreenContext* ctx, MaterialPtr* material) {
		using meshHelper_renderImm_t = void(__fastcall *)(ScreenContext *, Tessellator *, MaterialPtr *);
		static meshHelper_renderImm_t meshHelper_renderImm = reinterpret_cast<meshHelper_renderImm_t>(FindSignature("40 55 53 56 57 41 56 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 49 8B F0 48 8B DA"));
		meshHelper_renderImm(ctx, this, material);
	}
};

class BlockTessellator {
public:
	Mesh* getMeshForBlockInWorld(Tessellator * t, Block * block, Vec3i & pos);
	void tesselateBlockInWorld(Tessellator *t, Block *block, Vec3i &pos);
};